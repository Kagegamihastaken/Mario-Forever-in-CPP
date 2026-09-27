#include "Core/Object/Enemy/Behavior/ThwompAIBehavior.hpp"
#include "Core/ShakeView.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Utility.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Enumeration/SoundEnum.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Effect/FireballExplosionEffect.hpp"

auto ThwompAIBehavior::ThwompYStatusUpdate(const ThwompAIBehavior::ThwompData& data, const sf::FloatRect& hitbox, const sf::Vector2f& origin, float deltaTime) -> ThwompAIBehavior::ThwompData {
    auto dataOutput = data;
    if (dataOutput.decending && !dataOutput.hitGround) {
        dataOutput.position.y += dataOutput.velocity.y * deltaTime;
        dataOutput.velocity.y += deltaTime;
        //Check bottom
        // Bottom check
        bool NoAdd = false;
        bool l_hitGroundDetected = false;
        float CurrPosXCollide = 0.f, CurrPosYCollide = 0.f;
        bool l_bonusCollided = false;
        for (auto& bonus : GameScene::customTileManager.getBonusList()) {
            if (const sf::FloatRect BonusHitbox = getGlobalHitbox(bonus.getHitbox(), bonus.getPosition(), bonus.getOrigin()); isCollide(BonusHitbox, getGlobalHitbox(hitbox, dataOutput.position, origin))) {
                bonus.KickEvent();
                if (!bonus.isDestroyed()) {
                    CurrPosYCollide = bonus.getPosition().y;
                    l_bonusCollided = true;
                }
            }
        }
        if (l_bonusCollided) {
            dataOutput.velocity.y = 0.f;
            dataOutput.position.y = CurrPosYCollide - (hitbox.size.y - origin.y);
            l_hitGroundDetected = true;
        }
        if (float PlatPosY; PlatformYCollision(MFCPP::CollisionObject(dataOutput.position, origin, hitbox), PlatPosY, dataOutput.velocity.y, false)) {
            dataOutput.position.y = PlatPosY;
            dataOutput.velocity.y = 0.f;
            l_hitGroundDetected = true;
        }
        const float offset = std::min(dataOutput.velocity.x + 1.f, 3.f);
        if (QuickCheckOnlyObstacleBotCollision(MFCPP::CollisionObject(dataOutput.position, origin, hitbox), offset, CurrPosXCollide, CurrPosYCollide, NoAdd)) {
            if (dataOutput.velocity.y >= -dataOutput.velocity.x) {
                const float floorY = GetCurrFloorY(dataOutput.position, CurrPosXCollide, CurrPosYCollide);
                if (!(dataOutput.position.y < CurrPosYCollide + floorY - offset)) {
                    dataOutput.velocity.y = 0.f;
                    dataOutput.position.y = CurrPosYCollide + floorY - (hitbox.size.y - origin.y);
                    l_hitGroundDetected = true;
                }
            }
        }

        if (l_hitGroundDetected) {
            MFCPP::ShakeView::shake(Utility::RandomFloatNumberGenerator(4.f, 6.f));
            SoundManager::PlaySound(SoundID::GAME_STUN);
            dataOutput.decending = false;
            dataOutput.hitGround = true;
            //Fireball Explosion Effect
            //Left
            bool isCollidedLeft = false;
            bool isCollidedRight = false;
            if (float PlatPosY; PlatformYCollision(MFCPP::CollisionObject(dataOutput.position, sf::Vector2f(origin.x, 0.f), sf::FloatRect({0.f, 0.f}, {32.f, 8.f})), PlatPosY, dataOutput.velocity.y, false)) {
                isCollidedLeft = true;
            }
            if (QuickCheckBotCollision(MFCPP::CollisionObject(dataOutput.position, sf::Vector2f(origin.x, 0.f), sf::FloatRect({0.f, 0.f}, {32.f, 8.f})), offset, CurrPosXCollide, CurrPosYCollide)) {
                if (dataOutput.velocity.y >= -dataOutput.velocity.x) {
                    const float floorY = GetCurrFloorY(dataOutput.position, CurrPosXCollide, CurrPosYCollide);
                    if (!(dataOutput.position.y < CurrPosYCollide + floorY - offset)) {
                        isCollidedLeft = true;
                    }
                }
            }
            if (isCollidedLeft)
                GameScene::effectManager.addEffect<FireballExplosionEffect>(sf::Vector2f(dataOutput.position.x - origin.x + 16.f, dataOutput.position.y));
            //Right
            if (float PlatPosY; PlatformYCollision(MFCPP::CollisionObject(dataOutput.position + sf::Vector2f(32.f, 0.f), sf::Vector2f(origin.x, 0.f), sf::FloatRect({0.f, 0.f}, {32.f, 8.f})), PlatPosY, dataOutput.velocity.y, false)) {
                isCollidedRight = true;
            }
            if (QuickCheckBotCollision(MFCPP::CollisionObject(dataOutput.position + sf::Vector2f(32.f, 0.f), sf::Vector2f(origin.x, 0.f), sf::FloatRect({0.f, 0.f}, {32.f, 8.f})), offset, CurrPosXCollide, CurrPosYCollide)) {
                if (dataOutput.velocity.y >= -dataOutput.velocity.x) {
                    const float floorY = GetCurrFloorY(dataOutput.position, CurrPosXCollide, CurrPosYCollide);
                    if (!(dataOutput.position.y < CurrPosYCollide + floorY - offset)) {
                        isCollidedRight = true;
                    }
                }
            }
            if (isCollidedRight)
                GameScene::effectManager.addEffect<FireballExplosionEffect>(sf::Vector2f(dataOutput.position.x - origin.x + 48.f, dataOutput.position.y));
        }
    }

    if (dataOutput.hitGround) {
        dataOutput.countingBeforeRise += 1.f * deltaTime;
        if (dataOutput.countingBeforeRise >= dataOutput.rise) {
            dataOutput.position.y -= dataOutput.y_velocity_rise * deltaTime;
            if (dataOutput.position.y <= dataOutput.yStore) {
                dataOutput.position.y = dataOutput.yStore;
                dataOutput.hitGround = false;
                dataOutput.countingBeforeRise = 0.f;
            }
        }
    }
    return dataOutput;
}
