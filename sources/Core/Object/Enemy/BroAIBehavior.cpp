#include "Core/Object/Enemy/BroAIBehavior.hpp"

#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Collision/Collide.hpp"

const float BroAIBehavior::m_fixed_walking_value = 32.f;
const float BroAIBehavior::m_fixed_ticking_value = 1.f;

bool BroAIBehavior::SetWalkingValue(const float randomWalkingValue, float& movingValue, const int multiply, const bool reverse) {
    const float value = multiply * (randomWalkingValue + m_fixed_walking_value) * (reverse ? -1.f : 1.f);
    movingValue = f_abs(value);
    if (value >= 0.f) return false;
    return true;
}

BroAIBehavior::BroAIData BroAIBehavior::BroAIXUpdate(const BroAIData& data, const float tickingRNGRange, const float walkingRNGRange, const float deltaTime) {
    BroAIData dataOutput = data;
    if (dataOutput.movingValue <= 0.f) {
        if (dataOutput.movingValue < 0.f) dataOutput.movingValue = 0.f;
        dataOutput.timeTicking += dataOutput.tickingValue * deltaTime;
    }
    else {
        dataOutput.position.x += dataOutput.speed * deltaTime * (!dataOutput.directionMoving ? 1.f : -1.f);
        dataOutput.movingValue -= 1.f * deltaTime;
    }
    switch (dataOutput.state) {
        case 1:
            dataOutput.directionMoving = SetWalkingValue(dataOutput.randomWalking, dataOutput.movingValue, 1, true);
            dataOutput.timeTicking = 0.f;
            dataOutput.state = 2;
            break;
        case 2:
            if (dataOutput.movingValue <= 0.f) {
                dataOutput.movingValue = 0.f;
                dataOutput.state = 3;
            }
            break;
        case 3:
            if (dataOutput.timeTicking > dataOutput.stop_duration) {
                dataOutput.timeTicking = 0.f;
                dataOutput.directionMoving = SetWalkingValue(dataOutput.randomWalking, dataOutput.movingValue, 2, false);
                dataOutput.state = 4;
            }
            break;
        case 4:
            if (dataOutput.movingValue <= 0.f) {
                dataOutput.movingValue = 0.f;
                dataOutput.state = 5;
            }
            break;
        case 5:
            if (dataOutput.timeTicking > dataOutput.stop_duration) {
                dataOutput.timeTicking = 0.f;
                dataOutput.directionMoving = SetWalkingValue(dataOutput.randomWalking, dataOutput.movingValue, 1, true);
                dataOutput.state = 6;
            }
            break;
        case 6:
            if (dataOutput.movingValue <= 0.f) {
                dataOutput.movingValue = 0.f;
                dataOutput.state = 7;
            }
            break;
        case 7:
            dataOutput.tickingValue = m_fixed_ticking_value + static_cast<float>(RandomIntNumberGenerator(0, static_cast<int>(tickingRNGRange-1.f)));
            dataOutput.randomWalking = static_cast<float>(RandomIntNumberGenerator(0, static_cast<int>(walkingRNGRange-1.f))) * -1.f;
            dataOutput.state = 1;
            break;
        default:;
    }
    return dataOutput;
}

