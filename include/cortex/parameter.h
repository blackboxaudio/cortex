#pragma once

#include <juce_audio_processors/juce_audio_processors.h>

namespace cortex {
    enum class ParameterType {
        Boolean,
        Choice,
        Float,
    };

    struct BooleanParameterData {
        bool defaultValue;
    };

    struct ChoiceParameterData {
        juce::StringArray choices;
        int defaultValueIndex;
    };

    struct FloatParameterData {
        juce::NormalisableRange<float> range;
        float defaultValue;
        std::optional<juce::String> unit;
        int fractionDigits;
    };

    struct Parameter {
        juce::String name;
        ParameterType type;
        std::variant<BooleanParameterData, ChoiceParameterData, FloatParameterData> data;
    };

    class ParameterManager {
    public:
        static juce::AudioProcessorValueTreeState::ParameterLayout CreateParameterLayout(std::map<juce::String, Parameter> parameters);
        static std::map<juce::String, Parameter> LoadParametersFromJson(const juce::String& jsonString);
    };
}
