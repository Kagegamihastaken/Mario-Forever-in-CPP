#include "Core/Object/Enemy/Behavior/GoombaAIBehavior.hpp"
#include <algorithm>

#include "Core/SoundManager.hpp"
#include "Core/Tilemap.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Object/Mario.hpp"
#include "SFML/Graphics/Rect.hpp"

auto GoombaAIBehavior::GoombaAIYMove(const GoombaAIData& data, const float deltaTime) -> GoombaAIData {
    auto dataOutput = data;
    dataOutput.position += sf::Vector2f(0.f, data.velocity.y * deltaTime);
    dataOutput.velocity.y += (data.velocity.y >= 10.0f ? 0.0f : 1.f * deltaTime * 0.3f);
    return dataOutput;
}
auto GoombaAIBehavior::GoombaAIYCollision(const GoombaAIData& data, const sf::FloatRect& hitbox, const sf::Vector2f& origin, const bool RedKoopaAIEnabled, const float prevX) -> GoombaAIData {
    auto dataOutput = data;
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
    const float offset = std::min(data.velocity.x + 1.f, 3.f);
    if (QuickCheckBotCollision(MFCPP::CollisionObject(data.position, origin, hitbox), offset, CurrPosXCollide, CurrPosYCollide)) {
        if (RedKoopaAIEnabled) {
            if (!isAccurateCollideBotStopEdge(MFCPP::CollisionObject({data.position.x, data.position.y + 1.f}, origin, hitbox), !data.direction)) {
                dataOutput.position.x = prevX;
                dataOutput.direction = !data.direction;
            }
        }

        if (data.velocity.y >= -data.velocity.x) {
            const float floorY = GetCurrFloorY(data.position, CurrPosXCollide, CurrPosYCollide);
            if (data.position.y < CurrPosYCollide + floorY - offset) return dataOutput;
            dataOutput.velocity.y = 0.f;
            dataOutput.position.y = CurrPosYCollide + floorY - (hitbox.size.y - origin.y);
            return dataOutput;
        }
    }
    if (QuickCheckTopCollision(MFCPP::CollisionObject(data.position, origin, hitbox), CurrPosXCollide, CurrPosYCollide)) {
        float ceilY = (MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide, CurrPosYCollide) == 3 ? 32.0f : MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide, CurrPosYCollide).second);
        dataOutput.velocity.y = 0.f;
        dataOutput.position.y = CurrPosYCollide + (ceilY + origin.y);
    }
    return dataOutput;
}

auto GoombaAIBehavior::GoombaAIXMove(const GoombaAIData &data, const float deltaTime) -> GoombaAIData {
    auto dataOutput = data;
    dataOutput.position.x += data.velocity.x * (data.direction ? 1.f : -1.f) * deltaTime;
    return dataOutput;
}

auto GoombaAIBehavior::GoombaAIXCollision(const GoombaAIData &data, const sf::FloatRect &hitbox, const sf::FloatRect& hitbox_wall, const sf::Vector2f &origin) -> GoombaAIData {
    auto dataOutput = GoombaAIData(EnemyPlatformXCollision(data, hitbox, origin), data.velocity, data.direction);
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

auto GoombaAIBehavior::ShellXCollision(const GoombaAIData& data, const sf::FloatRect& hitbox, const sf::FloatRect& hitbox_wall, const sf::Vector2f& origin) -> GoombaAIData{
    auto dataOutput = GoombaAIData(EnemyPlatformXCollision(data, hitbox, origin), data.velocity, data.direction);
    float CurrPosXCollide = 0, CurrPosYCollide = 0;;
    for (auto& bonus : GameScene::customTileManager.getBonusList()) {
        if (const sf::FloatRect BonusHitbox = getGlobalHitbox(bonus.getHitbox(), bonus.getPosition(), bonus.getOrigin()); isCollide(BonusHitbox, getGlobalHitbox(hitbox, dataOutput.position, origin)))
            bonus.KickEvent();
    }

    const auto [fst, snd] = QuickCheckSideCollision(MFCPP::CollisionObject(dataOutput.position, origin, hitbox_wall),
                                                    dataOutput.direction, CurrPosXCollide, CurrPosYCollide);
    if (fst)
        dataOutput = EnemyAdjustXCollision(dataOutput, hitbox, origin, CurrPosXCollide, true);
    if (snd)
        dataOutput = EnemyAdjustXCollision(dataOutput, hitbox, origin, CurrPosXCollide, false);
    return dataOutput;
}

auto GoombaAIBehavior::GoombaAIEffectYMove(const GoombaAIData& data, const float deltaTime) -> GoombaAIData {
    auto dataOutput = data;
    dataOutput.position += sf::Vector2f(0.f, data.velocity.y * deltaTime);
    dataOutput.velocity.y += 1.f * deltaTime * 0.15f;
    return dataOutput;
}
auto GoombaAIBehavior::BulletBillEffectYMove(const GoombaAIData& data, const float deltaTime) -> GoombaAIData {
    auto dataOutput = data;
    dataOutput.position += sf::Vector2f(0.f, data.velocity.y * deltaTime);
    dataOutput.velocity.y += 1.f * deltaTime * 0.3f;
    return dataOutput;
}
auto GoombaAIBehavior::GoombaAICollision(MFCPP::Enemy* EnemyA, MFCPP::Enemy* EnemyB) -> void {
    if (!EnemyB->isCollideEachOther() || !EnemyA->isCollideEachOther()) return;
    if (const sf::FloatRect r1 = getGlobalHitbox(EnemyA->getHitbox(), EnemyA->getPosition(), EnemyA->getOrigin()); isCollide(r1, getGlobalHitbox(EnemyB->getHitbox(), EnemyB->getPosition(), EnemyB->getOrigin()))) {
        if (EnemyA->getPosition().x > EnemyB->getPosition().x) {
            EnemyA->setDirection(true);
            EnemyB->setDirection(false);
        } else if (EnemyA->getPosition().x < EnemyB->getPosition().x) {
            EnemyA->setDirection(false);
            EnemyB->setDirection(true);
        }
    }
}

auto GoombaAIBehavior::ShellMovingCollision(MFCPP::Enemy* Shell, MFCPP::Enemy* Other, unsigned short& val) -> void {
    if (!Shell->isShellKicking() || !Shell->isShellBlocker()) return;
    if (const sf::FloatRect r1 = getGlobalHitbox(Shell->getHitbox(), Shell->getPosition(), Shell->getOrigin()); isCollide(r1, getGlobalHitbox(Other->getHitbox(), Other->getPosition(), Other->getOrigin()))) {
        if (!Other->isShellKicking()) {
            if (Other->isShellBlocker()) {
                SoundManager::PlaySound("Kick2");
                AddScoreEffect(ScoreID::SCORE_100, Shell->getPosition().x, Shell->getPosition().y - Shell->getOrigin().y);
                Shell->ShellHit();
            }
            return;
        }

        if (!Other->isShellBlocker()) {
            val = Kicking(Other->getPosition(), Other->getOrigin(), val);
            Other->ShellHit();
        }
        else if (Other->isShellBlocker()){
            SoundManager::PlaySound("Kick2");
            AddScoreEffect(ScoreID::SCORE_100, Shell->getPosition().x, Shell->getPosition().y - Shell->getOrigin().y);
            AddScoreEffect(ScoreID::SCORE_100, Other->getPosition().x, Other->getPosition().y - Other->getOrigin().y);
            Shell->ShellHit();
            Other->ShellHit();
        }
    }
}
auto GoombaAIBehavior::GoombaAIStomping() -> void {
    if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) Mario::setYvelocity(-8.f);
    else Mario::setYvelocity(-13.f);
    SoundManager::PlaySound("Stomp");
    if (!Mario::getHolding()) Mario::setHolding(true);
}

