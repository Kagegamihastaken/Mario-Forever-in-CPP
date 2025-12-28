#include "Core/Object/Enemy/GoombaAIBehavior.hpp"
#include <algorithm>

#include "Core/SoundManager.hpp"
#include "Core/Collision/Collide.hpp"
#include "Object/Mario.hpp"
#include "SFML/Graphics/Rect.hpp"

GoombaAIBehavior::GoombaAIData GoombaAIBehavior::GoombaAIYMove(const GoombaAIData& data, const float deltaTime) {
    GoombaAIData dataOutput = data;
    dataOutput.position += sf::Vector2f(0.f, data.velocity.y * deltaTime);
    dataOutput.velocity.y += (data.velocity.y >= 10.0f ? 0.0f : 1.f * deltaTime * 0.3f);
    return dataOutput;
}
GoombaAIBehavior::GoombaAIData GoombaAIBehavior::GoombaAIYCollision(const GoombaAIData& data, const sf::FloatRect& hitbox, const sf::Vector2f& origin, const bool RedKoopaAIEnabled, const float prevX) {
    GoombaAIData dataOutput = data;
    float CurrPosXCollide, CurrPosYCollide;
    // Bottom check
    if (float PlatPosY; PlatformYCollision(MFCPP::CollisionObject(data.position, origin, hitbox), PlatPosY, data.velocity.y, false)) {
        if (RedKoopaAIEnabled) {
            if (!PlatformYCollisionEdge(MFCPP::CollisionObject(data.position, origin, hitbox), data.velocity.y, !data.direction)) {
                dataOutput.position.x = prevX;
                dataOutput.direction = !data.direction;
            }
        }
        dataOutput.position.y = PlatPosY;
        dataOutput.velocity.y = 0.f;
    }
    if (QuickCheckBotCollision(MFCPP::CollisionObject(data.position, origin, hitbox), CurrPosXCollide, CurrPosYCollide)) {
        if (RedKoopaAIEnabled) {
            if (!isAccurateCollideBotStopEdge(MFCPP::CollisionObject({data.position.x, data.position.y + 1.f}, origin, hitbox), !data.direction)) {
                dataOutput.position.x = prevX;
                dataOutput.direction = !data.direction;
            }
        }

        if (const float offset = std::min(data.velocity.x + 1.f, 3.f); data.velocity.y >= -data.velocity.x) {
            const float floorY = GetCurrFloorY(data.position, CurrPosXCollide, CurrPosYCollide);
            if (data.position.y < CurrPosYCollide + floorY - offset) return dataOutput;
            dataOutput.velocity.y = 0.f;
            dataOutput.position.y = CurrPosYCollide + floorY - (hitbox.size.y - origin.y);
            return dataOutput;
        }
    }
    if (QuickCheckTopCollision(MFCPP::CollisionObject(data.position, origin, hitbox), CurrPosXCollide, CurrPosYCollide)) {
        dataOutput.velocity.y = 0.f;
        dataOutput.position.y = CurrPosYCollide + (32.0f + origin.y);
    }
    return dataOutput;
}

GoombaAIBehavior::GoombaAIData GoombaAIBehavior::GoombaAIXMove(const GoombaAIData &data, const float deltaTime) {
    GoombaAIData dataOutput = data;
    dataOutput.position.x += data.velocity.x * (data.direction ? 1.f : -1.f) * deltaTime;
    return dataOutput;
}

GoombaAIBehavior::GoombaAIData GoombaAIBehavior::GoombaAIXCollision(const GoombaAIData &data, const sf::FloatRect &hitbox, const sf::FloatRect& hitbox_wall, const sf::Vector2f &origin) {
    GoombaAIData dataOutput = data;
    if (float PlatDistance; PlatformXCollision(MFCPP::CollisionObject(data.position, origin, hitbox), PlatDistance, data.velocity.y))
        dataOutput.position.x += PlatDistance;
    float CurrPosXCollide = 0.f, CurrPosYCollide = 0.f;
    bool NoAdd = false;
    // Loop through obstacles
    auto [fst, snd] = QuickCheckOnlyObstaclesSideCollision(
        MFCPP::CollisionObject(data.position, origin, hitbox_wall),
        data.direction, CurrPosXCollide, CurrPosYCollide, NoAdd);
    std::pair<bool, bool> BrickCollide = CheckCollisionSide(MFCPP::CollisionObject(data.position, origin, hitbox_wall), CurrPosXCollide, CurrPosYCollide, NoAdd, 1);
    std::pair<bool, bool> LuckyCollide = CheckCollisionSide(MFCPP::CollisionObject(data.position, origin, hitbox_wall), CurrPosXCollide, CurrPosYCollide, NoAdd, 2);
    if (fst || BrickCollide.first || LuckyCollide.first) {
        dataOutput.direction = !data.direction;
        dataOutput.position.x = CurrPosXCollide + 32.0f + origin.x;
    }
    if (snd || BrickCollide.second || LuckyCollide.second) {
        dataOutput.direction = !data.direction;
        dataOutput.position.x = CurrPosXCollide - (hitbox.size.x - origin.x);
    }
    return dataOutput;
}
void GoombaAIBehavior::GoombaAICollision(MFCPP::Enemy* EnemyA, MFCPP::Enemy* EnemyB) {
    if (!EnemyB->isCollideEachOther() || !EnemyA->isCollideEachOther()) return;
    if (const sf::FloatRect r1 = getGlobalHitbox(EnemyA->getHitbox(), EnemyA->getCurrentPosition(), EnemyA->getOrigin()); isCollide(r1, getGlobalHitbox(EnemyB->getHitbox(), EnemyB->getCurrentPosition(), EnemyB->getOrigin()))) {
        if (EnemyA->getCurrentPosition().x > EnemyB->getCurrentPosition().x) {
            EnemyA->setDirection(true);
            EnemyB->setDirection(false);
        } else if (EnemyA->getCurrentPosition().x < EnemyB->getCurrentPosition().x) {
            EnemyA->setDirection(false);
            EnemyB->setDirection(true);
        }
    }
}

void GoombaAIBehavior::GoombaAIStomping() {
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) Yvelo = -8.0f;
    else Yvelo = -13.0f;
    SoundManager::PlaySound("Stomp");
}

bool GoombaAIBehavior::GoombaAIEffectDisappearing(float& clock, float& alpha, const float deltaTime) {
    if (clock >= 4.f * 50) {
        if (alpha > 0) {
            alpha -= 7.5f * deltaTime;
            return false;
        }
        return true;
    }
    clock += 1.f * deltaTime;
    return false;
}
GoombaAIBehavior::GoombaAIData GoombaAIBehavior::GoombaAIEffectYMove(const GoombaAIData& data, const float deltaTime) {
    GoombaAIData dataOutput = data;
    dataOutput.position += sf::Vector2f(0.f, data.velocity.y * deltaTime);
    dataOutput.velocity.y += 1.f * deltaTime * 0.15f;
    return dataOutput;
}
