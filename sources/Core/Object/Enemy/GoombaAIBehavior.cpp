#include "Core/Object/Enemy/GoombaAIBehavior.hpp"
#include <algorithm>

#include "Block/Brick.hpp"
#include "Block/LuckyBlock.hpp"
#include "Core/SoundManager.hpp"
#include "Core/Collision/Collide.hpp"
#include "Effect/ScoreEffect.hpp"
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
    GoombaAIData dataOutput = EnemyPlatformXCollision(data, hitbox, origin);
    float CurrPosXCollide = 0.f, CurrPosYCollide = 0.f;
    bool NoAdd = false;
    // Loop through obstacles
    auto [fst, snd] = QuickCheckOnlyObstaclesSideCollision(
        MFCPP::CollisionObject(dataOutput.position, origin, hitbox_wall),
        dataOutput.direction, CurrPosXCollide, CurrPosYCollide, NoAdd);
    const std::pair<bool, bool> BrickCollide = CheckCollisionSide(MFCPP::CollisionObject(dataOutput.position, origin, hitbox_wall), CurrPosXCollide, CurrPosYCollide, NoAdd, 1);
    const std::pair<bool, bool> LuckyCollide = CheckCollisionSide(MFCPP::CollisionObject(dataOutput.position, origin, hitbox_wall), CurrPosXCollide, CurrPosYCollide, NoAdd, 2);
    if (fst || BrickCollide.first || LuckyCollide.first)
        dataOutput = EnemyAdjustXCollision(dataOutput, hitbox, origin, CurrPosXCollide, true);
    if (snd || BrickCollide.second || LuckyCollide.second)
        dataOutput = EnemyAdjustXCollision(dataOutput, hitbox, origin, CurrPosXCollide, false);
    return dataOutput;
}

GoombaAIBehavior::GoombaAIData GoombaAIBehavior::ShellXCollision(const GoombaAIData& data, const sf::FloatRect& hitbox, const sf::FloatRect& hitbox_wall, const sf::Vector2f& origin) {
    GoombaAIData dataOutput = EnemyPlatformXCollision(data, hitbox, origin);
    float BrickCurrPosX = 0, BrickCurrPosY = 0, LuckyCurrPosX = 0, LuckyCurrPosY = 0, CurrPosXCollide = 0, CurrPosYCollide = 0;;
    bool NoAdd = false;
    const std::pair<bool, bool> BrickCollideRemove = CheckCollisionSide(
        MFCPP::CollisionObject(dataOutput.position, origin, hitbox_wall), BrickCurrPosX, BrickCurrPosY, NoAdd, 1);
    if (BrickCollideRemove.first || BrickCollideRemove.second) {
        BrickBreak(BrickCurrPosX, BrickCurrPosY);
    }
    const std::pair<bool, bool> LuckyCollideRemove = CheckCollisionSide(
        MFCPP::CollisionObject(dataOutput.position, origin, hitbox_wall), LuckyCurrPosX, LuckyCurrPosY, NoAdd, 2);
    if (LuckyCollideRemove.first || LuckyCollideRemove.second) {
        LuckyHit(LuckyCurrPosX, LuckyCurrPosY);
    }

    NoAdd = false;

    if (BrickCollideRemove.first || BrickCollideRemove.second) {
        CurrPosXCollide = BrickCurrPosX;
        CurrPosYCollide = BrickCurrPosY;
    }
    else if (LuckyCollideRemove.first || LuckyCollideRemove.second) {
        CurrPosXCollide = LuckyCurrPosX;
        CurrPosYCollide = LuckyCurrPosY;
    }
    const auto [fst, snd] = QuickCheckOnlyObstaclesSideCollision(
        MFCPP::CollisionObject(dataOutput.position, origin, hitbox_wall),
        dataOutput.direction, CurrPosXCollide, CurrPosYCollide, NoAdd);
    if (fst || BrickCollideRemove.first || LuckyCollideRemove.first)
        dataOutput = EnemyAdjustXCollision(dataOutput, hitbox, origin, CurrPosXCollide, true);
    if (snd || BrickCollideRemove.second || LuckyCollideRemove.second)
        dataOutput = EnemyAdjustXCollision(dataOutput, hitbox, origin, CurrPosXCollide, false);
    return dataOutput;
}

