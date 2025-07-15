#ifndef BULLETLAUNCHERCLASS_HPP
#define BULLETLAUNCHERCLASS_HPP
#include "ActiveObjectClass.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"

namespace MFCPP {
    class BulletLauncher final : public ActiveObject<float>, public StaticAnimationObject {
    public:
        BulletLauncher(float launch_interval, const sf::Vector2f& pos);
        ~BulletLauncher() = default;
        void setLaunchInterval(float val);
        [[nodiscard]] float getLaunchInterval() const;
        void setFirstShotTime(float val);
        [[nodiscard]] float getFirstShotTime() const;
        void setRandomFireInterval(int val);
        [[nodiscard]] int getRandomFireInterval() const;
        void setTiming(float val);
        [[nodiscard]] float getTiming() const;
        void setDisabled(bool val);
        [[nodiscard]] bool isDisabled() const;
        void setState(bool val);
        [[nodiscard]] bool getState() const;
    private:
        float m_launch_interval{};
        float m_first_shot_time{};

        int   m_random_fire_interval{};

        float m_timing{};
        bool  m_disabled{};
        bool  m_state{};
    };
}

#endif //BULLETLAUNCHERCLASS_HPP
