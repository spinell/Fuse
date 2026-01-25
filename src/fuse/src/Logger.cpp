#include <fuse/Logger.h>

#include <print>
#include <utility>

namespace {

std::string_view logLevelAsString(fuse::LogLevel level) {
    using enum fuse::LogLevel;
    switch (level) {
            // clang-format off
        case Fatal:   return "Fatal";
        case Error:   return "Error";
        case Warn:    return "Warn";
        case Info:    return "Info";
        case Debug:   return "Debug";
        case Verbose: return "Verbose";
        default: std::unreachable();
            // clang-format on
    }
}

} // namespace


namespace fuse {

void log_initialize() {}

void log_shutdown() {}

void log_message(LogLevel level, std::string_view msg) {
    std::println("[{}] {}", logLevelAsString(level), msg);

    if (level == LogLevel::Fatal) {
        std::abort();
    }
}

} // namespace fuse
