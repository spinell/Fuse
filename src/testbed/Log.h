#pragma once
#include <format>
#include <string>

namespace fuse {

enum class LogLevel { Trace, Verbose, Debug, Info, Warn, Error, Fatal };

void logInitialize();
void logShutdown();

/// @brief
/// @param level
/// @param msg
void logMessage(LogLevel level, std::string msg);

template <typename... Args>
void logTrace(const std::format_string<Args...>& fmt, const Args&... args) {
    const std::string msg = std::vformat(fmt.get(), std::make_format_args(args...));
    logMessage(LogLevel::Trace, msg);
}

template <typename... Args>
void logVerbose(const std::format_string<Args...>& fmt, const Args&... args) {
    const std::string msg = std::vformat(fmt.get(), std::make_format_args(args...));
    logMessage(LogLevel::Verbose, msg);
}

template <typename... Args>
void logDebug(const std::format_string<Args...>& fmt, const Args&... args) {
    const std::string msg = std::vformat(fmt.get(), std::make_format_args(args...));
    logMessage(LogLevel::Debug, msg);
}

template <typename... Args>
void logInfo(const std::format_string<Args...>& fmt, const Args&... args) {
    const std::string msg = std::vformat(fmt.get(), std::make_format_args(args...));
    logMessage(LogLevel::Info, msg);
}
template <typename... Args>
void logWarn(const std::format_string<Args...>& fmt, const Args&... args) {
    const std::string msg = std::vformat(fmt.get(), std::make_format_args(args...));
    logMessage(LogLevel::Warn, msg);
}
template <typename... Args>
void logError(const std::format_string<Args...>& fmt, const Args&... args) {
    const std::string msg = std::vformat(fmt.get(), std::make_format_args(args...));
    logMessage(LogLevel::Error, msg);
}

template <typename... Args>
void logFatal(const std::format_string<Args...>& fmt, const Args&... args) {
    const std::string msg = std::vformat(fmt.get(), std::make_format_args(args...));
    logMessage(LogLevel::Fatal, msg);
}

} // namespace fuse
