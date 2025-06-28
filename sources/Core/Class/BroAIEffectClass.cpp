#include "Core/Class/BroAIEffectClass.hpp"

namespace MFCPP {
    BroAIEffect::BroAIEffect(const float Yvelo, const sf::Vector2f& pos, const sf::Vector2f& origin)
        : m_Yvelo(Yvelo) {
        setOrigin(origin);
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
    }
    void BroAIEffect::setYVelo(const float val) {
        m_Yvelo = val;
    }
    float BroAIEffect::getYVelo() const {
        return m_Yvelo;
    }
}
