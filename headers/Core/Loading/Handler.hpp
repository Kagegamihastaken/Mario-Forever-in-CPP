#ifndef HANDLER_HPP
#define HANDLER_HPP

namespace MFCPP {
    class Handler {
    public:
        Handler() = delete;
        Handler(const Handler&) = delete;
        Handler(Handler&&) = delete;
        Handler& operator=(const Handler&) = delete;
        Handler& operator=(Handler&&) = delete;

        static void HandlerInit();
        static void SIGHandler(int signum);
    };
}
#endif //HANDLER_HPP
