#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

/*
#include <stdexcept>
#include <boost/stacktrace/stacktrace.hpp>
#include <fmt/format.h>

namespace MFCPP {
    namespace Log {
        class traced_exception : public std::runtime_error {
        public:
            // Constructor that takes a message and captures the stack
            explicit traced_exception(const std::string& message)
                : std::runtime_error(message), trace_(boost::stacktrace::stacktrace()) {}

            // An override of what() to include the stack trace information
            const char* what() const noexcept override {
                // Use a thread_local buffer to safely store the formatted string
                // for the lifetime of the what() call.
                thread_local static std::string buffer;

                buffer = std::runtime_error::what();

                if (!trace_.empty()) {
                    buffer += "\n\n--- Stack Trace (Boost) ---\n";
                    // boost::stacktrace can be streamed directly.
                    // fmt::format can handle this when <fmt/ostream.h> is included.
                    buffer += fmt::format("{}", trace_);
                }

                return buffer.c_str();
            }

        private:
            boost::stacktrace::stacktrace trace_;
        };
    }
}
*/
#endif //EXCEPTION_HPP
