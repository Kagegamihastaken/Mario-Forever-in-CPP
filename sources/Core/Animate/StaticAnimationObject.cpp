#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/ImageManager.hpp"
namespace MFCPP {
    void StaticAnimationObject::setTexture(const std::string& name, bool anim_flip) {
        m_index.setTexture(ImageManager::GetReturnTexture(name), true);
        m_anim_flip = anim_flip;
        m_name = name;
    }
    void StaticAnimationObject::AnimationUpdate(const sf::Vector2f& pos, const sf::Vector2f& origin) {
        const sf::IntRect rect = m_index.getTextureRect();
        m_index.setPosition(pos);
        m_index.setOrigin(origin);
        if (!m_anim_flip)
            m_index.setTextureRect(sf::IntRect({0, rect.position.y}, {std::abs(rect.size.x), rect.size.y}));
        else
            m_index.setTextureRect(sf::IntRect({1 * std::abs(rect.size.x), rect.position.y}, {-std::abs(rect.size.x), rect.size.y}));
    }
    void StaticAnimationObject::setColor(const sf::Color& color) {
        m_index.setColor(color);
    }
    void StaticAnimationObject::setRotation(const sf::Angle angle) {
        m_index.setRotation(angle);
    }
    void StaticAnimationObject::AnimationDraw() const {
        ImageManager::AddToVertex(m_name, m_index.getTextureRect(), m_index.getTransform());
        //window.draw(m_index);
    }
    sf::Angle StaticAnimationObject::getRotation() const {
        return m_index.getRotation();
    }
    void StaticAnimationObject::setAnimationDirection(const bool val) {
        m_anim_flip = val;
    }
    bool StaticAnimationObject::getAnimationDirection() const {
        return m_anim_flip;
    }
}