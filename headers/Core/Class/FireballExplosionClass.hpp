#ifndef FIREBALLEXPLOSIONCLASS_HPP
#define FIREBALLEXPLOSIONCLASS_HPP

#include "Core/Class/ActiveObjectClass.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"

namespace MFCPP {
    class FireballExplosion : public ActiveObject, public SingleAnimationObject {
    public:
        FireballExplosion(const sf::Vector2f& pos, const sf::Vector2f& origin);
    };
}

#endif //FIREBALLEXPLOSIONCLASS_HPP
