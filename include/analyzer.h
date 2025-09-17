#pragma once

#include <juce_dsp/juce_dsp.h>

namespace cortex {
    class Analyzer {
    public:
        Analyzer();
        ~Analyzer() = default;

        void ProcessAudioBuffer(juce::AudioBuffer<float>& buffer);

        const float* GetFFTOutputData() const { return m_fftOutputData; }

        static constexpr int GetFFTSize() { return FFT_SIZE; }
        static constexpr int GetFFTOutputSize() { return FFT_OUTPUT_SIZE; }

    private:
        void LoadFFTQueue(juce::AudioBuffer<float>& buffer);
        void ProcessFFT();

        static constexpr int FFT_ORDER = 11;
        static constexpr int FFT_SIZE = 1 << FFT_ORDER;
        static constexpr int FFT_OUTPUT_SIZE = FFT_SIZE / 2;

        juce::dsp::FFT m_forwardFft { FFT_ORDER };
        juce::dsp::WindowingFunction<float> m_window { FFT_SIZE, juce::dsp::WindowingFunction<float>::hann };

        float m_fftQueue[FFT_SIZE] = {};
        float m_fftData[2 * FFT_SIZE] = {};
        float m_fftOutputData[FFT_OUTPUT_SIZE] = {};

        unsigned int m_fftQueueIndex = 0;
        bool m_isNextFftBlockReady = false;

        JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(Analyzer)
    };
}
