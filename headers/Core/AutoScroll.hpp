#ifndef MFCPP_AUTOSCROLL_HPP
#define MFCPP_AUTOSCROLL_HPP
#include "Class/ActiveObjectClass.hpp"

namespace MFCPP {
    class AutoScroll final {
    public:
        static void UpdateSpeed(float deltaTime);
        static void resetPosition();
        [[nodiscard]] static sf::Vector2f getPosition();
        static void setOriginPosition(const sf::Vector2f& pos);
        static void setAutoScrollMode(bool val);
        [[nodiscard]] static bool getAutoScrollMode();
        [[nodiscard]] static float getSpeed();
        [[nodiscard]] static bool getTankMode();
        [[nodiscard]] static bool getTankAnimationStop();
        static void AutoScrollBuild();
        static void AutoScrollInit();
    private:
        static sf::Vector2f m_position;
        static sf::Vector2f m_origin_pos;
        static bool m_tankAnimationStop;
        static float m_speed;
        static bool m_autoScrollMode;
        static bool m_tankMode;
    };
}

#endif //MFCPP_AUTOSCROLL_HPP
