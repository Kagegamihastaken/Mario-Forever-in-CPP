//
// Created by dovie on 7/8/2025.
//

#ifndef COINCLASS_HPP
#define COINCLASS_HPP
#include "ActiveObjectClass.hpp"
#include "Core/Loading/enum.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

namespace MFCPP {
    class Coin final : public ActiveObject<float> {
    public:
        Coin(CoinID id, CoinAtt att, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
        ~Coin() = default;
        void setHitbox(const sf::FloatRect& val);
        [[nodiscard]] sf::FloatRect getHitbox() const;
        void setID(CoinID val);
        [[nodiscard]] CoinID getID() const;
        void setAttribute(CoinAtt val);
        [[nodiscard]] CoinAtt getAttribute() const;
    private:
        sf::FloatRect m_hitbox;
        CoinID        m_id;
        CoinAtt       m_att;
    };
}

#endif //COINCLASS_HPP
