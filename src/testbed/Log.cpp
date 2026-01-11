#include "Log.h"

#include <print>
#include <utility>

namespace {
using namespace fuse;

std::string_view logLevelAsString(LogLevel level) {
    switch (level) {
            // clang-format off
        case LogLevel::Debug:   return "Debug";
        case LogLevel::Error:   return "Error";
        case LogLevel::Trace:   return "Trace";
        case LogLevel::Info:    return "Info";
        case LogLevel::Warn:    return "Warn";
        case LogLevel::Fatal:   return "Fatal";
        case LogLevel::Verbose: return "Verbose";
        default: std::unreachable();
            // clang-format on
    }
}

} // namespace


namespace fuse {

void logInitialize() {}
void logShutdown() {}

void logMessage(LogLevel level, std::string msg) {
    std::println("[{}] {}", logLevelAsString(level), msg);
    if(level == LogLevel::Fatal) {
        std::abort();
    }
}

} // namespace fuse
