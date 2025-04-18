#pragma once

#ifndef COLLIDE_HPP
#define COLLIDE_HPP

#include "../../Object/Mario.hpp"
#include "../../Block/Obstacles.hpp"

extern void setHitbox(sf::FloatRect& hitbox, const sf::FloatRect& Sethitbox);
extern sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Sprite& sprite);
extern bool isCollide(const sf::FloatRect& hitbox, const sf::Sprite& sprite, const sf::FloatRect& other);

extern bool isCollideLeft2(const MovableObject& object, std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList);
extern bool isCollideRight2(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList);
extern std::pair<bool, bool> isOrCollideSide(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList);
extern std::pair<bool, bool> isOrCollideSidet(const MovableObject& object, const std::vector<Obstacles>& OL, int first, int last, const std::vector<std::pair<float, float>>& SaveList);
extern std::pair<bool, bool> isAccurateCollideSide(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, const std::vector<std::pair<float, float>>& SaveList);
extern std::pair<bool, bool> isAccurateCollideSidet(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, int first, int last, float distance, const std::vector<std::pair<float, float>>& SaveList);
extern std::pair<bool, bool> isAccurateCollideSide2(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosXCollide, float& CurrPosYCollide, int first, int last, const std::vector<std::pair<float, float>>& SaveList);
extern bool isCollideBotSlope(const MovableObject& object, float Yvelo);
extern bool isCollideBot(const MovableObject& object, const std::vector<Obstacles>& OL);
extern std::pair<bool, bool> isAccurateCollideBot(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosYCollide, bool& NoAdd);
extern std::pair<bool, bool> isAccurateCollideBott(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OLVert, float& CurrPosYCollide, bool& NoAdd, int first, int last, float distance);
extern std::pair<bool, std::pair<bool, bool>> isAccuratelyCollideBotSlope(const MovableObject& object, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, float& ID, std::vector<std::array<float, 3>>& SlopeTemp);
extern std::pair<bool, std::pair<bool, bool>> isAccuratelyCollideBot(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, float& ID, std::vector<std::array<float, 3>>& SlopeTemp);
extern bool isCollideTop(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList);
extern std::vector<std::pair<float, float>> isCollideTopDetailed(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList);
extern std::pair<bool, bool> isAccurateCollideTop(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosYCollide, bool& NoAdd, const std::vector<std::pair<float, float>>& SaveList);
extern std::pair<bool, bool> isAccurateCollideTopt(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OLVert, float& CurrPosYCollide, bool& NoAdd, int first, int last, float distance);
extern int find_max_inx(const MovableObject& object, const std::vector<Obstacles>& OL);
extern int find_max_inx_dist(const MovableObject& object, const std::vector<Obstacles>& OL, float dist);
extern int find_min_inx(const MovableObject& object, const std::vector<Obstacles>& OL);
extern int find_min_inx_dist(const MovableObject& object, const std::vector<Obstacles>& OL, float dist);
extern int find_max_iny(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL);
extern int find_max_iny_dist(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, float dist);
extern int find_min_iny(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL);
extern int find_min_iny_dist(const MovableObject& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, float dist);

#endif // COLLIDE_HPP