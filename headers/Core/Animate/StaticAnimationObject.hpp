#pragma once
#ifndef STATICANIMATIONOBJECT_HPP
#define STATICANIMATIONOBJECT_HPP

#include "Core/SimpleSprite.hpp"

namespace MFCPP {
    class StaticAnimationObject {
        bool m_anim_flip = false;
        //cache
        SimpleSprite* m_precompute_cache = nullptr;
    public:
        void setTexture(std::string_view name, bool anim_flip = false);
        void animationUpdate(const sf::Vector2f& pos, const sf::Vector2f& origin);
        void animationDraw() const;
        void setColor(const sf::Color& color);
        void setRotation(sf::Angle angle);
        void setAnimationDirection(bool val);
        [[nodiscard]] bool getAnimationDirection() const;
        [[nodiscard]] sf::Angle getRotation() const;
    };
}

#endif //STATICANIMATIONOBJECT_HPP
