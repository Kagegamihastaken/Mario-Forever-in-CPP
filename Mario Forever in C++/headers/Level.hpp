#pragma once

#ifndef LEVEL_HPP
#define LEVEL_HPP

extern std::vector<std::vector<float>> LevelData;
extern void building();
extern void ReadData(std::ifstream& foi);
extern float LevelWidth, LevelHeight;

#endif // !LEVEL_HPP