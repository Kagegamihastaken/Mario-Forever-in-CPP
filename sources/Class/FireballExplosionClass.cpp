#include "Class/FireballExplosionClass.hpp"

namespace MFCPP {
    FireballExplosion::FireballExplosion(const sf::Vector2f &pos, const sf::Vector2f &origin) {
        setCurrentPosition(pos);
        setInterpolatedPosition(pos);
        setPreviousPosition(pos);
        setOrigin(origin);
    }

}