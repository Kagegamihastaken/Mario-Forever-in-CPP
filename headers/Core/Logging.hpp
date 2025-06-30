#ifndef LOGGING_HPP
#define LOGGING_HPP
#include <fmt/os.h>

namespace MFCPP {
    namespace Log {
        extern void ExceptionPrint(const std::exception* exp);
        extern void ErrorColorPrint(std::string_view str);
        extern void InfoPrint(std::string_view str);
        extern void WarningPrint(std::string_view str);
        extern void ErrorPrint(std::string_view str);

        void printSigTrace();
        void printCurrentTrace();
        extern void FatalPrint(std::string_view str);
    }
}


#endif //LOGGING_HPP
