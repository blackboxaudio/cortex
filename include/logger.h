#pragma once

#include <juce_core/juce_core.h>

#if CTX_ENABLE_LOGGING

void LogToFile(const juce::String& message, const juce::String& level);

#define BBX_DEBUG(msg) LogToFile(msg, "DEBUG")
#define BBX_INFO(msg) LogToFile(msg, "INFO")
#define BBX_WARN(msg) LogToFile(msg, "WARN")
#define BBX_ERROR(msg) LogToFile(msg, "ERROR")

#else

#define InitializeLogger(pluginName) ((void)0)

#define BBX_DEBUG(msg) ((void)0)
#define BBX_INFO(msg) ((void)0)
#define BBX_WARN(msg) ((void)0)
#define BBX_ERROR(msg) ((void)0)

#endif
