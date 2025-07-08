#ifndef LUCKYBLOCKCLASS_HPP
#define LUCKYBLOCKCLASS_HPP
#include "ActiveObjectClass.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "SFML/Graphics/Rect.hpp"
#include "SFML/System/Vector2.hpp"

namespace MFCPP {
    class LuckyBlock final : public ActiveObject, public SingleAnimationObject {
    public:
        LuckyBlock(LuckyBlockID id, LuckyBlockAtt att, const sf::Vector2f& pos);
        ~LuckyBlock() = default;
        void setID(LuckyBlockID val);
        [[nodiscard]] LuckyBlockID getID() const;
        void setAtt(LuckyBlockAtt val);
        [[nodiscard]] LuckyBlockAtt getAtt() const;
        void setState(bool val);
        [[nodiscard]] bool getState() const;
        void setStateCount(float val);
        [[nodiscard]] float getStateCount() const;
        void setUpDown(bool val);
        [[nodiscard]] bool getUpDown() const;
        void setHitbox(const sf::FloatRect& val);
        [[nodiscard]] sf::FloatRect getHitbox() const;
        void setWasHit(bool val);
        [[nodiscard]] bool WasHit() const;
        void setYPos(float val);
        [[nodiscard]] float getYPos() const;
    private:
        LuckyBlockID  m_id{};
        LuckyBlockAtt m_att{};
        sf::FloatRect m_hitbox{};
        bool          m_state{};
        float         m_state_count{};
        bool          m_updown{};
        bool          m_was_hit{};
        float         m_yPos{};
    };
}

#endif //LUCKYBLOCKCLASS_HPP
