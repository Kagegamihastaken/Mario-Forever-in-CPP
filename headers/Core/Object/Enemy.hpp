#ifndef MFCPP_ENEMY_HPP
#define MFCPP_ENEMY_HPP

#include <bitset>
#include <boost/intrusive/list.hpp>
#include "Core/Class/ActiveObjectClass.hpp"

using EnemyIntrusiveHook = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
class EnemyManager;

namespace MFCPP {
    class Enemy : public EnemyIntrusiveHook, public ActiveObject<float> {
        enum Flag : uint8_t {
            Direction = 0,
            Disabled = 1,
            CollideEachOther = 2,
            ShellKicking = 3,
            ShellBlocker = 4,
            DrawingPriorityLow = 5,
            DrawingPriorityHigh = 6
        };
    public:
        explicit Enemy(EnemyManager& manager) : m_enemyManager(manager) {};

        virtual void setPreviousData() = 0;
        virtual void interpolateData(float alpha) = 0;
        virtual void statusUpdate(float deltaTime) = 0;
        virtual void MarioCollision(float MarioYVelocity) = 0;
        virtual void XUpdate(float deltaTime) = 0;
        virtual void YUpdate(float deltaTime) = 0;
        virtual void EnemyCollision() = 0;
        virtual void Death(unsigned int state) = 0;
        virtual void BlockHit() = 0;
        virtual void ShellHit() = 0;
        virtual void draw() = 0;
        virtual void animationUpdate(float deltaTime) = 0;
        [[nodiscard]] virtual bool isDeath() = 0;

        void setDirection(bool dir) { m_option[Direction] = dir; }
        [[nodiscard]] bool getDirection() const { return m_option[Direction]; }
        void setDisabled(bool val) { m_option[Disabled] = val; }
        [[nodiscard]] bool isDisabled() const { return m_option[Disabled]; }
        void setCollideEachOther(bool val) {m_option[CollideEachOther] = val;}
        [[nodiscard]] bool isCollideEachOther() const { return m_option[CollideEachOther]; }
        void setShellKicking(bool val) { m_option[ShellKicking] = val; }
        [[nodiscard]] bool isShellKicking() const { return m_option[ShellKicking]; }
        void setShellBlocker(bool val) { m_option[ShellBlocker] = val;}
        [[nodiscard]] bool isShellBlocker() const { return m_option[ShellBlocker]; }
        void setDrawingPriority(int val) {
            m_option[DrawingPriorityLow] = val & 1;
            m_option[DrawingPriorityHigh] = (val >> 1) & 1;
        }
        [[nodiscard]] int getDrawingPriority() const {
            return m_option[DrawingPriorityLow] + (m_option[DrawingPriorityHigh] << 1);
        }
        virtual void Destroy() = 0;
        virtual ~Enemy() = default;
    protected:
        EnemyManager& m_enemyManager;
        std::bitset<7> m_option = 0;
    };
}

#endif //MFCPP_ENEMY_HPP