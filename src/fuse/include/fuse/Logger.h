#pragma once

#include <string_view>

#include <format>
#include <string>

namespace fuse {

/// @brief
enum class LogLevel {
    /// @brief Fatal error, should be used to stop the application when hit.
    Fatal = 0,
    /// @brief  indicate critical runtime problems that cause the application to run improperly.
    Error = 1,
    /// @brief  Indicate non-critical problems with the application that cause it to run suboptimal.
    Warn = 2,
    /// @brief Should be used for non-error informational purposes.
    Info = 3,
    /// @brief  Should be used for debugging purposes.
    Debug = 4,
    /// @brief  Should be used for verbose debugging purposes.
    Verbose = 5
};

/// @brief Initialize logging system
void log_initialize();

/// @brief Shutdown logging system
void log_shutdown();

/// @brief Log a message.
/// @param level The log level.
/// @param msg The message to log.
void log_message(LogLevel level, std::string_view msg);

/// @brief Log a compile time formatted message.
/// @param level The log level.
/// @param msg The message to log.
/// @param args The arguments toinsert in the formatted message.
template <typename... Args>
void log_message(LogLevel level, const std::format_string<Args...>& fmt, Args&&... args) {
    const std::string msg = std::vformat(fmt.get(), std::make_format_args(args...));
    log_message(level, msg);
}

} // namespace fuse

// TODO: Enable/Disable from the build system
#define FUSE_FATAL(...) fuse::log_message(fuse::LogLevel::Fatal, __VA_ARGS__)
#define FUSE_ERROR(...) fuse::log_message(fuse::LogLevel::Error, __VA_ARGS__)
#define FUSE_WARN(...) fuse::log_message(fuse::LogLevel::Warn, __VA_ARGS__)
#define FUSE_INFO(...) fuse::log_message(fuse::LogLevel::Info, __VA_ARGS__)
#define FUSE_DEBUG(...) fuse::log_message(fuse::LogLevel::Debug, __VA_ARGS__)
#define FUSE_VERBOSE(...) fuse::log_message(fuse::LogLevel::Verbose, __VA_ARGS__)
