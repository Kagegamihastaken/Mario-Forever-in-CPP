#ifndef PIRANHAAICLASS_HPP
#define PIRANHAAICLASS_HPP

#include "Core/Animate/SingleAnimationObject.hpp"
#include "Class/ActiveObjectClass.hpp"
#include "Core/Loading/enum.hpp"

namespace MFCPP {
    class PiranhaAI final : public SingleAnimationObject, public ActiveObject {
    public:
        PiranhaAI() = default;
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
        void restartStopClock();
        [[nodiscard]] sf::Clock getStopClock() const;
        void setStop(bool stop);
        [[nodiscard]] bool getStop() const;
        void setDistanceAppear(float distance_appear);
        [[nodiscard]] float getDistanceAppear() const;

    private:
        PiranhaID     m_ID{};
        sf::FloatRect m_hitbox{};
        bool          m_Disabled{};
        float         m_speed{};
        float         m_position_limit{};
        float         m_position_temporary{};
        bool          m_state{};
        float         m_stop_time{};
        sf::Clock     m_stop_clock;
        bool          m_stop{};
        float         m_distance_appear{};
    };
}

#endif //PIRANHAAICLASS_HPP
