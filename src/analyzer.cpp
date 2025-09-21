#include "cortex/analyzer.h"

using namespace cortex;

Analyzer::Analyzer()
{
    juce::zeromem(m_fftOutputData, sizeof(m_fftOutputData));
}

void Analyzer::ProcessAudioBuffer(juce::AudioBuffer<float>& buffer)
{
    LoadFFTQueue(buffer);
    ProcessFFT();
}

void Analyzer::LoadFFTQueue(juce::AudioBuffer<float>& buffer)
{
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();
    if (numSamples == 0)
        return;

    for (int channelIdx = 1; channelIdx < numChannels; ++channelIdx) {
        buffer.addFrom(0, 0, buffer, channelIdx, 0, numSamples);
    }

    if (numChannels > 1) {
        buffer.applyGain(0, 0, numSamples, 1.0f / static_cast<float>(numChannels));
    }

    const float* monoData = buffer.getReadPointer(0);
    int samplesProcessed = 0;
    while (samplesProcessed < numSamples) {
        const int samplesToFill = juce::jmin(FFT_SIZE - static_cast<int>(m_fftQueueIndex), numSamples - samplesProcessed);

        std::memcpy(&m_fftQueue[m_fftQueueIndex], &monoData[samplesProcessed], static_cast<unsigned long>(samplesToFill) * sizeof(float));

        m_fftQueueIndex += static_cast<unsigned int>(samplesToFill);
        samplesProcessed += samplesToFill;

        if (m_fftQueueIndex == FFT_SIZE) {
            if (!m_isNextFftBlockReady) {
                std::memcpy(m_fftData, m_fftQueue, sizeof(m_fftQueue));
                m_isNextFftBlockReady = true;
            }
            m_fftQueueIndex = 0;
        }
    }
}

void Analyzer::ProcessFFT()
{
    if (m_isNextFftBlockReady) {
        m_window.multiplyWithWindowingTable(m_fftData, FFT_SIZE);
        m_forwardFft.performFrequencyOnlyForwardTransform(m_fftData);

        const float smoothingFactor = 0.3f;
        for (int i = 0; i < FFT_OUTPUT_SIZE; i++) {
            m_fftOutputData[i] = m_fftOutputData[i] * (1.0f - smoothingFactor) + m_fftData[i] * smoothingFactor;
        }

        m_isNextFftBlockReady = false;
    } else {
        // Gentle decay if data is NOT ready
        for (float& i : m_fftOutputData) {
            i *= 0.95f;
        }
    }
}
