#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace cortex {
    /**
     * Minimal file-based preset manager: saves/loads the full parameter state
     * as versioned XML in the user's application data directory
     * (<AppData>/<CTX_COMPANY_NAME>/<CTX_PLUGIN_NAME>/Presets). Intended as
     * the plumbing under a plugin-specific preset browser UI.
     */
    class PresetManager {
    public:
        /**
         * Hook for adapting state saved by older plugin versions, shared by
         * session restore and preset loading. APVTS::replaceState already
         * tolerates missing parameters (they keep their current value) and
         * unknown ones (ignored), so this only needs to handle ID renames,
         * range remaps, and the like.
         */
        using StateMigrationFn = juce::ValueTree (*)(juce::ValueTree state, int fromVersion);

        PresetManager(juce::AudioProcessorValueTreeState& parameters, int stateVersion, StateMigrationFn migrateState = nullptr);

        juce::File GetPresetDirectory() const;
        juce::StringArray ListPresets() const;
        bool SavePreset(const juce::String& name);
        bool LoadPreset(const juce::String& name);

        void StampStateVersion(juce::XmlElement& xml) const;
        bool RestoreStateFromXml(const juce::XmlElement& xml);

    private:
        juce::AudioProcessorValueTreeState& m_parameters;
        int m_stateVersion;
        StateMigrationFn m_migrateState;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PresetManager)
    };
}
