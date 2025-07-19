#pragma once

#ifndef TILEPROPERTY_HPP
#define TILEPROPERTY_HPP

#include <string>

struct IntProps {
    std::string name;
    int val;
    int min;
    int max;
};
struct BoolProps {
    std::string name;
    bool val;
};
struct StringProps {
    std::string name;
    char val[128];
};
struct FloatProps {
    std::string name;
    float val;
    float min;
    float max;
};
struct Vector2fProps {
    std::string name;
    sf::Vector2f val;
};
#endif //TILEPROPERTY_HPP
