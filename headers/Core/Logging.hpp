#ifndef LOGGING_HPP
#define LOGGING_HPP
#include <exception>

namespace MFCPP {
    namespace Log {

        extern void ExceptionPrint(const std::exception* exp);
        extern void ErrorColorPrint(const std::string_view& str);
        extern void InfoPrint(const std::string_view& str);
        extern void WarningPrint(const std::string_view& str);
        extern void ErrorPrint(const std::string_view& str);
    }
}


#endif //LOGGING_HPP
