#ifndef MARIOPROJECTILECLASS_HPP
#define MARIOPROJECTILECLASS_HPP

#include "Class/ActiveObjectClass.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Loading/enum.hpp"

namespace MFCPP {
    class MarioProjectile final : public StaticAnimationObject, public ActiveObject {
    public:
        ~MarioProjectile() = default;
        MarioProjectile(bool direction, MarioProjectileType type, MarioProjectileBehavior behaviour, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
        void setYVelo(float val);
        [[nodiscard]] float getYVelo() const;
        void setXVelo(float val);
        [[nodiscard]] float getXVelo() const;
        [[nodiscard]] bool getDirection() const;
        void setType(MarioProjectileType val);
        [[nodiscard]] MarioProjectileType getType() const;
        void setBehaviour(MarioProjectileBehavior val);
        [[nodiscard]] MarioProjectileBehavior getBehaviour() const;
        void setHitbox(const sf::FloatRect& hitbox);
        [[nodiscard]] sf::FloatRect getHitbox() const;
        void willDestroy(bool val);
        [[nodiscard]] bool willBeDestroyed() const;
    private:
        float                   m_Xvelo;
        float                   m_Yvelo;
        bool                    m_direction;
        MarioProjectileType     m_type;
        MarioProjectileBehavior m_behaviour;
        sf::FloatRect           m_hitbox;
        bool                    m_destroy;
    };
}

#endif //MARIOPROJECTILECLASS_HPP
