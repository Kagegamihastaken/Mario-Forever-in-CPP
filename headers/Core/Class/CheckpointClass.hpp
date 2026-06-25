#pragma once

#ifndef CHECKPOINTCLASS_HPP
#define CHECKPOINTCLASS_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "../Object/ActiveObject.hpp"
#include "SFML/Graphics/Rect.hpp"

namespace MFCPP {
    class Checkpoint final : public SingleAnimationObject {
    public:
        explicit Checkpoint(const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
        ~Checkpoint() = default;
        void setTouch(bool val);
        [[nodiscard]] bool getTouch() const;
        void setLastState(int val);
        [[nodiscard]] int getLastState() const;
        [[nodiscard]] sf::FloatRect getHitbox() const;
        [[nodiscard]] sf::Vector2f getInterpolatedPosition(float alpha);
        [[nodiscard]] sf::Vector2f getOrigin();
        [[nodiscard]] sf::Vector2f getPosition();
    private:
        bool          m_isTouch{};
        int           m_lastState{};
        ActiveObject<float> m_transform;
        sf::FloatRect m_hitbox;
    };
}

#endif