#ifndef PIRANHAAICLASS_HPP
#define PIRANHAAICLASS_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Class/ActiveObjectClass.hpp"
#include "Core/Loading/enum.hpp"

namespace MFCPP {
    class PiranhaAI final : public SingleAnimationObject, public ActiveObject<float> {
    public:
        ~PiranhaAI() = default;
        explicit PiranhaAI(PiranhaID id, PiranhaDirection dir, float speed, float stop_time, unsigned fire_count, float fire_interval, const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
        void setID(PiranhaID ID);
        [[nodiscard]] PiranhaID getID() const;
        void setHitbox(const sf::FloatRect& hitbox);
        [[nodiscard]] sf::FloatRect getHitbox() const;
        void setDisabled(bool disabled);
        [[nodiscard]] bool isDisabled() const;
        void setSpeed(float speed);
        [[nodiscard]] float getSpeed() const;
        void setPositionLimit(float position_limit);
        [[nodiscard]] float getPositionLimit() const;
        void setPositionTemporary(float position_temporary);
        [[nodiscard]] float getPositionTemporary() const;
        void setState(bool state);
        [[nodiscard]] bool getState() const;
        void setStopTime(float stop_time);
        [[nodiscard]] float getStopTime() const;
        void setStopClock(float val);
        [[nodiscard]] float getStopClock() const;
        void setStop(bool stop);
        [[nodiscard]] bool getStop() const;
        void setDistanceAppear(float distance_appear);
        [[nodiscard]] float getDistanceAppear() const;
        [[nodiscard]] PiranhaDirection getDirection() const;
        void setFireCount(unsigned val);
        [[nodiscard]] unsigned getFireCount() const;
        void setFireCounting(unsigned val);
        [[nodiscard]] unsigned getFireCounting() const;
        void setFireInterval(float val);
        [[nodiscard]] float getFireInterval() const;
        void setFireTicking(float val);
        [[nodiscard]] float getFireTicking() const;
    private:
        PiranhaID        m_ID{};
        PiranhaDirection m_direction{};
        sf::FloatRect    m_hitbox{};
        bool             m_Disabled{};
        float            m_speed{};
        float            m_position_limit{};
        float            m_position_temporary{};
        bool             m_state{};
        float            m_stop_time{};
        float            m_stop_clock;
        //sf::Clock        m_stop_clock;
        bool             m_stop{};
        float            m_distance_appear{};

        unsigned         m_fire_count{};
        unsigned         m_fire_counting{};
        float            m_fire_interval{};
        float            m_fire_ticking{};
    };
}

#endif //PIRANHAAICLASS_HPP
