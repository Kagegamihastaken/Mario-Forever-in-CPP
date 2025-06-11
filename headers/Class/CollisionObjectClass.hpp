#ifndef COLLISIONOBJECT_HPP
#define COLLISIONOBJECT_HPP

#include <SFML/Graphics.hpp>
namespace MFCPP {
    class CollisionObject {
    public:
        CollisionObject(const sf::Vector2f& pos, const sf::Vector2f& origin, const sf::FloatRect& hitbox, const sf::FloatRect& hitbox2 = sf::FloatRect()) : m_origin(origin), m_pos(pos), m_lefthitbox(hitbox), m_righthitbox(hitbox2){}
        void SetOrigin(const sf::Vector2f& origin);
        sf::Vector2f GetOrigin() const;
        void SetPosition(const sf::Vector2f& pos);
        sf::Vector2f GetPosition() const;
        void SetLeftHitbox(const sf::FloatRect& hitbox);
        sf::FloatRect GetLeftHitbox() const;
        void SetRightHitbox(const sf::FloatRect& hitbox);
        sf::FloatRect GetRightHitbox() const;
    private:
        sf::Vector2f  m_origin;
        sf::Vector2f  m_pos;
        sf::FloatRect m_lefthitbox;
        sf::FloatRect m_righthitbox;
    };
}

#endif //COLLISIONOBJECT_HPP
