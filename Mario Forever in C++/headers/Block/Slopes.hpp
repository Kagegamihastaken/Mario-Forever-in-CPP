#pragma once

#ifndef SLOPES_HPP
#define SLOPES_HPP

#include "Obstacles.hpp"

extern std::vector<Obstacles> SlopesList;
extern std::vector<int> SlopesIDList;
extern std::array<std::array<sf::FloatRect, 32>, 3> SlopeRectList;
extern sf::Texture SlopeTexture;
extern std::vector<std::array<float, 2>> GraphList;
extern std::vector<std::array<int, 3>> IDSlope_list;
extern void SlopeUpdate();
extern void loadSlopeRes();
extern int getSlopesIndex(float x, float y);
#endif //SLOPES_HPP