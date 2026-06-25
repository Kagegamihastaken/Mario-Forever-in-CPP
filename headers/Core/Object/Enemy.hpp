#ifndef MFCPP_ENEMY_HPP
#define MFCPP_ENEMY_HPP

#include <bitset>
#include <boost/intrusive/list.hpp>
#include "ActiveObject.hpp"

using EnemyIntrusiveHook = boost::intrusive::list_base_hook<boost::intrusive::link_mode<boost::intrusive::auto_unlink>>;
class EnemyManager;

namespace MFCPP {
    class Enemy : public EnemyIntrusiveHook {
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

        virtual void updatePreviousData() = 0;
        virtual void statusUpdate(float deltaTime) = 0;
        virtual void MarioCollision(float MarioYVelocity) = 0;
        virtual void XUpdate(float deltaTime) = 0;
        virtual void YUpdate(float deltaTime) = 0;
        virtual void EnemyCollision() = 0;
        virtual void Death(unsigned int state) = 0;
        virtual void BlockHit() = 0;
        virtual void ShellHit() = 0;
        virtual void draw(float alpha) = 0;
        virtual void animationUpdate(float deltaTime) = 0;

        [[nodiscard]] virtual sf::Vector2f getPosition() = 0;
        [[nodiscard]] virtual sf::Vector2f getOrigin() = 0;
        [[nodiscard]] virtual sf::FloatRect getHitbox() = 0;

        [[nodiscard]] virtual bool isDestroyed() = 0;
        [[nodiscard]] virtual bool isDeath() = 0;

        void setDirection(bool dir) { m_option[Direction] = dir; }
        [[nodiscard]] bool getDirection() const { return m_option[Direction]; }
        [[nodiscard]] bool isDisabled() const { return m_option[Disabled]; }
        [[nodiscard]] bool isCollideEachOther() const { return m_option[CollideEachOther]; }
        [[nodiscard]] bool isShellKicking() const { return m_option[ShellKicking]; }
        [[nodiscard]] bool isShellBlocker() const { return m_option[ShellBlocker]; }
        [[nodiscard]] int32_t getDrawingPriority() const {
            return m_option[DrawingPriorityLow] + (m_option[DrawingPriorityHigh] << 1);
        }
        virtual void Destroy() = 0;
        virtual ~Enemy() = default;
    protected:
        void setShellBlocker(bool val) { m_option[ShellBlocker] = val;}
        void setShellKicking(bool val) { m_option[ShellKicking] = val;}
        void setCollideEachOther(bool val) {m_option[CollideEachOther] = val;}
        void setDisabled(bool val) { m_option[Disabled] = val; }
        void setDrawingPriority(int32_t val) {
            const uint8_t num = val & 3;
            m_option[DrawingPriorityLow] = num & 1;
            m_option[DrawingPriorityHigh] = (num >> 1) & 1;
        }

        EnemyManager& m_enemyManager;
        std::bitset<7> m_option = 0;
    };
}

#endif //MFCPP_ENEMY_HPP