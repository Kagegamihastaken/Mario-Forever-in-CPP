//
// Created by dovie on 7/8/2025.
//

#ifndef COINCLASS_HPP
#define COINCLASS_HPP
#include "../Object/ActiveObject.hpp"
#include "../Enumeration/enum.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

namespace MFCPP {
    class Coin final {
    public:
        explicit Coin(CoinID id, CoinAtt att, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
        ~Coin() = default;
        void setID(CoinID val);
        [[nodiscard]] CoinID getID() const;
        void setAttribute(CoinAtt val);
        [[nodiscard]] CoinAtt getAttribute() const;
        void setHitbox(const sf::FloatRect& val);
        [[nodiscard]] sf::FloatRect getHitbox() const;
        void destroy();
        [[nodiscard]] sf::Vector2f getPosition() const;
        [[nodiscard]] sf::Vector2f getOrigin() const;
        [[nodiscard]] bool isDestroyed() const ;
        [[nodiscard]] sf::Vector2f getInterpolatedPosition(float alpha) const;
    private:
        CoinID        m_id;
        CoinAtt       m_att;
        sf::FloatRect m_hitbox;
        ActiveObject<float> m_transform;

    };
}

#endif //COINCLASS_HPP
