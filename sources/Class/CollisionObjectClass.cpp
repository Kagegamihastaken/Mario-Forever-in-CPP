#include "Class/CollisionObjectClass.hpp"

namespace MFCPP {
    void CollisionObject::SetOrigin(const sf::Vector2f &origin) {
        m_origin = origin;
    }
    sf::Vector2f CollisionObject::GetOrigin() const {
        return m_origin;
    }
    sf::Vector2f CollisionObject::GetPosition() const {
        return m_pos;
    }
    void CollisionObject::SetPosition(const sf::Vector2f &pos) {
        m_pos = pos;
    }
    void CollisionObject::SetLeftHitbox(const sf::FloatRect &hitbox) {
        m_lefthitbox = hitbox;
    }
    sf::FloatRect CollisionObject::GetLeftHitbox() const {
        return m_lefthitbox;
    }
    void CollisionObject::SetRightHitbox(const sf::FloatRect &hitbox) {
        m_righthitbox = hitbox;
    }
    sf::FloatRect CollisionObject::GetRightHitbox() const {
        return m_righthitbox;
    }
}



