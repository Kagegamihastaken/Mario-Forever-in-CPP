#pragma once
#ifndef INTERPOLATION_HPP
#define INTERPOLATION_HPP

extern bool isInterpolation;
extern sf::Vector2f linearInterpolation(sf::Vector2f start, sf::Vector2f end, float alpha);
extern sf::Angle linearInterpolation(sf::Angle start, sf::Angle end, float alpha);

#endif