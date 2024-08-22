#pragma once

#ifndef COLLIDE_HPP
#define COLLIDE_HPP

#include "Mario.hpp"
#include "Obstacles.hpp"

extern void setHitbox(sf::FloatRect& hitbox, const sf::FloatRect& Sethitbox);
extern sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Sprite& sprite);
extern bool isCollide(const sf::FloatRect& hitbox, const sf::Sprite& sprite, const sf::FloatRect& other);

extern bool isCollideLeft2(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList);
extern bool isCollideRight2(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList);
extern std::pair<bool, bool> isOrCollideSide(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList);
extern std::pair<bool, bool> isAccurateCollideSide(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, const std::vector<std::pair<float, float>>& SaveList);
extern bool isCollideBot(const MovableObject& object, const std::vector<Obstacles>& OL);
extern std::pair<bool, std::pair<bool, bool>> isAccurateCollideBot(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosYCollide, bool& NoAdd);
extern std::pair<bool, std::pair<bool, bool>> isAccuratelyCollideBot(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, float& ID, std::vector<std::array<float, 3>>& SlopeTemp);
extern bool isCollideTop(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList);
extern std::vector<std::pair<float, float>> isCollideTopDetailed(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList);
extern std::pair<bool, std::pair<bool, bool>> isAccurateCollideTop(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosYCollide, bool& NoAdd, const std::vector<std::pair<float, float>>& SaveList);

#endif // COLLIDE_HPP