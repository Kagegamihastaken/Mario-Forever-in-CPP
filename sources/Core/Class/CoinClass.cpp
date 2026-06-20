#include "Core/Class/CoinClass.hpp"
namespace MFCPP {
    Coin::Coin(const CoinID id, const CoinAtt att, const sf::FloatRect &hitbox, const sf::Vector2f &pos, const sf::Vector2f &origin)
        : m_id(id), m_att(att) {
        setHitbox(hitbox);
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        setOrigin(origin);
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