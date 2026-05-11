#include "Object/Bonus/NormalBrick.hpp"

#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/CustomTileManager.hpp"
#include "Core/Object/CustomTile/Behavior/BumpBehavior.hpp"
#include "Core/Object/CustomTile/Behavior/HitBehavior.hpp"
#include "Effect/BrickParticle.hpp"
#include "Object/Mario.hpp"

NormalBrick::NormalBrick(CustomTileManager &manager, const sf::Vector2f &position) : CustomTile(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setTexture("NormalBrick");
    setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 32.f}));
    setOrigin(sf::Vector2f(0.f, 0.f));
    MFCPP::setIndexTilemapCollision(position.x, position.y, true);
    MFCPP::setIndexTilemapID(position.x, position.y, 1);
    MFCPP::setIndexTilemapFloorY(position.x, position.y, {0, 32});
    setDrawingPriority(0);
    m_ypos = position.y;
    m_state = false;
    m_updown = false;
    m_state_count = 0.f;
}

void NormalBrick::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
}

void NormalBrick::interpolateData(float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void NormalBrick::Break() {
    AddBrickParticle(BRICK_NORMAL, getCurrentPosition().x, m_ypos);
    SoundManager::PlaySound("Break");
    m_customTileManager.setCollision(sf::Vector2f(getCurrentPosition().x, m_ypos), false);
    Mario::setScore(Mario::getScore() + 50);
    setDestroyed(true);
    m_customTileManager.setDeletionFlag(true);
}

void NormalBrick::KickEvent() {
    Break();
}

void NormalBrick::HitEvent() {
    if (isDestroyed()) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitboxWall(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect BrickHitbox = getGlobalHitbox(getHitbox(), sf::Vector2f(getCurrentPosition().x, m_ypos), getOrigin()); isCollide(BrickHitbox, hitbox_mario)) {
        if (Mario::getPowerState() > 0) {
            HitBehavior::HitDetection(MFCPP::CollisionObject(getCurrentPosition(), getOrigin(), getHitbox()));
            Break();
        }
        else {
            m_state = true;
            m_updown = false;
            m_state_count = 0.f;
            SoundManager::PlaySound("Bump");
            //Hit Event
            HitBehavior::HitDetection(MFCPP::CollisionObject(getCurrentPosition(), getOrigin(), getHitbox()));
        }
    }
}

void NormalBrick::statusUpdate(float deltaTime) {
    const BumpBehavior::BumpData newData = BumpBehavior::BumpCoinUpdate(BumpBehavior::BumpData(getCurrentPosition(), m_state_count, m_state, m_updown), m_ypos, deltaTime);
    m_state_count = newData.state_count;
    m_state = newData.state;
    m_updown = newData.updown;
    setCurrentPosition(newData.pos);
}

void NormalBrick::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.animationDraw();
}
