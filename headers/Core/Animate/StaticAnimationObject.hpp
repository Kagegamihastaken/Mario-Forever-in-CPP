#ifndef STATICANIMATIONOBJECT_HPP
#define STATICANIMATIONOBJECT_HPP

#include "Core/SimpleSprite.hpp"
#include <SFML/Graphics/RenderWindow.hpp>

namespace MFCPP {
    class StaticAnimationObject {
        SimpleSprite m_index;
    public:
        void setTexture(const std::string& name);
        void AnimationUpdate(const sf::Vector2f& pos, const sf::Vector2f& origin);
        void AnimationDraw(sf::RenderWindow& window) const;
        void setColor(const sf::Color& color);
    };
}

#endif //STATICANIMATIONOBJECT_HPP
