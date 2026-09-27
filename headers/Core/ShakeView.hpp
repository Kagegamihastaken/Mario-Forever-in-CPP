#ifndef MFCPP_SHAKEVIEW_HPP
#define MFCPP_SHAKEVIEW_HPP
#include "SFML/System/Vector2.hpp"

namespace MFCPP {
    namespace ShakeView {
        static float intensity = 0.f;
        void shake(float val);
        void update(float deltaTime);
        [[nodiscard]] sf::Vector2f getOffset();
    }
}

#endif //MFCPP_SHAKEVIEW_HPP
