#pragma once

// TODO: Improve Assertion
// - Assertion level
// - Assertion category
// - Custom handler (Break/Ignore/...)
// - Add more info about the context (expression decomposition / formatted string with variable)

// TODO: enable/disable from build system
#define FUSE_ASSERTIONS_ENABLE 1

/// @def FUSE_DEBUG_BREAK()
/// @brief Trigger a breakpoint when debugger is attached.
// For GCC use SIGTRAP instead of __builtin_trap which allow to continue debuging.
// https://github.com/google/googletest/issues/4786
#if defined(__clang__)
#    define FUSE_DEBUG_BREAK() __builtin_debugtrap()
#elif defined(_MSC_VER)
#    define FUSE_DEBUG_BREAK() __debugbreak()
#elif defined(__GNUC__)
#    include <signal.h>
#    define FUSE_DEBUG_BREAK() raise(SIGTRAP);
#else
#    define FUSE_DEBUG_BREAK()
#    warning "FUSE_DEBUG_BREAK not set."
#endif


namespace fuse {

/// @brief Reports an assertion failure.
///        Note that this is not the assertion itself, just a reporting of an assertion failure
///        that has already occurred.
/// @param expr The expression to be reported.
/// @param msg A custom message to be reported, if provided.
/// @param file The path and name of the file containing the expression.
/// @param line The line number in the file where the assertion failure occurred.
void report_assertion_failure(const char* expr, const char* msg, const char* file, int line);

} // namespace fuse


#if FUSE_ASSERTIONS_ENABLE

#    define FUSE_ASSERT(expr) \
        (expr)                \
          ? void(0)           \
          : (fuse::report_assertion_failure(#expr, nullptr, __FILE__, __LINE__), FUSE_DEBUG_BREAK())

#    define FUSE_ASSERT_MSG(expr, msg) \
        (expr) ? void(0)               \
               : (fuse::report_assertion_failure(#expr, msg, __FILE__, __LINE__), FUSE_DEBUG_BREAK())

#else
#    define FUSE_ASSERT(expr)
#    define FUSE_ASSERT_MSG(expr)
#endif
