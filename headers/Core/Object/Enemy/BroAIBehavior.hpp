#ifndef MFCPP_BROAIBEHAVIOR_HPP
#define MFCPP_BROAIBEHAVIOR_HPP
#include "GoombaAIBehavior.hpp"

class BroAIBehavior {
public:
    struct BroAIData {
        sf::Vector2f position;
        float movingValue, timeTicking, tickingValue, speed, stop_duration, randomWalking;
        bool directionMoving;
        unsigned int state;
    };
    struct BroAIShootData {
        float launchTickingTime, launchInterval, launchIntervalTicking, launchRNG, launchWaitTime;
        int launchCount, launchCounting;
        float launchIntervalBetween, launchIntervalBetweenTicking;
    };
    struct BroAIShootingData {
        bool willFire, isFire;
    };
    static BroAIData BroAIXUpdate(const BroAIData& data, float tickingRNGRange, float walkingRNGRange, float deltaTime);
    static sf::Vector2f BroAIXCollision(const GoombaAIBehavior::GoombaAIData& data, const sf::FloatRect &hitbox, const sf::FloatRect& hitbox_wall, const sf::Vector2f &origin);
    static GoombaAIBehavior::GoombaAIData BroAIYCollision(const GoombaAIBehavior::GoombaAIData& data, const sf::FloatRect &hitbox, const sf::Vector2f &origin, float& timeCounter, bool& isFalling, float& lastY, float canJump, float deltaTime);
    static GoombaAIBehavior::GoombaAIData BroAIYUpdateMovement(const GoombaAIBehavior::GoombaAIData& data, float deltaTime);
    static BroAIShootingData ShootUpdate(const GoombaAIBehavior::GoombaAIData& data, BroAIShootData& shootData, bool willFire, float deltaTime);
    static const float m_fixed_walking_value;
    static const float m_fixed_ticking_value;
private:
    struct JumpingBehavior {
        bool isJump, isFall;
    };
    static bool SetWalkingValue(float randomWalkingValue, float& movingValue, int multiply, bool reverse);
    static JumpingBehavior BroAIYUpdateJump(float& timeCounter, const sf::Vector2f& position, bool& isFalling, float& lastY, bool canJump, float deltaTime);
};

#endif //MFCPP_BROAIBEHAVIOR_HPP