#pragma once
#ifndef STATICANIMATIONOBJECT_HPP
#define STATICANIMATIONOBJECT_HPP

#include "Core/SimpleSprite.hpp"

namespace MFCPP {
    class StaticAnimationObject {
        SimpleSprite m_index;
        bool m_anim_flip = false;
        std::string m_name;
    public:
        void setTexture(const std::string& name, bool anim_flip = false);
        void AnimationUpdate(const sf::Vector2f& pos, const sf::Vector2f& origin);
        void AnimationDraw() const;
        void setColor(const sf::Color& color);
        void setRotation(sf::Angle angle);
        void setAnimationDirection(bool val);
        [[nodiscard]] bool getAnimationDirection() const;
        [[nodiscard]] sf::Angle getRotation() const;
    };
}

#endif //STATICANIMATIONOBJECT_HPP
