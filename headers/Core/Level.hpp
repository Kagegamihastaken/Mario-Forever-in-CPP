#pragma once

#ifndef LEVEL_HPP
#define LEVEL_HPP

extern void Bgbuilding();
extern void Obstaclebuilding();
extern void Objectbuilding();
extern void Slopebuilding();
extern void ExitGateBuilding();
extern void ReadData(std::string_view path);
extern float LevelWidth, LevelHeight;

#endif // !LEVEL_HPP