sf::Vector2f BroAIBehavior::BroAIXCollision(const GoombaAIBehavior::GoombaAIData& data, const sf::FloatRect &hitbox, const sf::FloatRect& hitbox_wall, const sf::Vector2f &origin) {
    GoombaAIBehavior::GoombaAIData dataOutput = GoombaAIBehavior::GoombaAIData(GoombaAIBehavior::EnemyPlatformXCollision(GoombaAIBehavior::GoombaAIData(data.position, sf::Vector2f(0.f, data.velocity.y), data.direction), hitbox, origin), data.velocity, data.direction);
    float CurrPosXCollide = 0.f, CurrPosYCollide = 0.f;
    bool NoAdd = false;
    const auto [fst, snd] = QuickCheckOnlyObstaclesSideCollision(
    MFCPP::CollisionObject(dataOutput.position, origin,
        hitbox_wall), dataOutput.direction, CurrPosXCollide, CurrPosYCollide, NoAdd);

    if (fst)
        dataOutput = GoombaAIBehavior::EnemyAdjustXCollision(dataOutput, hitbox, origin, CurrPosXCollide, true);
    if (snd) {
        dataOutput = GoombaAIBehavior::EnemyAdjustXCollision(dataOutput, hitbox, origin, CurrPosXCollide, false);
    }
    return dataOutput.position;
}
BroAIBehavior::JumpingBehavior BroAIBehavior::BroAIYUpdateJump(float& timeCounter, const sf::Vector2f& position, bool& isFalling, float& lastY, const bool canJump, const float deltaTime) {
    JumpingBehavior output = {false, false};
    if (canJump) {
        if (timeCounter >= 0.2f * 50) {
            timeCounter = 0.f;
            switch (RandomIntNumberGenerator(0, 19)) {
                case 1:
                    output.isFall = true;
                    break;
                case 2:
                    output.isJump = true;
                    break;
                default: break;
            }
        }
        else timeCounter += deltaTime;
    }
    if (isFalling && position.y > lastY + 48.f) {
        lastY = -1.f;
        isFalling = false;
    }
    return output;
}
GoombaAIBehavior::GoombaAIData BroAIBehavior::BroAIYUpdateMovement(const GoombaAIBehavior::GoombaAIData& data, const float deltaTime) {
    GoombaAIBehavior::GoombaAIData dataOutput = data;
    dataOutput.position.y += dataOutput.velocity.y * deltaTime;
    dataOutput.velocity.y += (dataOutput.velocity.y >= 10.f ? 0.f : 0.175f * deltaTime);
    return dataOutput;
}
GoombaAIBehavior::GoombaAIData BroAIBehavior::BroAIYCollision(const GoombaAIBehavior::GoombaAIData& data, const sf::FloatRect &hitbox, const sf::Vector2f &origin, float& timeCounter, bool& isFalling, float& lastY, const float canJump, const float deltaTime) {
    GoombaAIBehavior::GoombaAIData dataOutput = data;
    float CurrPosYCollide = 0.f, CurrPosXCollide = 0.f;
    JumpingBehavior jumpingBehavior = BroAIYUpdateJump(timeCounter, dataOutput.position, isFalling, lastY, canJump, deltaTime);
    if (float PlatPosY; PlatformYCollision(MFCPP::CollisionObject(dataOutput.position, origin, hitbox), PlatPosY, data.velocity.y, false)) {
        dataOutput.position.y = PlatPosY;
        dataOutput.velocity.y = 0.f;
    }
    bool NoAdd = false;
    if (QuickCheckOnlyObstacleBotCollision(MFCPP::CollisionObject(dataOutput.position, origin, hitbox), CurrPosXCollide, CurrPosYCollide, NoAdd)) {
        if (const float offset = std::min(dataOutput.velocity.x + 1.f, 3.f); dataOutput.velocity.y >= -dataOutput.velocity.x) {
            const float floorY = GetCurrFloorY(dataOutput.position, CurrPosXCollide, CurrPosYCollide);
            if (dataOutput.position.y >= CurrPosYCollide + floorY - offset) {
                dataOutput.velocity.y = (jumpingBehavior.isJump ? -8.f : 0.f);
                dataOutput.position.y = CurrPosYCollide + floorY - hitbox.position.y - (hitbox.size.y - origin.y);
            }
        }
    }
    else {
        if (QuickCheckOnlyHittableBotCollision(MFCPP::CollisionObject(dataOutput.position, origin, hitbox), CurrPosXCollide, CurrPosYCollide, NoAdd) && dataOutput.velocity.y > 0.f) {
            if (!isFalling) {
                if (dataOutput.velocity.y >= 0.0f) {
                    dataOutput.velocity.y = 0.f;
                    dataOutput.position.y = CurrPosYCollide - hitbox.position.y - (hitbox.size.y - origin.y);
                    if (jumpingBehavior.isJump) dataOutput.velocity.y = -8.f;
                    else if (jumpingBehavior.isFall) {
                        isFalling = true;
                        lastY = dataOutput.position.y;
                        dataOutput.velocity.y = -3.f;
                    }
                }
            }
        }
    }
    NoAdd = false;
    if (QuickCheckOnlyObstacleTopCollision(MFCPP::CollisionObject(dataOutput.position, origin, hitbox), CurrPosXCollide, CurrPosYCollide, NoAdd)) {
        dataOutput.velocity.y = 0.f;
        dataOutput.position.y = CurrPosYCollide - hitbox.position.y + (32.0f + origin.y);
    }
    return dataOutput;
}
BroAIBehavior::BroAIShootingData BroAIBehavior::ShootUpdate(const GoombaAIBehavior::GoombaAIData& data, BroAIShootData& shootData, bool willFire, const float deltaTime) {
    BroAIShootingData Output(willFire, false); // Animation purpose only
                                                     // isFire always reset no matter what for 1 times only
    if (shootData.launchTickingTime == 0.f) {
        shootData.launchIntervalTicking += deltaTime;
        if (shootData.launchIntervalTicking >= shootData.launchInterval) {
            if (RandomIntNumberGenerator(0, static_cast<int>(shootData.launchRNG)) == 1 && !isOutScreen(
                        data.position.x, data.position.y, 32.f, 32.f)) {
                shootData.launchTickingTime += deltaTime;
                Output.willFire = true;
            }
            shootData.launchIntervalTicking = 0.f;
        }
    }
    else if (shootData.launchTickingTime > 0.f) {
        if (shootData.launchTickingTime <= shootData.launchWaitTime)
            shootData.launchTickingTime += deltaTime;
        else {
            if (shootData.launchCounting > 0) {
                if (shootData.launchIntervalBetweenTicking < shootData.launchIntervalBetween)
                    shootData.launchIntervalBetweenTicking += deltaTime;
                else {
                    Output.isFire = true;
                    shootData.launchIntervalBetweenTicking = 0.f;
                    --shootData.launchCounting;
                }
            }
            else {
                shootData.launchIntervalBetweenTicking = shootData.launchIntervalBetween;
                shootData.launchTickingTime = 0.f;
                shootData.launchCounting = shootData.launchCount;
                Output.willFire = false;
            }
        }
    }
    return Output;
}
