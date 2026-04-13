#ifndef MFCPP_SCENERYHELPER_HPP
#define MFCPP_SCENERYHELPER_HPP

#include "Core/ImageManager.hpp"

extern std::vector<std::string> BlueCloudAnimName;
extern std::vector<std::string> BlueGrassAnimName;
extern void SceneryInit();
extern void AddScenery(int id, const sf::Vector2f& pos);

#endif //MFCPP_SCENERYHELPER_HPP
