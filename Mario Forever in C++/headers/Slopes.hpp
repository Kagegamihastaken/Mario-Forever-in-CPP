#ifndef SLOPES_HPP
#define SLOPES_HPP

#include "../headers/Obstacles.hpp"

extern std::vector<Obstacles> SlopesList;
extern std::vector<int> SlopesGraphIDList;
extern sf::Texture SlopeTexture;
extern std::vector<std::array<float, 2>> GraphList;
extern std::vector<std::array<int, 4>> IDSlope_list;
extern void SlopeUpdate();
extern int getSlopesIndex(float x, float y);
#endif //SLOPES_HPP