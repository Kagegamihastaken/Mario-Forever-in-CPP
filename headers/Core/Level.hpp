#pragma once

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include "Editor/SelectTile.hpp"

struct PlatformData {
    sf::Vector2f start;
    sf::Vector2f end;
    CustomTileProperty props;
};
struct RotodiscData {
    unsigned int data;
    sf::Vector2f pos;
    CustomTileProperty props;
};

extern void Bgbuilding();
extern void CheckpointBuilding();
extern void Obstaclebuilding();
extern void Objectbuilding();
extern void Slopebuilding();
extern void ExitGateBuilding();
extern void ReadData(const std::filesystem::path& path);
extern float LevelWidth, LevelHeight;
extern std::string getMusicLevelName();

#endif // !LEVEL_HPP