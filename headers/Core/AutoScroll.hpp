#ifndef MFCPP_AUTOSCROLL_HPP
#define MFCPP_AUTOSCROLL_HPP
#include "Object/ActiveObject.hpp"

namespace MFCPP {
    namespace AutoScroll {
        void UpdateSpeed(float deltaTime);
        void resetPosition();
        [[nodiscard]] sf::Vector2f getPosition();
        void setOriginPosition(const sf::Vector2f& pos);
        void setAutoScrollMode(bool val);
        void setTankMode(bool val);
        void setAutoScrollSpeed(float val);
        [[nodiscard]] bool getAutoScrollMode();
        [[nodiscard]] float getSpeed();
        [[nodiscard]] bool getTankMode();
        [[nodiscard]] bool getTankAnimationStop();
        void AutoScrollBuild();
        void AutoScrollInit();
    };
}

#endif //MFCPP_AUTOSCROLL_HPP
