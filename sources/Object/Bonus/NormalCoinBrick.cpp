#include "Object/Bonus/NormalCoinBrick.hpp"

#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/CustomTileManager.hpp"
#include "Core/Object/CustomTile/Behavior/BumpBehavior.hpp"
#include "Core/Object/CustomTile/Behavior/HitBehavior.hpp"
#include "Effect/BrickParticle.hpp"
#include "Effect/CoinEffect.hpp"
#include "Object/Coin.hpp"
#include "Object/Mario.hpp"

NormalCoinBrick::NormalCoinBrick(CustomTileManager &manager, const sf::Vector2f &position)
    : CustomTile(manager),
    m_transform(position, sf::Vector2f(0.f, 0.f), sf::degrees(0.f)){
    m_animation.setTexture("NormalBrick");
    m_hitbox = sf::FloatRect({0.f, 0.f}, {32.f, 32.f});
    MFCPP::Tilemap::setIndexTilemapCollision(position.x, position.y, true);
    MFCPP::Tilemap::setIndexTilemapID(position.x, position.y, 1);
    MFCPP::Tilemap::setIndexTilemapFloorY(position.x, position.y, {0, 32});
    setDrawingPriority(0);
    m_ypos = position.y;
    m_state = false;
    m_updown = false;
    m_state_count = 0.f;
    m_time = 0.f;
    m_disabled = false;
    m_hitted = false;

    m_time_limit = 300.f;
}

void NormalCoinBrick::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void NormalCoinBrick::Break() {
    AddBrickParticle(BrickID::NORMAL, m_transform.getCurrentPosition().x, m_ypos);
    SoundManager::PlaySound(SoundID::GAME_BREAK);
    m_customTileManager.setCollision(sf::Vector2f(m_transform.getCurrentPosition().x, m_ypos), false);
    Mario::setScore(Mario::getScore() + 50);
    m_transform.destroy();
    m_customTileManager.setDeletionFlag(true);
}
void NormalCoinBrick::Hit() {
    if (m_disabled) return;

    if (!m_hitted) {
        m_hitted = true;
        m_time = 0.f;
    }
    else {
        if (m_time > m_time_limit) {
            m_animation.setTexture("NormalHittedBrick");
            m_disabled = true;
        }
    }
    m_state = true;
    m_updown = false;
    m_state_count = 0.f;
    AddCoinEffect(CoinID::NORMAL, CoinAtt::ONE_COIN, m_transform.getCurrentPosition().x + 15.0f, m_transform.getCurrentPosition().y);
    ++CoinCount;
    SoundManager::PlaySound(SoundID::GAME_COIN);

    //Hit Event
    HitBehavior::HitDetection(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()));
}

void NormalCoinBrick::KickEvent() {
    if (!m_disabled) Hit();
}

void NormalCoinBrick::HitEvent() {
    if (isDestroyed() || m_disabled) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitboxWall(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect BrickHitbox = getGlobalHitbox(getHitbox(), sf::Vector2f(m_transform.getCurrentPosition().x, m_ypos), getOrigin()); isCollide(BrickHitbox, hitbox_mario)) {
        Hit();
    }
}

void NormalCoinBrick::statusUpdate(float deltaTime) {
    // Time Counting
    if (m_hitted && m_time <= m_time_limit) m_time += deltaTime;
    // Moving
    const BumpBehavior::BumpData newData = BumpBehavior::BumpCoinUpdate(BumpBehavior::BumpData(m_transform.getCurrentPosition(), m_state_count, m_state, m_updown), m_ypos, deltaTime);
    m_state_count = newData.state_count;
    m_state = newData.state;
    m_updown = newData.updown;
    m_transform.setCurrentPosition(newData.pos);
}

void NormalCoinBrick::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

sf::Vector2f NormalCoinBrick::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f NormalCoinBrick::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect NormalCoinBrick::getHitbox() {
    return m_hitbox;
}

bool NormalCoinBrick::isDestroyed() {
    return m_transform.isDestroyed();
}

void NormalCoinBrick::animationUpdate(float deltaTime) {}
