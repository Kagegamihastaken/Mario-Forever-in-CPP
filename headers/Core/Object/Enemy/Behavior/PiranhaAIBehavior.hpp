#ifndef MFCPP_PIRANHAAIBEHAVIOR_HPP
#define MFCPP_PIRANHAAIBEHAVIOR_HPP

#include "Core/Loading/enum.hpp"
#include "SFML/System/Vector2.hpp"

class PiranhaAIBehavior {
public:
    struct PiranhaAIData {
        sf::Vector2f pos;
        bool stop, state;
        float speed, pos_temp, pos_limit, stop_clock, stop_time, distance_appear;
        int fire_counting, fire_count;
        float fire_ticking, fire_interval;
    };
    static sf::Vector2f PiranhaPositionAdjust(PiranhaDirection dir, const sf::Vector2f& pos);
    static PiranhaAIData PiranhaFireUpdate(bool& flag, PiranhaAIData data, float deltaTime);
    static PiranhaAIData PiranhaMovementUpdate(PiranhaAIData data, PiranhaDirection dir, float deltaTime);
};

#endif //MFCPP_PIRANHAAIBEHAVIOR_HPP