GoombaAIBehavior::GoombaAIData GoombaAIBehavior::GoombaAIEffectYMove(const GoombaAIData& data, const float deltaTime) {
    GoombaAIData dataOutput = data;
    dataOutput.position += sf::Vector2f(0.f, data.velocity.y * deltaTime);
    dataOutput.velocity.y += 1.f * deltaTime * 0.15f;
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

void GoombaAIBehavior::ShellMovingCollision(MFCPP::Enemy* Shell, MFCPP::Enemy* Other, unsigned short& val) {
    if (!Shell->isShellKicking() || !Shell->isShellBlocker()) return;
    if (const sf::FloatRect r1 = getGlobalHitbox(Shell->getHitbox(), Shell->getCurrentPosition(), Shell->getOrigin()); isCollide(r1, getGlobalHitbox(Other->getHitbox(), Other->getCurrentPosition(), Other->getOrigin()))) {
        if (!Other->isShellKicking()) {
            if (Other->isShellBlocker()) {
                SoundManager::PlaySound("Kick2");
                AddScoreEffect(SCORE_100, Shell->getCurrentPosition().x, Shell->getCurrentPosition().y - Shell->getOrigin().y);
                Shell->ShellHit();
            }
            return;
        }

        if (!Other->isShellBlocker()) {
            val = Kicking(Other->getCurrentPosition(), Other->getOrigin(), val);
            Other->ShellHit();
        }
        else if (Other->isShellBlocker()){
            SoundManager::PlaySound("Kick2");
            AddScoreEffect(SCORE_100, Shell->getCurrentPosition().x, Shell->getCurrentPosition().y - Shell->getOrigin().y);
            AddScoreEffect(SCORE_100, Other->getCurrentPosition().x, Other->getCurrentPosition().y - Other->getOrigin().y);
            Shell->ShellHit();
            Other->ShellHit();
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

GoombaAIBehavior::GoombaAIData GoombaAIBehavior::EnemyPlatformXCollision(const GoombaAIData &data, const sf::FloatRect &hitbox, const sf::Vector2f &origin) {
    GoombaAIData dataOutput = data;
    if (float PlatDistance; PlatformXCollision(MFCPP::CollisionObject(data.position, origin, hitbox), PlatDistance, data.velocity.y))
        dataOutput.position.x += PlatDistance;
    return dataOutput;
}

GoombaAIBehavior::GoombaAIData GoombaAIBehavior::EnemyAdjustXCollision(const GoombaAIData &data, const sf::FloatRect &hitbox, const sf::Vector2f &origin, float CurrPosXCollide, bool side) {
    GoombaAIData dataOutput = data;
    if (side) {
        dataOutput.direction = !data.direction;
        dataOutput.position.x = CurrPosXCollide + 32.0f + origin.x;
    }
    else {
        dataOutput.direction = !data.direction;
        dataOutput.position.x = CurrPosXCollide - (hitbox.size.x - origin.x);
    }
    return dataOutput;
}
short unsigned int GoombaAIBehavior::Kicking(const sf::Vector2f& pos, const sf::Vector2f& origin, const short unsigned int val) {
    short unsigned int output = val;
    switch (val) {
        case 0:
            SoundManager::PlaySound("Kick2");
            AddScoreEffect(SCORE_100, pos.x, pos.y - origin.y);
            output += 1;
            break;
        case 1:
            SoundManager::PlaySound("Kick3");
            AddScoreEffect(SCORE_200, pos.x, pos.y - origin.y);
            output += 1;
            break;
        case 2:
            SoundManager::PlaySound("Kick4");
            AddScoreEffect(SCORE_500, pos.x, pos.y - origin.y);
            output += 1;
            break;
        case 3:
            SoundManager::PlaySound("Kick5");
            AddScoreEffect(SCORE_1000, pos.x, pos.y - origin.y);
            output += 1;
            break;
        case 4:
            SoundManager::PlaySound("Kick6");
            AddScoreEffect(SCORE_2000, pos.x, pos.y - origin.y);
            output += 1;
            break;
        case 5:
            SoundManager::PlaySound("Kick7");
            AddScoreEffect(SCORE_5000, pos.x, pos.y - origin.y);
            output += 1;
            break;
        case 6:
            SoundManager::PlaySound("Kick8");
            AddScoreEffect(SCORE_1UP, pos.x, pos.y - origin.y);
            output = 0;
            break;
        default: ;
    }
    return output;
}

GoombaAIBehavior::GoombaAIData GoombaAIBehavior::MushroomAppearing(const GoombaAIData &data, float &appearY, const float appearYSpeed, const float appearYMax, bool& isAppearing, const bool isDisabled, const float deltaTime) {
    GoombaAIData dataOutput = data;
    if (isAppearing && !isDisabled) {
        appearY += appearYSpeed * deltaTime;
        if (appearY >= appearYMax) {
            dataOutput.position.y += appearY - appearYMax - appearYSpeed * deltaTime;
            isAppearing = false;
        }
        else
            dataOutput.position.y -= appearYSpeed * deltaTime;
    }
    return dataOutput;
}
