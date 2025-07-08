#include "Core/Class/BulletBillEffectClass.hpp"

namespace MFCPP {
    BulletBillEffect::BulletBillEffect(const float Yvelo, const float Xvelo, const sf::Vector2f& pos, const sf::Vector2f& origin)
        : m_Yvelo(Yvelo), m_Xvelo(Xvelo) {
        setOrigin(origin);
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
    }
    void BulletBillEffect::setYVelo(const float val) {
        m_Yvelo = val;
    }
    float BulletBillEffect::getYVelo() const {
        return m_Yvelo;
    }
    void BulletBillEffect::setXVelo(const float val) {
        m_Xvelo = val;
    }
    float BulletBillEffect::getXVelo() const {
        return m_Xvelo;
    }
}
