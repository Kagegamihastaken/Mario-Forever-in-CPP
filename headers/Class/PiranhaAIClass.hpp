#ifndef PIRANHAAICLASS_HPP
#define PIRANHAAICLASS_HPP

#include <SFML/Graphics.hpp>
#include "Core/Animate/LocalAnimationManager.hpp"
#include "Core/Loading/enum.hpp"

class PiranhaAI : public sf::Drawable, public sf::Transformable {
public:
    LocalAnimationManager m_animation;
    PiranhaAI(const sf::Texture& texture, const sf::IntRect& rect);
    PiranhaAI(const sf::Texture& texture, const sf::Vector2f& position);
    PiranhaAI(const sf::Texture& texture, const sf::Vector2f& position, const sf::IntRect& rect);
    PiranhaAI(const sf::Vector2f& position);
    void setTextureRect(const sf::IntRect& rect, bool resetSize = false);
    void setCurrentPosition(const sf::Vector2f& position);
    sf::Vector2f getCurrentPosition() const;
    void setPreviousPosition(const sf::Vector2f& position);
    sf::Vector2f getPreviousPosition() const;
    void setID(PiranhaID ID);
    PiranhaID getID() const;
    void setHitbox(const sf::FloatRect& hitbox);
    sf::FloatRect getHitbox() const;
    void setDisabled(bool disabled);
    bool isDisabled() const;
    void setSpeed(float speed);
    float getSpeed() const;
    void setPositionLimit(float position_limit);
    float getPositionLimit() const;
    void setPositionTemporary(float position_temporary);
    float getPositionTemporary() const;
    void setState(bool state);
    bool getState() const;
    void setStopTime(float stop_time);
    float getStopTime() const;
    void restartStopClock();
    sf::Clock getStopClock() const;
    void setStop(bool stop);
    bool getStop() const;
    void setDistanceAppear(float distance_appear);
    float getDistanceAppear() const;
    void setTexture(const sf::Texture& texture,bool resetRect = false);

private:
    void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

    sf::VertexArray       m_vertices;
    sf::Texture           m_texture;
    sf::Vector2f          m_curr{};
    sf::Vector2f          m_prev{};
    PiranhaID             m_ID{};
    sf::FloatRect         m_hitbox;
    bool                  m_Disabled{};
    float                 m_speed{};
    float                 m_position_limit{};
    float                 m_position_temporary{};
    bool                  m_state{};
    float                 m_stop_time{};
    sf::Clock             m_stop_clock;
    bool                  m_stop{};
    float                 m_distance_appear{};
};

#endif //PIRANHAAICLASS_HPP
