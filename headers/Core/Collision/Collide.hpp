#pragma once

#ifndef COLLIDE_HPP
#define COLLIDE_HPP

#include "Object/Mario.hpp"
#include "Block/Obstacles.hpp"
#include "Class/CollisionObjectClass.hpp"

extern void setHitbox(sf::FloatRect& hitbox, const sf::FloatRect& Sethitbox);
extern sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Sprite& sprite);
extern sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
extern sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Sprite& sprite);
extern bool isCollide(const sf::FloatRect& hitbox, const sf::Sprite& sprite, const sf::FloatRect& other);
extern bool isCollide(const sf::FloatRect& hitbox, const sf::FloatRect& other);

extern std::pair<bool, bool> isAccurateCollideSide(const MFCPP::CollisionObject& CollideObj, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, int first, int last, float distance);
extern std::pair<bool, bool> isAccurateCollideSidet(const MFCPP::CollisionObject& CollideObj, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, int first, int last, float distance);
extern std::vector<sf::Vector2f> isAccurateCollideMaint(const MFCPP::CollisionObject& CollideObj, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, const int& first, const int& last, const float& distance);
extern bool isAccurateCollideBott(const MFCPP::CollisionObject& CollideObj, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OLVert, float& CurrPosYCollide, bool& NoAdd, int first, int last, float distance);
extern std::vector<std::pair<float, float>> isCollideTopDetailed(const MFCPP::CollisionObject& CollideObj, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OLVert, int first, int last, float distance);
extern bool isAccurateCollideTopt(const MFCPP::CollisionObject& CollideObj, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OLVert, float& CurrPosYCollide, bool& NoAdd, int first, int last, float distance);
extern bool isAccurateCollideTop(const MFCPP::CollisionObject& CollideObj, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OLVert, float& CurrPosYCollide, bool& NoAdd, int first, int last, float distance);
extern bool isAccurateCollideBot(const MFCPP::CollisionObject& CollideObj, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OLVert, float& CurrPosYCollide, bool& NoAdd, int first, int last, float distance);
extern int find_max_inx(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL);
extern int find_max_inx_dist(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, float dist);
extern int find_min_inx(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL);
extern int find_min_inx_dist(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, float dist);
extern int find_max_iny(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL);
extern int find_max_iny_dist(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, float dist);
extern int find_min_iny(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL);
extern int find_min_iny_dist(const sf::Vector2f& object, const std::vector<std::pair<sf::FloatRect, sf::Vector2f>>& OL, float dist);

#endif // COLLIDE_HPP