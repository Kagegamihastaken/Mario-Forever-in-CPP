#pragma once

#ifndef TILEPROPERTY_HPP
#define TILEPROPERTY_HPP

#include <string>

#include "imgui.h"
#include "Core/Common.hpp"

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
struct ColorProps {
    std::string name;
    MFCPP::Color val;
};
#endif //TILEPROPERTY_HPP
