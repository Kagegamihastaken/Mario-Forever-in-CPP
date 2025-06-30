#include "Core/Class/BrickClass.hpp"

namespace MFCPP {
    Brick::Brick(const BrickID id, const BrickAtt att, const sf::Vector2f& pos) : m_ID(id),  m_att(att) {
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        setOrigin(sf::Vector2f(0.f, 0.f));

        m_state = false;
        m_state_count = 0.f;
        m_updown = false;
        m_YPos = pos.y;
        m_clock.restart();
        m_was_hit = false;
        m_disabled = false;
    }
    void Brick::setState(const bool val) {
        m_state = val;
    }
    bool Brick::getState() const {
        return m_state;
    }
    void Brick::setStateCount(const float val) {
        m_state_count = val;
    }
    float Brick::getStateCount() const {
        return m_state_count;
    }
    void Brick::setUpDown(const bool val) {
        m_updown = val;
    }
    bool Brick::getUpDown() const {
        return m_updown;
    }
    void Brick::setID(const BrickID val) {
        m_ID = val;
    }
    BrickID Brick::getID() const {
        return m_ID;
    }
    void Brick::setAtt(const BrickAtt val) {
        m_att = val;
    }
    BrickAtt Brick::getAtt() const {
        return m_att;
    }
    void Brick::setYPos(const float val) {
        m_YPos = val;
    }
    float Brick::getYPos() const {
        return m_YPos;
    }
    void Brick::restartClock() {
        m_clock.restart();
    }
    sf::Clock Brick::getClock() const {
        return m_clock;
    }
    void Brick::setWasHit(const bool val) {
        m_was_hit = val;
    }
    bool Brick::getWasHit() const {
        return m_was_hit;
    }
    void Brick::setDisabled(const bool val) {
        m_disabled = val;
    }
    bool Brick::getDisabled() const {
        return m_disabled;
    }

}