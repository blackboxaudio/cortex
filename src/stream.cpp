#include "cortex/stream.h"

std::vector<std::byte> cortex::StreamToVector(juce::InputStream& stream)
{
    const auto sizeInBytes = static_cast<size_t>(stream.getTotalLength());
    std::vector<std::byte> result(sizeInBytes);

    stream.setPosition(0);
    [[maybe_unused]] const auto bytesRead = stream.read(result.data(), result.size());

#if JUCE_WINDOWS
    /**
     * NOTE: No assertion b/c ssize_t is not available on Windows.
     */
#else
    jassert(bytesRead == static_cast<ssize_t>(sizeInBytes));
#endif
    return result;
}
