#include "Object/Bonus/NormalBrick.hpp"

#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/CustomTileManager.hpp"
#include "Core/Object/CustomTile/Behavior/BumpBehavior.hpp"
#include "Core/Object/CustomTile/Behavior/HitBehavior.hpp"
#include "Effect/BrickParticle.hpp"
#include "Object/Mario.hpp"

NormalBrick::NormalBrick(CustomTileManager &manager, const sf::Vector2f &position)
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
}

void NormalBrick::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void NormalBrick::Break() {
    AddBrickParticle(BrickID::BRICK_NORMAL, m_transform.getCurrentPosition().x, m_ypos);
    SoundManager::PlaySound("Break");
    m_customTileManager.setCollision(sf::Vector2f(m_transform.getCurrentPosition().x, m_ypos), false);
    Mario::setScore(Mario::getScore() + 50);
    m_transform.destroy();
    m_customTileManager.setDeletionFlag(true);
}

void NormalBrick::KickEvent() {
    Break();
}

void NormalBrick::HitEvent() {
    if (isDestroyed()) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitboxWall(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect BrickHitbox = getGlobalHitbox(getHitbox(), sf::Vector2f(m_transform.getCurrentPosition().x, m_ypos), getOrigin()); isCollide(BrickHitbox, hitbox_mario)) {
        if (Mario::getPowerState() > 0) {
            HitBehavior::HitDetection(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()));
            Break();
        }
        else {
            m_state = true;
            m_updown = false;
            m_state_count = 0.f;
            SoundManager::PlaySound("Bump");
            //Hit Event
            HitBehavior::HitDetection(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()));
        }
    }
}

void NormalBrick::statusUpdate(float deltaTime) {
    const BumpBehavior::BumpData newData = BumpBehavior::BumpCoinUpdate(BumpBehavior::BumpData(m_transform.getCurrentPosition(), m_state_count, m_state, m_updown), m_ypos, deltaTime);
    m_state_count = newData.state_count;
    m_state = newData.state;
    m_updown = newData.updown;
    m_transform.setCurrentPosition(newData.pos);
}

void NormalBrick::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

sf::Vector2f NormalBrick::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f NormalBrick::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect NormalBrick::getHitbox() {
    return m_hitbox;
}

bool NormalBrick::isDestroyed() {
    return m_transform.isDestroyed();
}

void NormalBrick::animationUpdate(float deltaTime) {}
