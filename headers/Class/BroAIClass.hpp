#ifndef BROAICLASS_HPP
#define BROAICLASS_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Class/ActiveObjectClass.hpp"

namespace MFCPP {
    class BroAI final : public SingleAnimationObject, public ActiveObject {
    public:
        BroAI(float speed, float stop_duration, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
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

    private:
        float         m_fixed_walking_value{};
        float         m_fixed_ticking_value{};
        float         m_movingValue{};
        float         m_random_offset_wv{};
        bool          m_direction{};
        float         m_time_ticking{};     // Total sum of m_ticking_value
        float         m_ticking_value{};
        int           m_state{};
        float         m_RNG_walking_range{};
        float         m_RNG_ticking_range{};
        float         m_Yvelo{};
        bool          m_isFalling{};
        float         m_lastY{};
        sf::Clock     m_JumpClock;

        sf::FloatRect m_hitbox{};
        float         m_stop_duration{};
        float         m_speed{};
    };
}

#endif //BROAICLASS_HPP
