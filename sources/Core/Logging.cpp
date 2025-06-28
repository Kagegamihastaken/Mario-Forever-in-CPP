#include "Core/Logging.hpp"

#include <source_location>

namespace MFCPP {
    namespace Log {
        void ExceptionPrint(const std::exception* exp) {
            //fmt::print(fmt::fg(fmt::color::orange_red) | fmt::emphasis::bold, "[FATAL] {}\n", exp->what());
            const std::source_location location = std::source_location::current();
            fmt::print(fmt::fg(fmt::color::orange_red) | fmt::emphasis::bold, "[FATAL] Exception {} {}\n", typeid(*exp).name(),exp->what());
        }
        void ErrorColorPrint(const std::string_view& str) {
            fmt::print(fmt::fg(fmt::color::salmon), "{}\n", str);
        }
        void ErrorPrint(const std::string_view& str) {
            fmt::print(fmt::fg(fmt::color::salmon), "[ERROR] {}\n", str);
        }
        void InfoPrint(const std::string_view& str) {
            fmt::print(fmt::fg(fmt::color::dim_gray) | fmt::emphasis::italic, "[INFO] {}\n", str);
        }
        void WarningPrint(const std::string_view& str) {
            fmt::print(fmt::fg(fmt::color::gold), "[WARNING] {}\n", str);
        }
    }
}
