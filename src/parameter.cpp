#include "cortex/parameter.h"
#include "cortex/logger.h"

juce::AudioProcessorValueTreeState::ParameterLayout cortex::ParameterManager::CreateParameterLayout(std::map<juce::String, cortex::Parameter> parameters)
{
    std::vector<std::unique_ptr<juce::RangedAudioParameter>> layout;

    for (const auto& [id, param] : parameters) {
        std::unique_ptr<juce::RangedAudioParameter> parameter;

        switch (param.type) {
            case ParameterType::Boolean: {
                const auto& boolData = std::get<BooleanParameterData>(param.data);
                parameter = std::make_unique<juce::AudioParameterBool>(
                    juce::ParameterID(id),
                    param.name,
                    boolData.defaultValue);
                break;
            }
            case ParameterType::Choice: {
                const auto& choiceData = std::get<ChoiceParameterData>(param.data);
                parameter = std::make_unique<juce::AudioParameterChoice>(
                    juce::ParameterID(id),
                    param.name,
                    choiceData.choices,
                    choiceData.defaultValueIndex);
                break;
            }
            case ParameterType::Float: {
                const auto& floatData = std::get<FloatParameterData>(param.data);
                parameter = std::make_unique<juce::AudioParameterFloat>(
                    juce::ParameterID(id),
                    param.name,
                    floatData.range,
                    floatData.defaultValue,
                    floatData.unit.value_or(""));
                break;
            }
        }

        if (parameter) {
            layout.push_back(std::move(parameter));
        }
    }

    return { layout.begin(), layout.end() };
}

std::map<juce::String, cortex::Parameter> cortex::ParameterManager::LoadParametersFromJson(const juce::String& jsonString)
{
    std::map<juce::String, cortex::Parameter> parameters;

    auto json = juce::JSON::parse(jsonString);
    if (!json.isObject()) {
        BBX_ERROR("Failed to parse parameter JSON");
        return parameters;
    }

    auto* root = json.getDynamicObject();
    if (!root)
        return parameters;

    auto parametersArray = root->getProperty("parameters");
    if (!parametersArray.isArray()) {
        BBX_ERROR("No 'parameters' array found in JSON");
        return parameters;
    }

    auto* paramsArray = parametersArray.getArray();
    for (const auto& paramVar : *paramsArray) {
        if (!paramVar.isObject())
            continue;

        auto* paramObject = paramVar.getDynamicObject();
        if (!paramObject)
            continue;

        // Required
        juce::String id = paramObject->getProperty("id").toString();
        juce::String name = paramObject->getProperty("name").toString();
        juce::String typeString = paramObject->getProperty("type").toString();

        ParameterType type;
        if (typeString == "boolean") {
            type = ParameterType::Boolean;
        } else if (typeString == "choice") {
            type = ParameterType::Choice;
        } else if (typeString == "float") {
            type = ParameterType::Float;
        } else {
            BBX_ERROR("Unknown parameter type: " + typeString);
            continue;
        }

        Parameter param;
        param.name = name;
        param.type = type;

        switch (type) {
            case ParameterType::Boolean: {
                bool defaultValue = paramObject->getProperty("defaultValue");

                BooleanParameterData boolData = { defaultValue };
                param.data = boolData;

                break;
            }
            case ParameterType::Choice: {
                auto choicesVar = paramObject->getProperty("choices");
                int defaultValueIndex = static_cast<int>(paramObject->getProperty("defaultValueIndex"));

                juce::StringArray choices;
                if (choicesVar.isArray()) {
                    auto* choicesArray = choicesVar.getArray();
                    for (const auto& choice : *choicesArray) {
                        if (choice.isString()) {
                            choices.add(choice.toString());
                        } else {
                            choices.add(juce::String(static_cast<double>(choice)));
                        }
                    }
                }

                ChoiceParameterData choiceData = {
                    choices,
                    defaultValueIndex,
                };
                param.data = choiceData;

                break;
            }
            case ParameterType::Float: {
                float minValue = static_cast<float>(paramObject->getProperty("min"));
                float maxValue = static_cast<float>(paramObject->getProperty("max"));
                float defaultValue = static_cast<float>(paramObject->getProperty("defaultValue"));

                // Optional
                std::optional<juce::String> unit = paramObject->hasProperty("unit") ? std::make_optional(paramObject->getProperty("unit").toString()) : std::nullopt;
                float interval = paramObject->hasProperty("interval") ? static_cast<float>(paramObject->getProperty("interval")) : 0.0f;
                int fractionDigits = paramObject->hasProperty("fractionDigits") ? static_cast<int>(paramObject->getProperty("fractionDigits")) : 0;

                // Create NormalisableRange object
                juce::NormalisableRange<float> range;
                if (interval > 0.0f) {
                    range = juce::NormalisableRange<float>(minValue, maxValue, interval);
                } else {
                    range = juce::NormalisableRange<float>(minValue, maxValue);
                }

                if (paramObject->hasProperty("midpoint")) {
                    float midpoint = static_cast<float>(paramObject->getProperty("midpoint"));
                    range.setSkewForCentre(midpoint);
                }

                FloatParameterData floatData = {
                    range,
                    defaultValue,
                    unit,
                    fractionDigits,
                };
                param.data = floatData;

                break;
            }
        }

        BBX_DEBUG("Parsed parameter: " + name + " (type: " + typeString + ")");
        parameters[id] = param;
    }

    BBX_DEBUG("Parsed all parameters from JSON");
    return parameters;
}
