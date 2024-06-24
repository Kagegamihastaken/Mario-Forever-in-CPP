#pragma once
#ifndef LUCKYBLOCK_HPP
#define LUCKYBLOCK_HPP

#include <vector>
#include "../headers/Obstacles.hpp"
#include "../headers/enum.hpp"
#include "../headers/LocalAnimationManager.hpp"

extern std::vector<Obstacles> LuckyBlock;
extern std::vector<LuckyBlockID> LuckyBlockIDList;
extern sf::Texture LuckyBlockTexture;
extern std::vector<std::pair<float, float>> LuckyBlockSaveList;
extern void AddLuckyBlock(LuckyBlockID ID, float x, float y);
extern void LuckyBlockUpdate();
extern void LuckyAnimationUpdate();
void LuckyHitEvent(float x, float y);

#endif // LUCKYBLOCK_HPP