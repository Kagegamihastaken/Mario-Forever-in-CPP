#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/ImageManager.hpp"
namespace MFCPP {
    void StaticAnimationObject::setTexture(const std::string& name) {
        m_index.setTexture(*ImageManager::GetReturnTexture(name), true);
    }
    void StaticAnimationObject::AnimationUpdate(const sf::Vector2f& pos, const sf::Vector2f& origin) {
        m_index.setPosition(pos);
        m_index.setOrigin(origin);
    }
    void StaticAnimationObject::setColor(const sf::Color& color) {
        m_index.setColor(color);
    }
    void StaticAnimationObject::AnimationDraw(sf::RenderWindow& window) const {
        window.draw(m_index);
    }
}