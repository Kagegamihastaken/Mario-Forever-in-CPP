#ifndef MFCPP_INPUT_HPP
#define MFCPP_INPUT_HPP

namespace MFCPP {
    namespace Input {
        static std::atomic up = false;
        static std::atomic down = false;
        static std::atomic left = false;
        static std::atomic right = false;
        static std::atomic jump = false;
        static std::atomic fire = false;
        void inputHandling();
        [[nodiscard]] std::atomic<bool>& isUpKeyPressed();
        [[nodiscard]] std::atomic<bool>& isDownKeyPressed();
        [[nodiscard]] std::atomic<bool>& isLeftKeyPressed();
        [[nodiscard]] std::atomic<bool>& isRightKeyPressed();
        [[nodiscard]] std::atomic<bool>& isJumpKeyPressed();
        [[nodiscard]] std::atomic<bool>& isFireKeyPressed();
    }
}

#endif //MFCPP_INPUT_HPP
