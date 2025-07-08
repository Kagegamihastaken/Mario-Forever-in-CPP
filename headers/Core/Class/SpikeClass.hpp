#pragma once
#ifndef SPIKECLASS_HPP
#define SPIKECLASS_HPP
#include "ActiveObjectClass.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "SFML/Graphics/Rect.hpp"

namespace MFCPP {
    class Spike final : public ActiveObject, public SingleAnimationObject {
    public:
        Spike(SpikeID id, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
        ~Spike() = default;
        void setID(SpikeID val);
        [[nodiscard]] SpikeID getID() const;
        void setHitbox(const sf::FloatRect& hitbox);
        [[nodiscard]] sf::FloatRect getHitbox() const;
    private:
        SpikeID       m_id{};
        sf::FloatRect m_hitbox{};
    };
}

#endif //SPIKECLASS_HPP