auto GoombaAIBehavior::GoombaAIEffectDisappearing(float& clock, float& alpha, const float deltaTime) -> bool {
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

auto GoombaAIBehavior::EnemyPlatformXCollision(const GoombaAIData &data, const sf::FloatRect &hitbox, const sf::Vector2f &origin) -> sf::Vector2f {
    auto dataOutput = data;
    if (float PlatDistance; PlatformXCollision(MFCPP::CollisionObject(data.position, origin, hitbox), PlatDistance, data.velocity.y))
        dataOutput.position.x += PlatDistance;
    return dataOutput.position;
}

auto GoombaAIBehavior::EnemyAdjustXCollision(const GoombaAIData &data, const sf::FloatRect &hitbox, const sf::Vector2f &origin, float CurrPosXCollide, bool side) -> GoombaAIData {
    auto dataOutput = data;
    if (side) {
        dataOutput.direction = !data.direction;
        dataOutput.position.x = CurrPosXCollide + 32.0f - hitbox.position.x + origin.x;
    }
    else {
        dataOutput.direction = !data.direction;
        dataOutput.position.x = CurrPosXCollide - (hitbox.position.x + hitbox.size.x - origin.x);
    }
    return dataOutput;
}
auto GoombaAIBehavior::Kicking(const sf::Vector2f& pos, const sf::Vector2f& origin, const uint8_t val) -> uint8_t {
    auto output = val;
    switch (val) {
        case 0:
            SoundManager::PlaySound("Kick2");
            AddScoreEffect(ScoreID::SCORE_100, pos.x, pos.y - origin.y);
            output += 1;
            break;
        case 1:
            SoundManager::PlaySound("Kick3");
            AddScoreEffect(ScoreID::SCORE_200, pos.x, pos.y - origin.y);
            output += 1;
            break;
        case 2:
            SoundManager::PlaySound("Kick4");
            AddScoreEffect(ScoreID::SCORE_500, pos.x, pos.y - origin.y);
            output += 1;
            break;
        case 3:
            SoundManager::PlaySound("Kick5");
            AddScoreEffect(ScoreID::SCORE_1000, pos.x, pos.y - origin.y);
            output += 1;
            break;
        case 4:
            SoundManager::PlaySound("Kick6");
            AddScoreEffect(ScoreID::SCORE_2000, pos.x, pos.y - origin.y);
            output += 1;
            break;
        case 5:
            SoundManager::PlaySound("Kick7");
            AddScoreEffect(ScoreID::SCORE_5000, pos.x, pos.y - origin.y);
            output += 1;
            break;
        case 6:
            SoundManager::PlaySound("Kick8");
            AddScoreEffect(ScoreID::SCORE_1UP, pos.x, pos.y - origin.y);
            output = 0;
            break;
        default: ;
    }
    return output;
}

auto GoombaAIBehavior::MushroomAppearing(const GoombaAIData &data, float &appearY, const float appearYSpeed, const float appearYMax, bool& isAppearing, const bool isDisabled, const float deltaTime) -> GoombaAIData {
    auto dataOutput = data;
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
