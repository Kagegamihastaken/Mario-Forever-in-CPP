#include "Class/CoinEffectClass.hpp"

namespace MFCPP {
    CoinEffect::CoinEffect(const CoinID id, const CoinAtt att, const sf::Vector2f& pos, const sf::Vector2f& origin) : m_id(id), m_att(att) {
        setCurrentPosition(pos);
        setInterpolatedPosition(pos);
        setPreviousPosition(pos);
        setOrigin(origin);
        m_Yvelo = -3.5f;
    }
    void CoinEffect::setID(const CoinID id) {
        m_id = id;
    }
    CoinID CoinEffect::getID() const {
        return m_id;
    }
    void CoinEffect::setAttribute(const CoinAtt att) {
        m_att = att;
    }
    CoinAtt CoinEffect::getAttribute() const {
        return m_att;
    }
    void CoinEffect::setYVelo(const float val) {
        m_Yvelo = val;
    }
    float CoinEffect::getYVelo() const {
        return m_Yvelo;
    }
}