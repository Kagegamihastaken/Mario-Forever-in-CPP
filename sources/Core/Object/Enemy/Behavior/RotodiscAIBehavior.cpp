#include "../../../../../headers/Core/Object/Enemy/Behavior/RotodiscAIBehavior.hpp"

#include "Core/Logging.hpp"
#include "Core/WindowFrame.hpp"

sf::Vector2f RotodiscAIBehavior::RotodiscPositionAdjust(const sf::Vector2f &position, unsigned int arrange_mode) {
    return position + sf::Vector2f(((arrange_mode >> 1) & 1) * 16.f, (arrange_mode & 1) * 16.f);
    // 1 bit: y, 2 bit: x
}

sf::Vector2f RotodiscAIBehavior::RotodiscRoundXUpdate(const sf::Vector2f &position, const sf::Vector2f &pos_origin, const float angle, const float radius) {
    return sf::Vector2f(pos_origin.x + radius * std::sin(angle * M_PI / 180.f), position.y);
}

sf::Vector2f RotodiscAIBehavior::RotodiscRoundYUpdate(const sf::Vector2f &position, const sf::Vector2f &pos_origin, const float angle, const float radius) {
    return sf::Vector2f(position.x, pos_origin.y + radius * std::cos(angle * M_PI / 180.f));
}

float RotodiscAIBehavior::RotodiscAngleUpdate(float angle, float speed, float deltaTime) {
    float Output_angle = angle;
    Output_angle += speed * deltaTime;
    if (Output_angle > 360.f) {
        Output_angle = f_mod(Output_angle, 360.f);
    }
    return Output_angle;
}
void RotodiscAIBehavior::RotodiscRadiusChange(float& current_radius, bool& state, float radius, float speed_change, float deltaTime) {
    if (!state) {
        current_radius += speed_change * deltaTime;
        if (current_radius >= radius) {
            current_radius = radius;
            state = true;
        }
    }
    else {
        current_radius -= speed_change * deltaTime;
        if (current_radius <= 0.f) {
            current_radius = 0.f;
            state = false;
        }
    }
}