#ifndef MFCPP_ENEMY_HPP
#define MFCPP_ENEMY_HPP

#include <bitset>
#include <boost/intrusive/list.hpp>
#include "Core/Class/ActiveObjectClass.hpp"

using EnemyIntrusiveHook = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
class EnemyManager;

namespace MFCPP {
    class Enemy : public EnemyIntrusiveHook, public ActiveObject<float> {
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
        [[nodiscard]] virtual bool isDeath() = 0;

        void setDirection(const bool dir) { m_option[0] = dir; }
        [[nodiscard]] bool getDirection() const { return m_option[0]; }
        void setDisabled(const bool val) { m_option[1] = val; }
        [[nodiscard]] bool isDisabled() const { return m_option[1]; }
        void setCollideEachOther(const bool val) {m_option[2] = val;}
        [[nodiscard]] bool isCollideEachOther() const { return m_option[2]; }
        void setShellKicking(const bool val) { m_option[3] = val; }
        [[nodiscard]] bool isShellKicking() const { return m_option[3]; }
        void setShellBlocker(const bool val) { m_option[4] = val;}
        [[nodiscard]] bool isShellBlocker() const { return m_option[4]; }
        void setDrawingPriority(const int val) {
            m_option[5] = val & 1;
            m_option[6] = (val >> 1) & 1;
        }
        [[nodiscard]] int getDrawingPriority() const {
            return m_option[5] + (m_option[6] << 1);
        }

        virtual void Destroy() = 0;
        virtual ~Enemy() = default;
    protected:
        EnemyManager& m_enemyManager;
        std::bitset<7> m_option = 0;
        /*
         * Note:
         * Bit 1: Direction
         * Bit 2: Disabled
         * Bit 3: Collide Each Other
         * Bit 4: Shell Kicking (Mean enemy die from shell)
         * Bit 5: Shell Blocker
         * Bit 6: Drawing Lower Priority
         */
    };
}

#endif //MFCPP_ENEMY_HPP