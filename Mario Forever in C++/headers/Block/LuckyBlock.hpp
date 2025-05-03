#pragma once
#ifndef LUCKYBLOCK_HPP
#define LUCKYBLOCK_HPP

#include <vector>
#include "Obstacles.hpp"
#include "../Core/Loading/enum.hpp"
#include "../Core/Animate/LocalAnimationManager.hpp"

extern std::vector<Obstacles> LuckyBlock;
extern std::vector<LuckyBlockID> LuckyBlockIDList;
extern std::vector<std::pair<float, float>> LuckyBlockSaveList;
extern std::vector<std::pair<sf::FloatRect, sf::Vector2f>> LuckyVertPosList;
extern void LuckyBlockSort();
extern void LuckyHit(float x, float y, int i);
extern int getLuckyIndex(float x, float y);
extern void AddLuckyBlock(LuckyBlockID ID, LuckyBlockAtt Att, float x, float y);
extern void LoadLuckyBlock();
extern inline void LuckyBlockDraw();
extern inline void LuckyBlockUpdate(float deltaTime);
extern inline void LuckyAnimationUpdate();
extern void DeleteAllLuckyBlock();
void LuckyHitEvent(float x, float y);

#endif // LUCKYBLOCK_HPP