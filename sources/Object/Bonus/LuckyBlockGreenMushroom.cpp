#include "Object/Bonus/LuckyBlockGreenMushroom.hpp"

#include "Block/LuckyBlock.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Scroll.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Object/CustomTileManager.hpp"
#include "Core/Object/CustomTile/Behavior/BumpBehavior.hpp"
#include "Core/Object/CustomTile/Behavior/HitBehavior.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/BrickParticle.hpp"
#include "Object/Mario.hpp"
#include "Object/Bonus/GreenMushroom.hpp"

LuckyBlockGreenMushroom::LuckyBlockGreenMushroom(CustomTileManager &manager, const sf::Vector2f &position) : CustomTile(manager) {
    setCurrentPosition(position);
    setPreviousPosition(position);
    setInterpolatedPosition(position);
    m_animation.setAnimationSequence(NormLuckyBlockAnimName);
    m_animation.setAnimation(0, 2, 9);
    setHitbox(sf::FloatRect({0.f, 0.f}, {32.f, 32.f}));
    setOrigin(sf::Vector2f(0.f, 0.f));
    MFCPP::setIndexTilemapCollision(position.x, position.y, true);
    MFCPP::setIndexTilemapID(position.x, position.y, 1);
    m_ypos = position.y;
    m_state = false;
    m_updown = false;
    m_state_count = 0.f;
    m_disabled = false;
}

void LuckyBlockGreenMushroom::setPreviousData() {
    if (isDestroyed()) return;
    setPreviousPosition(getCurrentPosition());
}

void LuckyBlockGreenMushroom::interpolateData(float alpha) {
    if (isDestroyed()) return;
    setInterpolatedPosition(linearInterpolation(getPreviousPosition(), getCurrentPosition(), alpha));
}

void LuckyBlockGreenMushroom::Break() {
    AddBrickParticle(BRICK_NORMAL, getCurrentPosition().x, m_ypos);
    SoundManager::PlaySound("Break");
    m_customTileManager.setCollision(sf::Vector2f(getCurrentPosition().x, m_ypos), false);
    Score += 50;
    setDestroyed(true);
    m_customTileManager.setDeletionFlag(true);
}
void LuckyBlockGreenMushroom::Hit() {
    if (m_disabled) return;
    m_disabled = true;
    m_state = true;
    m_updown = false;
    m_state_count = 0.f;
    m_animation.setAnimation(3, 3, 9);
    GameScene::enemyManager.addEnemy<GreenMushroom>(getCurrentPosition() + sf::Vector2f(16.f, 0.f));
    SoundManager::PlaySound("Vine");

    //Hit Event
    HitBehavior::HitDetection(MFCPP::CollisionObject(getCurrentPosition(), getOrigin(), getHitbox()));
}

void LuckyBlockGreenMushroom::KickEvent() {
    if (!m_disabled) Hit();
}

void LuckyBlockGreenMushroom::HitEvent() {
    if (isDestroyed() || m_disabled) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxWall, player.curr, player.property.getOrigin());
    if (const sf::FloatRect BrickHitbox = getGlobalHitbox(getHitbox(), sf::Vector2f(getCurrentPosition().x, m_ypos), getOrigin()); isCollide(BrickHitbox, hitbox_mario)) {
        Hit();
    }
}

void LuckyBlockGreenMushroom::statusUpdate(float deltaTime) {
    const BumpBehavior::BumpData newData = BumpBehavior::BumpItemUpdate(BumpBehavior::BumpData(getCurrentPosition(), m_state_count, m_state, m_updown), m_ypos, deltaTime);
    m_state_count = newData.state_count;
    m_state = newData.state;
    m_updown = newData.updown;
    setCurrentPosition(newData.pos);
}

void LuckyBlockGreenMushroom::draw() {
    if (isOutOfScreen(MFCPP::CollisionObject(getInterpolatedPosition(), getOrigin(), getHitbox()), 0.f)) return;
    m_animation.AnimationUpdate(getInterpolatedPosition(), getOrigin());
    m_animation.AnimationDraw();
}
