#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP
#include <exception>
#include <string>

namespace MFCPP {
    namespace Exception {
        class AlreadyExistElement final : public std::exception {
        private:
            std::string messange;
        public:
            explicit AlreadyExistElement(const std::string_view msg) : messange(msg) {};
            const char* what() const noexcept override {
                return messange.c_str();
            }
        };

        class NonExistElement final : public std::exception {
        private:
            std::string messange;
        public:
            explicit NonExistElement(const std::string_view msg) : messange(msg) {};
            const char* what() const noexcept override {
                return messange.c_str();
            }
        };
    }
}

#endif //EXCEPTION_HPP
