#include "Object/Bonus/NormalCoinBrick.hpp"

#include "Core/Interpolation.hpp"
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

NormalCoinBrick::NormalCoinBrick(CustomTileManager &manager, const sf::Vector2f &position) : CustomTile(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("NormalBrick");
    setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 32.f}));
    setOrigin(sf::Vector2f(0.f, 0.f));
    MFCPP::setIndexTilemapCollision(position.x, position.y, true);
    MFCPP::setIndexTilemapID(position.x, position.y, 1);
    m_ypos = position.y;
    m_state = false;
    m_updown = false;
    m_state_count = 0.f;
    m_time = 0.f;
    m_disabled = false;
    m_hitted = false;

    m_time_limit = 300.f;
}

void NormalCoinBrick::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
}

void NormalCoinBrick::interpolateData(float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void NormalCoinBrick::Break() {
    AddBrickParticle(BRICK_NORMAL, getCurrentPosition().x, m_ypos);
    SoundManager::PlaySound("Break");
    m_customTileManager.setCollision(sf::Vector2f(getCurrentPosition().x, m_ypos), false);
    Score += 50;
    setDestroyed(true);
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
    AddCoinEffect(COIN_NORMAL, ONE_COIN, getCurrentPosition().x + 15.0f, getCurrentPosition().y);
    ++CoinCount;
    SoundManager::PlaySound("Coin");

    //Hit Event
    HitBehavior::HitDetection(MFCPP::CollisionObject(getCurrentPosition(), getOrigin(), getHitbox()));
}

void NormalCoinBrick::KickEvent() {
    if (!m_disabled) Hit();
}

void NormalCoinBrick::HitEvent() {
    if (isDestroyed() || m_disabled) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxWall, player.curr, player.property.getOrigin());
    if (const sf::FloatRect BrickHitbox = getGlobalHitbox(getHitbox(), sf::Vector2f(getCurrentPosition().x, m_ypos), getOrigin()); isCollide(BrickHitbox, hitbox_mario)) {
        Hit();
    }
}

void NormalCoinBrick::statusUpdate(float deltaTime) {
    // Time Counting
    if (m_hitted && m_time <= m_time_limit) m_time += deltaTime;
    // Moving
    const BumpBehavior::BumpData newData = BumpBehavior::BumpCoinUpdate(BumpBehavior::BumpData(getCurrentPosition(), m_state_count, m_state, m_updown), m_ypos, deltaTime);
    m_state_count = newData.state_count;
    m_state = newData.state;
    m_updown = newData.updown;
    setCurrentPosition(newData.pos);
}

void NormalCoinBrick::draw() {
    if (isOutScreen(getInterpolatedPosition().x, getInterpolatedPosition().y, 32, 32)) return;
    m_animation.AnimationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.AnimationDraw();
}
