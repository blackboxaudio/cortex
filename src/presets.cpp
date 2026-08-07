#include "cortex/presets.h"

#ifndef CTX_COMPANY_NAME
#define CTX_COMPANY_NAME "Cortex"
#endif

#ifndef CTX_PLUGIN_NAME
#define CTX_PLUGIN_NAME "Unknown"
#endif

namespace {
    const char* const PRESET_EXTENSION = ".preset";
    const char* const STATE_VERSION_ATTRIBUTE = "stateVersion";
}

cortex::PresetManager::PresetManager(juce::AudioProcessorValueTreeState& parameters, int stateVersion, StateMigrationFn migrateState)
    : m_parameters(parameters)
    , m_stateVersion(stateVersion)
    , m_migrateState(migrateState)
{
}

juce::File cortex::PresetManager::GetPresetDirectory() const
{
    return juce::File::getSpecialLocation(juce::File::userApplicationDataDirectory)
        .getChildFile(CTX_COMPANY_NAME)
        .getChildFile(CTX_PLUGIN_NAME)
        .getChildFile("Presets");
}

juce::StringArray cortex::PresetManager::ListPresets() const
{
    juce::StringArray names;
    for (const auto& file : GetPresetDirectory().findChildFiles(
             juce::File::findFiles, false, juce::String("*") + PRESET_EXTENSION)) {
        names.add(file.getFileNameWithoutExtension());
    }
    names.sortNatural();
    return names;
}

bool cortex::PresetManager::SavePreset(const juce::String& name)
{
    const auto legalName = juce::File::createLegalFileName(name);
    if (legalName.isEmpty())
        return false;

    const auto directory = GetPresetDirectory();
    if (!directory.createDirectory())
        return false;

    const std::unique_ptr<juce::XmlElement> xml(m_parameters.copyState().createXml());
    if (xml == nullptr)
        return false;

    StampStateVersion(*xml);
    return xml->writeTo(directory.getChildFile(legalName + PRESET_EXTENSION));
}

bool cortex::PresetManager::LoadPreset(const juce::String& name)
{
    const auto file = GetPresetDirectory().getChildFile(
        juce::File::createLegalFileName(name) + PRESET_EXTENSION);
    if (!file.existsAsFile())
        return false;

    const std::unique_ptr<juce::XmlElement> xml(juce::XmlDocument::parse(file));
    if (xml == nullptr)
        return false;

    return RestoreStateFromXml(*xml);
}

void cortex::PresetManager::StampStateVersion(juce::XmlElement& xml) const
{
    xml.setAttribute(STATE_VERSION_ATTRIBUTE, m_stateVersion);
}

bool cortex::PresetManager::RestoreStateFromXml(const juce::XmlElement& xml)
{
    if (!xml.hasTagName(m_parameters.state.getType()))
        return false;

    const int fromVersion = xml.getIntAttribute(STATE_VERSION_ATTRIBUTE, 0);
    auto state = juce::ValueTree::fromXml(xml);
    if (m_migrateState != nullptr)
        state = m_migrateState(std::move(state), fromVersion);

    m_parameters.replaceState(state);
    return true;
}
