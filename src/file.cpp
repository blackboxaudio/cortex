#include "file.h"

const char* cortex::GetMimeTypeFromExtension(const juce::String& extension)
{
    static const std::unordered_map<juce::String, const char*> mimeMap = {
        { { "js" }, "application/javascript" },
        { { "json" }, "application/json" },
        { { "map" }, "application/json" },
        { { "woff2" }, "font/woff2" },
        { { "jpeg" }, "image/jpeg" },
        { { "jpg" }, "image/jpeg" },
        { { "png" }, "image/png" },
        { { "svg" }, "image/svg+xml" },
        { { "ico" }, "image/vnd.microsoft.icon" },
        { { "css" }, "text/css" },
        { { "htm" }, "text/html" },
        { { "html" }, "text/html" },
        { { "txt" }, "text/plain" },
    };

    if (const auto mimeType = mimeMap.find(extension.toLowerCase()); mimeType != mimeMap.end()) {
        return mimeType->second;
    } else {
        jassertfalse;
        return "";
    }
}
