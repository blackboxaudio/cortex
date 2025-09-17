#include "logger.h"

#ifdef CTX_ENABLE_LOGGING
void LogToFile(const juce::String& message, const juce::String& level)
{
    static juce::CriticalSection logMutex;
    static juce::File logFile;
    static bool initialized = false;

    juce::ScopedLock lock(logMutex);

    if (!initialized) {
        auto logDir = juce::File::getSpecialLocation(juce::File::userDocumentsDirectory)
                          .getChildFile("Black Box Audio")
                          .getChildFile("Logs");

        if (!logDir.exists()) {
            logDir.createDirectory();
        }

        logFile = logDir.getChildFile(juce::Time::getCurrentTime().formatted("%Y%m%d_%H%M%S") + ".txt");

        initialized = true;
    }

    auto now = juce::Time::getCurrentTime();
    auto timestamp = now.formatted("%H:%M:%S.") + juce::String(now.getMilliseconds());

    auto threadId = juce::String::toHexString(
        static_cast<std::uintptr_t>(
            reinterpret_cast<uintptr_t>(juce::Thread::getCurrentThreadId())));

    auto logEntry = juce::String::formatted("[%s] [%s] [T:%s] %s\n",
        timestamp.toRawUTF8(),
        level.toRawUTF8(),
        threadId.toRawUTF8(),
        message.toRawUTF8());

    if (!logFile.appendText(logEntry)) {
        DBG("LogToFile failed: " + message);
    }
}
#endif
