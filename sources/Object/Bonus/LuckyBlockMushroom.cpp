#include "Object/Bonus/LuckyBlockMushroom.hpp"

#include "Block/LuckyBlock.hpp"
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
#include "Object/Bonus/Mushroom.hpp"

LuckyBlockMushroom::LuckyBlockMushroom(CustomTileManager &manager, const sf::Vector2f &position)
    : CustomTile(manager),
    m_transform(position, sf::Vector2f(0.f, 0.f), sf::degrees(0.f)){
    m_animation.setAnimationSequence("NormLuckyBlockAnimName");
    m_animation.setAnimation(0, 2, 9, true);
    m_hitbox = sf::FloatRect({0.f, 0.f}, {32.f, 32.f});
    MFCPP::Tilemap::setIndexTilemapCollision(position.x, position.y, true);
    MFCPP::Tilemap::setIndexTilemapID(position.x, position.y, 1);
    MFCPP::Tilemap::setIndexTilemapFloorY(position.x, position.y, {0, 32});
    setDrawingPriority(0);
    m_ypos = position.y;
    m_state = false;
    m_updown = false;
    m_state_count = 0.f;
    m_disabled = false;
}

void LuckyBlockMushroom::updatePreviousData() {
    if (isDestroyed()) return;
    m_transform.Update();
}

void LuckyBlockMushroom::Break() {
    AddBrickParticle(BrickID::NORMAL, m_transform.getCurrentPosition().x, m_ypos);
    SoundManager::PlaySound(SoundID::GAME_BREAK);
    m_customTileManager.setCollision(sf::Vector2f(m_transform.getCurrentPosition().x, m_ypos), false);
    Mario::setScore(Mario::getScore() + 50);
    m_transform.destroy();
    m_customTileManager.setDeletionFlag(true);
}
void LuckyBlockMushroom::Hit() {
    if (m_disabled) return;
    m_disabled = true;
    m_state = true;
    m_updown = false;
    m_state_count = 0.f;
    m_animation.setAnimation(3, 3, 9);
    GameScene::enemyManager.addEnemy<Mushroom>(m_transform.getCurrentPosition() + sf::Vector2f(16.f, 0.f));
    SoundManager::PlaySound(SoundID::GAME_VINE);

    //Hit Event
    HitBehavior::HitDetection(MFCPP::CollisionObject(m_transform.getCurrentPosition(), getOrigin(), getHitbox()));
}

void LuckyBlockMushroom::KickEvent() {
    if (!m_disabled) Hit();
}

void LuckyBlockMushroom::HitEvent() {
    if (isDestroyed() || m_disabled) return;
    const sf::FloatRect hitbox_mario = getGlobalHitbox(Mario::getHitboxWall(), Mario::getCurrentPosition(), Mario::getOrigin());
    if (const sf::FloatRect BrickHitbox = getGlobalHitbox(getHitbox(), sf::Vector2f(m_transform.getCurrentPosition().x, m_ypos), getOrigin()); isCollide(BrickHitbox, hitbox_mario)) {
        Hit();
    }
}

void LuckyBlockMushroom::statusUpdate(float deltaTime) {
    const BumpBehavior::BumpData newData = BumpBehavior::BumpItemUpdate(BumpBehavior::BumpData(m_transform.getCurrentPosition(), m_state_count, m_state, m_updown), m_ypos, deltaTime);
    m_state_count = newData.state_count;
    m_state = newData.state;
    m_updown = newData.updown;
    m_transform.setCurrentPosition(newData.pos);
}

void LuckyBlockMushroom::draw(float alpha) {
    if (Scroll::isOutOfScreen(MFCPP::CollisionObject(m_transform.getInterpolatedPosition(alpha), getOrigin(), getHitbox()), 0.f)) {
        m_animation.frameUpdate();
        return;
    }
    m_animation.animationUpdate(m_transform.getInterpolatedPosition(alpha), getOrigin());
    m_animation.animationDraw();
}

void LuckyBlockMushroom::animationUpdate(float deltaTime) {
    m_animation.frameTimeAccumulate(deltaTime);
}

sf::Vector2f LuckyBlockMushroom::getPosition() {
    return m_transform.getCurrentPosition();
}

sf::Vector2f LuckyBlockMushroom::getOrigin() {
    return m_transform.getOrigin();
}

sf::FloatRect LuckyBlockMushroom::getHitbox() {
    return m_hitbox;
}

bool LuckyBlockMushroom::isDestroyed() {
    return m_transform.isDestroyed();
}
