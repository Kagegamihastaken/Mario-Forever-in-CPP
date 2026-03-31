#pragma once

#ifndef MARIO_HPP
#define MARIO_HPP

#include <config.h>
#include "Core/ImageManager.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Class/ActiveObjectClass.hpp"

class Mario {
public:
	Mario() = delete;
	static void MarioUpdateAnimation();

	static void SetPrevMarioPos();
	static void InterpolateMarioPos(float alpha);
	static void KeyboardMovement(float deltaTime);
	static void MarioDraw();
	static void PowerDown();
	static void MarioPosYUpdate(float deltaTime);
	static void MarioVertYBottomUpdate();
	static void MarioVertYTopUpdate();
	static void MarioPosXUpdate(float deltaTime);
	static void MarioVertXUpdate();
	static void CheckForDeath();
	static void loadMarioRes();
	static void Death();
	static void SetPowerState(int ps);
	static void InvincibleStateUpdate();
	static void MarioUpdateHitbox();
	static void MarioOutSideScreen();
	//get & set function
	static void setXvelocity(float val);
	static void setYvelocity(float val);
	[[nodiscard]] static float getXvelocity();
	[[nodiscard]] static float getYvelocity();

	[[nodiscard]] static sf::FloatRect getHitbox();
	[[nodiscard]] static sf::FloatRect getHitboxWall();
	[[nodiscard]] static sf::Vector2f getOrigin();
	static void setCurrentPosition(const sf::Vector2f& val);
	[[nodiscard]] static sf::Vector2f getCurrentPosition();
	static void resetPreviousPosition();
	static void setDirection(bool val);
	static void setFirstDirection(bool val);
	[[nodiscard]] static bool getDirection();
	[[nodiscard]] static bool getFirstDirection();
	static void setLives(int val);
	static void setScore(long long val);
	[[nodiscard]] static int getLives();
	[[nodiscard]] static int getScore();
	[[nodiscard]] static bool isPrejump();
	[[nodiscard]] static bool isFalling();
	[[nodiscard]] static int getPowerState();
	static void setCanControl(bool val);
	[[nodiscard]] static bool getCanControl();
	[[nodiscard]] static sf::Vector2f getInterpolatedPosition();

	// extern float player_speed;
	// extern bool MarioCrouchDown;
	// extern bool MarioAppearing;
	// extern bool Holding;
private:
	static void UpdateSequenceAnimation();

	static sf::FloatRect m_hitboxFloor;
	static sf::FloatRect m_hitboxTop;
	static sf::FloatRect m_hitboxWall;

	static MFCPP::SingleAnimationObject m_MarioAnimation;
	static MFCPP::ActiveObject<float> m_player;
	static sf::Vector2f m_velocity;
	static bool m_FirstMarioDirection;
	static bool m_MarioDirection ;
	static bool m_MarioCurrentFalling;
	static bool m_PreJump;
	static bool m_Holding;
	static bool m_MarioCrouchDown;
	static float m_player_speed;
	static int m_MarioState;
	static int m_lastMarioState;
	static int m_PowerState;
	static int m_lastPowerState;

	static int m_Lives;

	static bool m_OverSpeed;
	static long long int m_Score;
	static sf::Clock m_AppearingTimer;
	static sf::Clock m_InvincibleTimer;
	static bool m_Invincible;
	static bool m_InvincibleState;
	static bool m_MarioAppearing;

	static bool m_OutsideWallLeft;

	static float m_FireTime;
	static float m_FireTimeCounting;
	static bool m_isFireHolding;

	static bool m_CanControlMario;

	static constexpr int MARIO_IMAGE_WIDTH = 310;
	static constexpr int MARIO_WIDTH = 31;
	static constexpr int MARIO_HEIGHT = 62;
	static constexpr int MARIO_OFFSET_X = 4.f;
};

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

static std::vector<std::string> SmallMario;
static std::vector<std::string> BigMario;
static std::vector<std::string> FireMario;

//define here
// 0 for right; 1 for left

#endif //MARIO_HPP