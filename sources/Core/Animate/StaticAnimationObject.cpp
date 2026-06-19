#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/ImageManager.hpp"
#include "Core/WindowFrame.hpp"

namespace MFCPP {
    void StaticAnimationObject::setTexture(const std::string& name, bool anim_flip) {
        m_anim_flip = anim_flip;
        m_name = name;
    }
    void StaticAnimationObject::animationUpdate(const sf::Vector2f& pos, const sf::Vector2f& origin) {
        const sf::IntRect rect = ImageManager::getSpritePreCompute(m_name).getTextureRect();
        ImageManager::getSpritePreCompute(m_name).setPosition(pos);
        ImageManager::getSpritePreCompute(m_name).setOrigin(origin);
        if (!m_anim_flip)
            ImageManager::getSpritePreCompute(m_name).setTextureRect(sf::IntRect({0, rect.position.y}, {std::abs(rect.size.x), rect.size.y}));
        else
            ImageManager::getSpritePreCompute(m_name).setTextureRect(sf::IntRect({1 * std::abs(rect.size.x), rect.position.y}, {-std::abs(rect.size.x), rect.size.y}));
    }
    void StaticAnimationObject::setColor(const sf::Color& color) {
        ImageManager::getSpritePreCompute(m_name).setColor(color);
    }
    void StaticAnimationObject::setRotation(const sf::Angle angle) {
        ImageManager::getSpritePreCompute(m_name).setRotation(angle);
    }
    void StaticAnimationObject::animationDraw() const {
        WindowFrame::getWindow().draw(ImageManager::getSpritePreCompute(m_name));
    }
    sf::Angle StaticAnimationObject::getRotation() const {
        return ImageManager::getSpritePreCompute(m_name).getRotation();
    }
    void StaticAnimationObject::setAnimationDirection(const bool val) {
        m_anim_flip = val;
    }
    bool StaticAnimationObject::getAnimationDirection() const {
        return m_anim_flip;
    }
}
