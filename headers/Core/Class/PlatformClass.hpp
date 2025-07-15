#ifndef PLATFORMCLASS_HPP
#define PLATFORMCLASS_HPP
#include "ActiveObjectClass.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"

namespace MFCPP {
    class Platform final : public ActiveObject<float>, public SingleAnimationObject {
    public:
        explicit Platform(const sf::Vector2f& start, const sf::Vector2f& end, float speed, const sf::FloatRect& hitbox, bool smooth = false, bool fall = false, bool wait = false);
        void setHitbox(const sf::FloatRect& val);
        [[nodiscard]] sf::FloatRect getHitbox() const;
        void setStartPoint(const sf::Vector2f& val);
        [[nodiscard]] sf::Vector2f getStartPoint() const;
        void setEndPoint(const sf::Vector2f& val);
        [[nodiscard]] sf::Vector2f getEndPoint() const;
        void setSpeed(float val);
        [[nodiscard]] float getSpeed() const;
        void setMovingToEnd(bool val);
        [[nodiscard]] bool isMovingToEnd() const;
        void setCurrentSpeed(float val);
        [[nodiscard]] float getCurrentSpeed() const;
        void setSmoothStore(float val);
        [[nodiscard]] float getSmoothStore() const;
        void setYVelo(float val);
        [[nodiscard]] float getYVelo() const;
        void setIsFall(bool val);
        [[nodiscard]] bool willFall() const;
        void setWaitSate(unsigned val);
        [[nodiscard]] unsigned getWaitState() const;
        //Only Get Property;
        [[nodiscard]] bool isSmooth() const;
        [[nodiscard]] bool isFall() const;
        [[nodiscard]] bool isWait() const;
    private:
        //General
        sf::FloatRect m_hitbox;
        sf::Vector2f  m_startPoint;
        sf::Vector2f  m_endPoint;
        float         m_speed;
        bool          m_movingToEnd;
        //Smooth Movement
        float         m_currentSpeed;
        float         m_smoothStore;
        //Falling
        float         m_Yvelo;
        bool          m_isFall;
        //Wait
        unsigned      m_waitState;

        //Property Platform
        bool          m_smooth;
        bool          m_fall;
        bool          m_wait;
    };
}

#endif //PLATFORMCLASS_HPP
