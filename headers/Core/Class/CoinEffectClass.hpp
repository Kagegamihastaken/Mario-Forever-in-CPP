#ifndef COINEFFECTCLASS_HPP
#define COINEFFECTCLASS_HPP
#include "Core/Class/ActiveObjectClass.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"

namespace MFCPP {
    class CoinEffect : public ActiveObject, public SingleAnimationObject {
    public:
        CoinEffect(CoinID id, CoinAtt att, const sf::Vector2f& pos, const sf::Vector2f& origin);
        void setID(CoinID id);
        [[nodiscard]] CoinID getID() const;
        void setAttribute(CoinAtt att);
        [[nodiscard]] CoinAtt getAttribute() const;
        void setYVelo(float val);
        [[nodiscard]] float getYVelo() const;
    private:
        CoinID  m_id;
        CoinAtt m_att;
        float   m_Yvelo;
    };
}

#endif //COInEFFECTCLASS_HPP
