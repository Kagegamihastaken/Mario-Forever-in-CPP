#pragma once

#ifndef BRICKPARTICLECLASS_HPP
#define BRICKPARTICLECLASS_HPP
#include "ActiveObjectClass.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "SFML/System/Vector2.hpp"

namespace MFCPP {
    class BrickParticle final : public ActiveObject<float>, public StaticAnimationObject {
    public:
        explicit BrickParticle(float Xvelo, float Yvelo, const sf::Vector2f& pos, const sf::Vector2f& origin);
        ~BrickParticle() = default;
        void setXvelo(float val);
        void setYvelo(float val);
        [[nodiscard]] float getXvelo() const;
        [[nodiscard]] float getYvelo() const;
    private:
        float m_Xvelo;
        float m_Yvelo;
    };
}

#endif //BRICKPARTICLECLASS_HPP
