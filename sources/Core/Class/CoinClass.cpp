#include "Core/Class/CoinClass.hpp"
namespace MFCPP {
    Coin::Coin(const CoinID id, const CoinAtt att, const sf::FloatRect &hitbox, const sf::Vector2f &pos, const sf::Vector2f &origin)
        : m_hitbox(hitbox), m_id(id), m_att(att) {
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        setOrigin(origin);
    }
    void Coin::setHitbox(const sf::FloatRect &val) {
        m_hitbox = val;
    }
    sf::FloatRect Coin::getHitbox() const {
        return m_hitbox;
    }
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
}