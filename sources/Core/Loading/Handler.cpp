#include "Core/Loading/Handler.hpp"

#include <csignal>

#include "Core/Logging.hpp"
#include <fmt/format.h>

#include "cpptrace/utils.hpp"

namespace MFCPP {
    void SIGHandler (const int signum) {
        //cpptrace::generate_trace().print();
        switch (signum) {
            case SIGFPE:
                MFCPP::Log::FatalPrint("Floating-point Exception");
                break;
            case SIGILL:
                MFCPP::Log::FatalPrint("Illegal instruction Exception");
                break;
            case SIGSEGV:
                MFCPP::Log::FatalPrint("Invalid memory access (Segmentation Fault)");
                break;
            case SIGINT:
                MFCPP::Log::FatalPrint("External Interrupted");
                break;
            case SIGTERM:
                MFCPP::Log::FatalPrint("Termination Request");
                break;
            case SIGABRT:
                MFCPP::Log::FatalPrint("Abort Request");
                break;
            default:
                MFCPP::Log::FatalPrint(fmt::format("Signal Code {} received.", signum));
                break;
        }
        MFCPP::Log::printSigTrace();
    }
    void HandlerInit() {
        cpptrace::register_terminate_handler();
        std::signal(SIGSEGV, SIGHandler);
        std::signal(SIGFPE, SIGHandler);
        std::signal(SIGILL, SIGHandler);
        std::signal(SIGINT, SIGHandler);
        std::signal(SIGTERM, SIGHandler);
        std::signal(SIGABRT, SIGHandler);
    }
}
