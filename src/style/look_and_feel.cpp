#include "look_and_feel.h"

using namespace cortex;

void CortexLookAndFeel::drawRotarySlider(juce::Graphics& g, int x, int y, int width, int height, float sliderPos, float rotaryStartAngle, float rotaryEndAngle, juce::Slider& slider)
{
    // get all necessary variables
    const float radius = juce::jmin(width / 2, height / 2) - 2.0f;
    const float centerX = x + width * 0.5f;
    const float centerY = y + height * 0.5f;
    const float rx = centerX - radius;
    const float ry = centerY - radius;
    const float rw = radius * 2.0f;

    // even if slider is fully zero, still display something
    // if (sliderPos == 0.0f && slider.getName() != "inversion") sliderPos = 0.01f;
    const float angle = rotaryStartAngle + sliderPos * (rotaryEndAngle - rotaryStartAngle);

    // get readout value
    const double value = slider.getValue();
    juce::String readoutValue = (value >= 1000.0 ? juce::String(value / 1000.0, 1) + "k" : juce::String(value, 1));
    juce::String readout = readoutValue + slider.getTextValueSuffix();

    // set string properties according to slider
    auto name = slider.getName();
    if (name == "rate") {
        readoutValue = value >= 1000.0 ? juce::String(value / 1000.0, 2) + "k" : juce::String(value, 1);
        readout = readoutValue + slider.getTextValueSuffix();
    } else if (name == "mix" || name == "level") {
        readout = juce::String(value, 1) + slider.getTextValueSuffix();
    } else {
        readout = juce::String(value, 2) + slider.getTextValueSuffix();
    }

    // handle slider coloring (esp invert button / slider)
    if (name == "inversion") {
        readout = "ON";
        g.setColour(slider.getValue() > 0.5f ? activeFill : inactiveFill);
    } else if (name == "pulseWidth" && !slider.isEnabled()) {
        g.setColour(inactiveFill);
    } else {
        g.setColour(activeFill);
    }

    // draw text accordingly
    g.drawText(
        readout,
        (int)(centerX - radius), // int x (radius for suffix offset)
        (int)(centerY - (m_fontSize * 5.0f / 12.0f)), // int y (ratio based off font size)
        (int)rw, // int width
        (int)m_fontSize, // int height
        juce::Justification::centred);

    // set color variables for gradient
    juce::ColourGradient sliderFill = juce::ColourGradient(activeFillStart, (float)x, 0.0f, activeFillStop, (float)width, 0.0f, false);
    g.setGradientFill(sliderFill);
    if (slider.getName() == "pulseWidth" && !slider.isEnabled())
        g.setColour(inactiveFill);

    // draw the filled path
    juce::Path arcPathFilled;
    arcPathFilled.addArc(rx, ry, rw, rw, rotaryStartAngle, angle, true);
    juce::PathStrokeType(3.0f).createStrokedPath(arcPathFilled, arcPathFilled);
    g.fillPath(arcPathFilled);
}
