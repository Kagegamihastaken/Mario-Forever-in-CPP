#ifndef MFCPP_PLATFORMBEHAVIOR_HPP
#define MFCPP_PLATFORMBEHAVIOR_HPP

class PlatformBehavior {
public:
    struct PlatformData {
        sf::Vector2f position;
        //setting
        bool smooth{}, wait{}, fall{};
        sf::Vector2f start, end;
        float speed{}, y_velocity{}, smoothStore{}, maxSpeed{};
        bool movingToEnd{}, willFall{};
        int waitState{};

    };
    static PlatformData PlatformStatusUpdate(const PlatformData& data, float deltaTime);
};

#endif //MFCPP_PLATFORMBEHAVIOR_HPP
