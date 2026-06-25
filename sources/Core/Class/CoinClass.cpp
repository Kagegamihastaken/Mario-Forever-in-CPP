#include "Core/Class/CoinClass.hpp"
namespace MFCPP {
    Coin::Coin(const CoinID id, const CoinAtt att, const sf::FloatRect &hitbox, const sf::Vector2f &pos, const sf::Vector2f &origin)
        : m_id(id),
        m_att(att),
        m_transform(pos, origin, sf::degrees(0.f)),
        m_hitbox(hitbox){};
    void Coin::setID(const CoinID val) {
        m_id = val;
    }
    CoinID Coin::getID() const {
        return m_id;
    }
    void Coin::setAttribute(const CoinAtt val) {
        m_att = val;
    }
    CoinAtt Coin::getAttribute() const {
        return m_att;
    }
    void Coin::setHitbox(const sf::FloatRect &val) {
        m_hitbox = val;
    }

    sf::FloatRect Coin::getHitbox() const {
        return m_hitbox;
    }

    void Coin::destroy() {
        m_transform.destroy();
    }

    sf::Vector2f Coin::getPosition() const {
        return m_transform.getCurrentPosition();
    }

    sf::Vector2f Coin::getOrigin() const {
        return m_transform.getOrigin();
    }

    bool Coin::isDestroyed() const {
        return m_transform.isDestroyed();
    }

    sf::Vector2f Coin::getInterpolatedPosition(float alpha) const {
        return m_transform.getInterpolatedPosition(alpha);
    }
}
