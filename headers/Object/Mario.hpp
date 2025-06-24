#pragma once

#ifndef MARIO_HPP
#define MARIO_HPP

#include "../Core/ImageManager.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Editor/RenderTile.hpp"

class MovableObject {
public:
	sf::FloatRect hitboxMain;
	sf::FloatRect hitboxBot;
	sf::FloatRect hitboxBot2;
	sf::FloatRect hitboxLeft;
	sf::FloatRect hitboxRight;
	sf::FloatRect hitboxTop;
	sf::FloatRect hitboxSlopeBot;
	sf::Vector2f curr;
	sf::Vector2f prev;
	sf::Sprite property = sf::Sprite(tempTex);

	sf::FloatRect hitboxWall;
	sf::FloatRect hitboxFloor;
};
extern void MarioUpdateAnimation();
extern MovableObject player;
extern void SetPrevMarioPos();
extern void InterpolateMarioPos(float alpha);
extern float player_speed;
extern float Xvelo;
extern float Yvelo;
extern bool MarioDirection;
extern bool MarioCurrentFalling;
extern bool MarioCrouchDown;
extern bool MarioAppearing;
extern bool CanControlMario;
extern bool FirstMarioDirection;
extern bool PreJump;
extern bool Holding;
extern long long int Score;
extern void KeyboardMovement(float deltaTime);
extern void MarioDraw();
extern void PowerDown();
extern void MarioPosYUpdate(float deltaTime);
extern void MarioVertYUpdate();
extern void MarioPosXUpdate(float deltaTime);
extern void MarioVertXUpdate();
extern void CheckForDeath();
extern void loadMarioRes();
extern void Death();
extern void SetPowerState(int ps);
extern void InvincibleStateUpdate();
extern void MarioUpdateHitbox();
extern int PowerState;
extern int Lives;

static constexpr int MARIO_IMAGE_WIDTH = 310;
static constexpr int MARIO_WIDTH = 31;
static constexpr int MARIO_HEIGHT = 59;

static std::vector<std::string> SmallMarioLeft;
static std::vector<std::string> SmallMarioRight;
static std::vector<std::string> BigMarioLeft;
static std::vector<std::string> BigMarioRight;
static std::vector<std::string> FireMarioLeft;
static std::vector<std::string> FireMarioRight;

//define here
inline SingleAnimationObject MarioAnimation;
inline MovableObject player;
inline float Xvelo = 0.0f;
inline float Yvelo = 0.0f;
inline bool FirstMarioDirection = false;
inline bool MarioDirection = FirstMarioDirection;
inline bool MarioCurrentFalling = true;
inline bool PreJump = false;
inline bool Holding;
inline bool MarioCrouchDown = false;
inline float player_speed;
inline int MarioState = 0;
inline static int lastMarioState = -1;
inline int PowerState = 0;
inline static int lastPowerState = 0;

inline int Lives = 4;

static bool OverSpeed = false;
inline long long int Score = 0;
static sf::Clock AppearingTimer;
static sf::Clock InvincibleTimer;
inline bool Invincible = false;
inline bool InvincibleState = false;
inline bool MarioAppearing = false;

static float FireTime = 4.f;
static float FireTimeCounting = FireTime;
static bool isFireHolding = false;


inline bool CanControlMario = true;
// 0 for right; 1 for left

#endif //MARIO_HPP