#ifndef BRICKCLASS_HPP
#define BRICKCLASS_HPP
#include "ActiveObjectClass.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Loading/enum.hpp"
#include <SFML/System/Clock.hpp>

namespace MFCPP {
    class Brick final : public ActiveObject<float>, public StaticAnimationObject {
    public:
        explicit Brick(BrickID id, BrickAtt att, const sf::Vector2f& pos);
        void setState(bool val);
        [[nodiscard]] bool getState() const;
        void setStateCount(float val);
        [[nodiscard]] float getStateCount() const;
        void setUpDown(bool val);
        [[nodiscard]] bool getUpDown() const;
        void setID(BrickID val);
        [[nodiscard]] BrickID getID() const;
        void setAtt(BrickAtt val);
        [[nodiscard]] BrickAtt getAtt() const;
        void setYPos(float val);
        [[nodiscard]] float getYPos() const;
        void restartClock();
        [[nodiscard]] sf::Clock getClock() const;
        void setWasHit(bool val);
        [[nodiscard]] bool getWasHit() const;
        void setDisabled(bool val);
        [[nodiscard]] bool getDisabled() const;
        ~Brick() = default;
    private:
        bool      m_state;
        float     m_state_count;
        bool      m_updown;
        BrickID   m_ID;
        BrickAtt  m_att;
        float     m_YPos;
        sf::Clock m_clock;
        bool      m_was_hit;
        bool      m_disabled;

    };
}

#endif //BRICKCLASS_HPP
