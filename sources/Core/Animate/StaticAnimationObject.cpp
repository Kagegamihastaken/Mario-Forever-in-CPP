#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/ImageManager.hpp"
#include "Core/WindowFrame.hpp"

namespace MFCPP {
    void StaticAnimationObject::setTexture(const std::string& name, bool anim_flip) {
        m_anim_flip = anim_flip;
        m_precompute_cache = &ImageManager::getSpritePreCompute(name);
    }
    void StaticAnimationObject::animationUpdate(const sf::Vector2f& pos, const sf::Vector2f& origin) {
        if (!m_precompute_cache) throw std::bad_alloc();
        const sf::IntRect rect = m_precompute_cache->getTextureRect();
        m_precompute_cache->setPosition(pos);
        m_precompute_cache->setOrigin(origin);
        if (!m_anim_flip)
            m_precompute_cache->setTextureRect(sf::IntRect({0, rect.position.y}, {std::abs(rect.size.x), rect.size.y}));
        else
            m_precompute_cache->setTextureRect(sf::IntRect({1 * std::abs(rect.size.x), rect.position.y}, {-std::abs(rect.size.x), rect.size.y}));
    }
    void StaticAnimationObject::setColor(const sf::Color& color) {
        if (!m_precompute_cache) throw std::bad_alloc();
        m_precompute_cache->setColor(color);
    }
    void StaticAnimationObject::setRotation(const sf::Angle angle) {
        if (!m_precompute_cache) throw std::bad_alloc();
        m_precompute_cache->setRotation(angle);
    }
    void StaticAnimationObject::animationDraw() const {
        if (!m_precompute_cache) throw std::bad_alloc();
        WindowFrame::getWindow().draw(*m_precompute_cache);
    }
    sf::Angle StaticAnimationObject::getRotation() const {
        if (!m_precompute_cache) throw std::bad_alloc();
        return m_precompute_cache->getRotation();
    }
    void StaticAnimationObject::setAnimationDirection(const bool val) {
        m_anim_flip = val;
    }
    bool StaticAnimationObject::getAnimationDirection() const {
        return m_anim_flip;
    }
}
