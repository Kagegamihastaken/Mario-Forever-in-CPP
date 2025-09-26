#pragma once

#ifndef BULLETBILLEFFECTCLASS_HPP
#define BULLETBILLEFFECTCLASS_HPP
#include "ActiveObjectClass.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "SFML/System/Vector2.hpp"

namespace MFCPP {
    class BulletBillEffect final : public ActiveObject<float>, public StaticAnimationObject {
    public:
        explicit BulletBillEffect(float Yvelo, float Xvelo, const sf::Vector2f& pos, const sf::Vector2f& origin);
        ~BulletBillEffect() = default;
        void setYVelo(float val);
        [[nodiscard]] float getYVelo() const;
        void setXVelo(float val);
        [[nodiscard]] float getXVelo() const;
    private:
        float m_Yvelo;
        float m_Xvelo;
    };
}

#endif //BULLETBILLEFFECTCLASS_HPP
