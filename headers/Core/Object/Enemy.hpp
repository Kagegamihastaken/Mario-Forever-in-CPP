#ifndef MFCPP_ENEMY_HPP
#define MFCPP_ENEMY_HPP

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

        virtual void draw() = 0;

        void setDirection(const bool dir) { m_direction = dir; }
        [[nodiscard]] bool getDirection() const { return m_direction; }
        void setDisabled(const bool val) { m_disabled = val; }
        [[nodiscard]] bool isDisabled() const { return m_disabled; }
        void setCollideEachOther(const bool val) {m_collide_each_other = val;}
        [[nodiscard]] bool isCollideEachOther() const { return m_collide_each_other; }

        virtual void Destroy() = 0;
        virtual ~Enemy() = default;
    protected:
        EnemyManager& m_enemyManager;
        bool m_direction = true;
        bool m_disabled = false;
        bool m_collide_each_other = false;
    };
}

#endif //MFCPP_ENEMY_HPP