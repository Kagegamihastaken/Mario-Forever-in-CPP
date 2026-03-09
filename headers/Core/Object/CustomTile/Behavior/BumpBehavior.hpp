#ifndef MFCPP_BUMPBEHAVIOR_HPP
#define MFCPP_BUMPBEHAVIOR_HPP
#include "SFML/System/Vector2.hpp"

class BumpBehavior {
public:
    struct BumpData {
        sf::Vector2f pos;
        float state_count{};
        bool state{}, updown{};
    };
    static BumpData BumpCoinUpdate(const BumpData& data, float Ypos, float deltaTime);
    static BumpData BumpItemUpdate(const BumpData& data, float Ypos, float deltaTime);
};

#endif //MFCPP_BUMPBEHAVIOR_HPP