#pragma once
#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

namespace MFCPP {
    namespace Interpolation {
        sf::Vector2f linearInterpolation(sf::Vector2f start, sf::Vector2f end, float alpha);
        sf::Angle linearInterpolation(sf::Angle start, sf::Angle end, float alpha);

        inline bool isInterpolation;
    }
}

#endif