#include <fuse/Assert.h>

#include <iostream>
#include <print>
#include <stacktrace>

void fuse::report_assertion_failure(const char* expr, const char* msg, const char* file, int line) {
    if (msg) {
        std::println("Assertion Failure: '{}', {}, {}:{}", expr, msg, file, line);
    } else {
        std::println("Assertion Failure: '{}',  {}:{}", expr, file, line);
    }

    // skip 1 entry, this function
    std::println("Stackstrace: \n\n{}", std::stacktrace::current(1));
}
