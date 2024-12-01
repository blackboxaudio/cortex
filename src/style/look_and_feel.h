#pragma once

#include "juce_gui_basics/juce_gui_basics.h"

namespace cortex {

    class CortexLookAndFeel : public juce::LookAndFeel_V4 {
    public:
        CortexLookAndFeel() = default;
        ~CortexLookAndFeel() override = default;

        void drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider) override;

    private:
        const juce::Colour inactiveFill = juce::Colour::fromRGBA(255, 255, 255, 63);
        const juce::Colour activeFill = juce::Colour::fromRGBA(167, 98, 255, 255);
        const juce::Colour activeFillStart = juce::Colour::fromRGBA(116, 42, 208, 255);
        const juce::Colour activeFillStop = juce::Colour::fromRGBA(144, 78, 228, 255);

        float m_strokeWidth;
        float m_fontSize;
    };

}
