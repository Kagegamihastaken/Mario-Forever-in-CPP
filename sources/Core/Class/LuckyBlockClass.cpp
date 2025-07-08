#include "Core/Class/LuckyBlockClass.hpp"

namespace MFCPP {
    LuckyBlock::LuckyBlock(const LuckyBlockID id, const LuckyBlockAtt att, const sf::Vector2f &pos)
        : m_id(id), m_att(att) {
        setCurrentPosition(pos);
        setPreviousPosition(pos);
        setInterpolatedPosition(pos);
        setOrigin(sf::Vector2f(0.f, 0.f));
        m_state = false;
        m_state_count = 0.f;
        m_updown = false;
        m_was_hit = false;
        m_yPos = pos.y;
        m_hitbox = sf::FloatRect({ 0.f, 0.f }, { 32.f, 32.f });
    }
    void LuckyBlock::setState(const bool val) {
        m_state = val;
    }
    bool LuckyBlock::getState() const {
        return m_state;
    }
    void LuckyBlock::setStateCount(const float val) {
        m_state_count = val;
    }
    float LuckyBlock::getStateCount() const {
        return m_state_count;
    }
    void LuckyBlock::setUpDown(const bool val) {
        m_updown = val;
    }
    bool LuckyBlock::getUpDown() const {
        return m_updown;
    }
    void LuckyBlock::setHitbox(const sf::FloatRect &val) {
        m_hitbox = val;
    }
    sf::FloatRect LuckyBlock::getHitbox() const {
        return m_hitbox;
    }
    void LuckyBlock::setWasHit(const bool val) {
        m_was_hit = val;
    }
    bool LuckyBlock::WasHit() const {
        return m_was_hit;
    }
    void LuckyBlock::setAtt(const LuckyBlockAtt val) {
        m_att = val;
    }
    LuckyBlockAtt LuckyBlock::getAtt() const {
        return m_att;
    }
    void LuckyBlock::setID(const LuckyBlockID val) {
        m_id = val;
    }
    LuckyBlockID LuckyBlock::getID() const {
        return m_id;
    }
    void LuckyBlock::setYPos(const float val) {
        m_yPos = val;
    }
    float LuckyBlock::getYPos() const {
        return m_yPos;
    }
}