#pragma once

#include <juce_core/juce_core.h>

namespace cortex {
    std::vector<std::byte> StreamToVector(juce::InputStream& stream);
}
