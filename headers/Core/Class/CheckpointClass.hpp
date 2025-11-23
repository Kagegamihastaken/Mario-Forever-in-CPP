#pragma once

#ifndef CHECKPOINTCLASS_HPP
#define CHECKPOINTCLASS_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Class/ActiveObjectClass.hpp"
#include "SFML/Graphics/Rect.hpp"

namespace MFCPP {
    class Checkpoint final : public ActiveObject<float>, public SingleAnimationObject {
    public:
        explicit Checkpoint(const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
        ~Checkpoint() = default;
        void setTouch(bool val);
        [[nodiscard]] bool getTouch() const;
        void setHitbox(const sf::FloatRect& val);
        [[nodiscard]] sf::FloatRect getHitbox() const;
        void setLastState(int val);
        [[nodiscard]] int getLastState() const;
    private:
        bool          m_isTouch{};
        sf::FloatRect m_hitbox{};
        int           m_lastState{};
    };
}

#endif