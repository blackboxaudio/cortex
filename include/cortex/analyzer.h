#pragma once

#include <juce_dsp/juce_dsp.h>

#include <atomic>

namespace cortex {
    class Analyzer {
    public:
        Analyzer();
        ~Analyzer() = default;

        void ProcessAudioBuffer(const juce::AudioBuffer<float>& buffer) noexcept;

        const float* GetFFTOutputData() const noexcept { return m_fftOutputData; }

        static constexpr int GetFFTSize() noexcept { return FFT_SIZE; }
        static constexpr int GetFFTOutputSize() noexcept { return FFT_OUTPUT_SIZE; }

    private:
        void LoadFFTQueue(const juce::AudioBuffer<float>& buffer) noexcept;
        void ProcessFFT() noexcept;

        static constexpr int FFT_ORDER = 11;
        static constexpr int FFT_SIZE = 1 << FFT_ORDER;
        static constexpr int FFT_OUTPUT_SIZE = FFT_SIZE / 2;

        juce::dsp::FFT m_forwardFft { FFT_ORDER };
        juce::dsp::WindowingFunction<float> m_window { FFT_SIZE, juce::dsp::WindowingFunction<float>::hann };

        float m_fftQueue[FFT_SIZE] = {};
        float m_fftData[2 * FFT_SIZE] = {};
        float m_fftOutputData[FFT_OUTPUT_SIZE] = {};

        unsigned int m_fftQueueIndex = 0;
        std::atomic<bool> m_isNextFftBlockReady { false };

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Analyzer)
    };
}
