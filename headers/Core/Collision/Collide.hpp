#pragma once

#ifndef COLLIDE_HPP
#define COLLIDE_HPP

#include "Core/Class/CollisionObjectClass.hpp"

extern void setHitbox(sf::FloatRect& hitbox, const sf::FloatRect& Sethitbox);
extern sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Sprite& sprite);
extern sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Vector2f& origin);
extern sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Vector2f& pos, const sf::Sprite& sprite);
extern bool isCollide(const sf::FloatRect& hitbox, const sf::Sprite& sprite, const sf::FloatRect& other);
extern bool isCollide(const sf::FloatRect& hitbox, const sf::FloatRect& other);

extern bool GetRelativeTilemapSlopeUp(float CurrPosXCollide, float CurrPosYCollide);
extern bool GetRelativeTilemapSlopeDown(float CurrPosXCollide, float CurrPosYCollide);
extern bool GetRelativeTilemapSlopeLeft(float CurrPosXCollide, float CurrPosYCollide);
extern bool GetRelativeTilemapSlopeRight(float CurrPosXCollide, float CurrPosYCollide);
extern bool GetRelativeTilemapSlopeBetween(float CurrPosXCollide, float CurrPosYCollide);

extern std::pair<bool, bool> isAccurateCollideSide(const MFCPP::CollisionObject& CollideObj, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, unsigned ID);
extern std::vector<sf::Vector2f> isAccurateCollideMainCollectable(const MFCPP::CollisionObject& CollideObj, unsigned ID, const sf::FloatRect& rect);
extern std::vector<std::pair<float, float>> isCollideTopDetailed(const MFCPP::CollisionObject& CollideObj, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, unsigned ID);
extern bool isAccurateCollideTop(const MFCPP::CollisionObject& CollideObj, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, unsigned ID);
extern bool isAccurateCollideBot(const MFCPP::CollisionObject& CollideObj, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, unsigned ID);
extern bool isAccurateCollideBotStopEdge(const MFCPP::CollisionObject& CollideObj, bool direction);

extern std::pair<bool, bool> CheckCollisionSide(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd, unsigned ID);
extern bool CheckCollisionBot(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd, unsigned ID);
extern bool CheckCollisionTop(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd, unsigned ID);
extern std::vector<std::pair<float, float>> CheckCollisionTopDetailed(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd, unsigned ID);

extern std::pair<bool, bool> QuickCheckOnlyObstaclesSideCollision(const MFCPP::CollisionObject& CollideObj, bool direction, float& CurrX, float& CurrY, bool& NoAdd);
extern std::pair<bool, bool> QuickCheckOnlyHittableSideCollision(const MFCPP::CollisionObject& CollideObj, bool direction, float& CurrX, float& CurrY, bool& NoAdd);
extern bool QuickCheckOnlyObstacleTopCollision(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd);
extern bool QuickCheckOnlyObstacleBotCollision(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd);
extern bool QuickCheckOnlyHittableBotCollision(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd);
extern bool QuickCheckOnlyHittableTopCollision(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY, bool& NoAdd);
extern std::pair<bool, bool> QuickCheckSideCollision(const MFCPP::CollisionObject& CollideObj, bool direction, float& CurrX, float& CurrY);
extern bool QuickCheckBotCollision(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY);
extern bool QuickCheckTopCollision(const MFCPP::CollisionObject& CollideObj, float& CurrX, float& CurrY);

extern float GetCurrFloorY(const sf::Vector2f& pos, float CurrX, float CurrY);
#endif // COLLIDE_HPP