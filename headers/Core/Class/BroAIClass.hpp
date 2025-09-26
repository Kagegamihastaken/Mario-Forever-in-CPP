#ifndef BROAICLASS_HPP
#define BROAICLASS_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Class/ActiveObjectClass.hpp"
#include "Core/Loading/enum.hpp"

namespace MFCPP {
    class BroAI final : public SingleAnimationObject, public ActiveObject<float> {
    public:
        explicit BroAI(BroAIType type, BroAIMovementType movementType, float speed, float stop_duration, float launch_interval, int launch_count, float launch_interval_between, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
        ~BroAI() = default;
        void setMovingValue(float val);
        [[nodiscard]] float getMovingValue() const;
        void setSpeed(float val);
        [[nodiscard]] float getSpeed() const;
        void setState(int val);
        [[nodiscard]] int getState() const;
        [[nodiscard]] float getFixedWalkingValue() const;
        void setRandomWalking(float val);
        [[nodiscard]] float getRandomWalking() const;
        void setDirectionWalking(bool val);
        [[nodiscard]] bool getDirectionWalking() const;
        void setStopDuration(float val);
        [[nodiscard]] float getStopDuration() const;
        void setTimeTicking(float val);
        [[nodiscard]] float getTimeTicking() const;
        void setTickingValue(float val);
        [[nodiscard]] float getTickingValue() const;
        [[nodiscard]] float getFixedTickingValue() const;
        [[nodiscard]] float getWalkingRNGRange() const;
        [[nodiscard]] float getTickingRNGRange() const;
        void setHitbox(const sf::FloatRect& hitbox);
        [[nodiscard]] sf::FloatRect getHitbox() const;
        void setYVelo(float val);
        [[nodiscard]] float getYVelo() const;
        void restartJumpClock();
        [[nodiscard]] sf::Clock getJumpClock() const;
        void setFalling(bool val);
        [[nodiscard]] bool isFalling() const;
        void setLastY(float val);
        [[nodiscard]] float getLastY() const;
        void setMovementType(BroAIMovementType val);
        [[nodiscard]] BroAIMovementType getMovementType() const;
        void setLaunchInterval(float val);
        [[nodiscard]] float getLaunchInterval() const;
        [[nodiscard]] float getLaunchRNG() const;
        void setLaunchWaitTime(float val);
        [[nodiscard]] float getLaunchWaitTime() const;
        void setLaunchIntervalTicking(float val);
        [[nodiscard]] float getLaunchIntervalTicking() const;
        void setLaunchTickingTime(float val);
        [[nodiscard]] float getLaunchTickingTime() const;
        void setWallHitbox(const sf::FloatRect& hitbox);
        [[nodiscard]] sf::FloatRect getWallHitbox() const;
        void setLaunchCount(int val);
        [[nodiscard]] int getLaunchCount() const;
        void setLaunchCounting(int val);
        [[nodiscard]] int getLaunchCounting() const;
        void setLaunchIntervalBetween(float val);
        [[nodiscard]] float getLaunchIntervalBetween() const;
        void setLaunchIBTicking(float val);
        [[nodiscard]] float getLaunchIBTicking() const;
        void setDisabled(bool val);
        [[nodiscard]] bool isDisabled() const;
        void setType(BroAIType val);
        [[nodiscard]] BroAIType getType() const;
        void DeathBehaviour(ScoreID score_id) const;

    private:
        float             m_fixed_walking_value{};
        float             m_fixed_ticking_value{};
        float             m_movingValue{};
        float             m_random_offset_wv{};
        bool              m_direction{};
        float             m_time_ticking{};     // Total sum of m_ticking_value
        float             m_ticking_value{};
        int               m_state{};
        float             m_RNG_walking_range{};
        float             m_RNG_ticking_range{};
        float             m_Yvelo{};
        bool              m_isFalling{};
        float             m_lastY{};
        sf::Clock         m_JumpClock{};
        bool              m_disabled{};

        float             m_launchRNG{};
        float             m_launchWaitTime{};
        float             m_launch_interval_ticking{};
        float             m_launchTickingTime{};

        int               m_launch_count{};
        int               m_launch_counting{};
        float             m_launch_interval_between{};
        float             m_launch_interval_bticking{};

        BroAIMovementType m_movementType{};
        sf::FloatRect     m_hitbox{};
        sf::FloatRect     m_wall_hitbox{};
        float             m_stop_duration{};
        float             m_speed{};
        float             m_launch_interval{};
        BroAIType         m_type{};
    };
}

#endif //BROAICLASS_HPP
