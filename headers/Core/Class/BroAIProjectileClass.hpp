#ifndef BROAIPROJECTILECLASS_HPP
#define BROAIPROJECTILECLASS_HPP

#include "Core/Class/ActiveObjectClass.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Loading/enum.hpp"

namespace MFCPP {
    class BroAIProjectile final : public StaticAnimationObject, public ActiveObject<float> {
    public:
        ~BroAIProjectile() = default;
        explicit BroAIProjectile(bool direction, BroAIProjectileType type, BroAIProjectileBehavior behaviour, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
        void setYVelo(float val);
        [[nodiscard]] float getYVelo() const;
        void setXVelo(float val);
        [[nodiscard]] float getXVelo() const;
        [[nodiscard]] bool getDirection() const;
        void setType(BroAIProjectileType val);
        [[nodiscard]] BroAIProjectileType getType() const;
        void setBehaviour(BroAIProjectileBehavior val);
        [[nodiscard]] BroAIProjectileBehavior getBehaviour() const;
        void setHitbox(const sf::FloatRect& hitbox);
        [[nodiscard]] sf::FloatRect getHitbox() const;
    private:
        float                   m_Xvelo;
        float                   m_Yvelo;
        bool                    m_direction;
        BroAIProjectileType     m_type;
        BroAIProjectileBehavior m_behaviour;
        sf::FloatRect           m_hitbox;
    };
}

#endif //BROAIPROJECTILECLASS_HPP
