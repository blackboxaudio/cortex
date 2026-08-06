#include "cortex/analyzer.h"

#include <array>

using namespace cortex;

Analyzer::Analyzer()
{
    juce::zeromem(m_fftOutputData, sizeof(m_fftOutputData));
}

void Analyzer::ProcessAudioBuffer(const juce::AudioBuffer<float>& buffer) noexcept
{
    LoadFFTQueue(buffer);
    ProcessFFT();
}

void Analyzer::LoadFFTQueue(const juce::AudioBuffer<float>& buffer) noexcept
{
    const int numChannels = buffer.getNumChannels();
    const int numSamples = buffer.getNumSamples();
    if (numSamples == 0 || numSamples > FFT_SIZE)
        return;

    // Mix to mono using stack buffer (avoids mutating input buffer)
    std::array<float, FFT_SIZE> monoBuffer {};
    const float* firstChannel = buffer.getReadPointer(0);

    if (numChannels == 1) {
        std::memcpy(monoBuffer.data(), firstChannel, static_cast<size_t>(numSamples) * sizeof(float));
    } else {
        const float gain = 1.0f / static_cast<float>(numChannels);
        for (int i = 0; i < numSamples; ++i) {
            float sum = 0.0f;
            for (int ch = 0; ch < numChannels; ++ch) {
                sum += buffer.getReadPointer(ch)[i];
            }
            monoBuffer[static_cast<size_t>(i)] = sum * gain;
        }
    }

    const float* monoData = monoBuffer.data();
    int samplesProcessed = 0;
    while (samplesProcessed < numSamples) {
        const int samplesToFill = juce::jmin(FFT_SIZE - static_cast<int>(m_fftQueueIndex), numSamples - samplesProcessed);

        std::memcpy(&m_fftQueue[m_fftQueueIndex], &monoData[samplesProcessed], static_cast<size_t>(samplesToFill) * sizeof(float));

        m_fftQueueIndex += static_cast<unsigned int>(samplesToFill);
        samplesProcessed += samplesToFill;

        if (m_fftQueueIndex == FFT_SIZE) {
            if (!m_isNextFftBlockReady.load(std::memory_order_relaxed)) {
                std::memcpy(m_fftData, m_fftQueue, sizeof(m_fftQueue));
                m_isNextFftBlockReady.store(true, std::memory_order_relaxed);
            }
            m_fftQueueIndex = 0;
        }
    }
}

void Analyzer::ProcessFFT() noexcept
{
    if (m_isNextFftBlockReady.load(std::memory_order_relaxed)) {
        m_window.multiplyWithWindowingTable(m_fftData, FFT_SIZE);
        m_forwardFft.performFrequencyOnlyForwardTransform(m_fftData);

        constexpr float smoothingFactor = 0.3f;
        for (int i = 0; i < FFT_OUTPUT_SIZE; i++) {
            m_fftOutputData[i] = m_fftOutputData[i] * (1.0f - smoothingFactor) + m_fftData[i] * smoothingFactor;
        }

        m_isNextFftBlockReady.store(false, std::memory_order_relaxed);
    } else {
        // Gentle decay if data is NOT ready
        for (float& i : m_fftOutputData) {
            i *= 0.95f;
        }
    }
}
