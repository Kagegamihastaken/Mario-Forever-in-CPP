#include "Core/Class/BrickParticleClass.hpp"

namespace MFCPP {
    BrickParticle::BrickParticle(float Xvelo, float Yvelo, const sf::Vector2f &pos, const sf::Vector2f &origin) : m_Xvelo(Xvelo), m_Yvelo(Yvelo) {
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        setOrigin(origin);
    }
    void BrickParticle::setXvelo(const float val) {
        m_Xvelo = val;
    }
    void BrickParticle::setYvelo(const float val) {
        m_Yvelo = val;
    }
    float BrickParticle::getXvelo() const {
        return m_Xvelo;
    }
    float BrickParticle::getYvelo() const {
        return m_Yvelo;
    }
}