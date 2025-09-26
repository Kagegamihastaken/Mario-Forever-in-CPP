#ifndef PIRANHAAIPROJECTILECLASS_HPP
#define PIRANHAAIPROJECTILECLASS_HPP

#ifndef BROAIPROJECTILECLASS_HPP
#define BROAIPROJECTILECLASS_HPP

#include "Core/Class/ActiveObjectClass.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Loading/enum.hpp"
#include <SFML/Graphics/Rect.hpp>

namespace MFCPP {
    class PiranhaAIProjectile final : public StaticAnimationObject, public ActiveObject<float> {
    public:
        ~PiranhaAIProjectile() = default;
        explicit PiranhaAIProjectile(bool direction, PiranhaProjectileType type, PiranhaDirection piranha_dir, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
        void setYVelo(float val);
        [[nodiscard]] float getYVelo() const;
        void setXVelo(float val);
        [[nodiscard]] float getXVelo() const;
        [[nodiscard]] bool getDirection() const;
        void setType(PiranhaProjectileType val);
        [[nodiscard]] PiranhaProjectileType getType() const;
        void setHitbox(const sf::FloatRect& hitbox);
        [[nodiscard]] sf::FloatRect getHitbox() const;
        [[nodiscard]] PiranhaDirection getPiranhaDirection() const;
    private:
        float                   m_Xvelo;
        float                   m_Yvelo;
        bool                    m_direction;
        PiranhaProjectileType   m_type;
        sf::FloatRect           m_hitbox;
        PiranhaDirection        m_piranha_dir;
    };
}

#endif //BROAIPROJECTILECLASS_HPP


#endif //PIRANHAAIPROJECTILECLASS_HPP
