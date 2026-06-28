#pragma once

#ifndef MARIO_HPP
#define MARIO_HPP

#include <config.h>
#include "Core/ImageManager.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Object/ActiveObject.hpp"

class Mario {
public:
	Mario() = delete;
	Mario(const Mario&) = delete;
	Mario(Mario&&) = delete;
	Mario& operator=(const Mario&) = delete;
	Mario& operator=(Mario&&) = delete;

	static void MarioUpdateAnimation();

	static void SetPrevMarioPos();
	static void KeyboardMovement(float deltaTime);
	static void MarioDraw(float alpha);
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
	static void MarioAnimationUpdate(float deltaTime);
	//get & set function
	static void setXvelocity(float val);
	static void setYvelocity(float val);
	[[nodiscard]] static float getXvelocity();
	[[nodiscard]] static float getYvelocity();

	[[nodiscard]] static sf::FloatRect getHitbox();
	[[nodiscard]] static sf::FloatRect getHitboxWall();
	[[nodiscard]] static sf::FloatRect getHitboxFloor();
	[[nodiscard]] static sf::Vector2f getOrigin();
	static void setCurrentPosition(const sf::Vector2f& val);
	static void forceSetPosition(const sf::Vector2f& val);
	[[nodiscard]] static sf::Vector2f getCurrentPosition();
	static void resetPreviousPosition();
	static void setDirection(bool val);
	static void setFirstDirection(bool val);
	[[nodiscard]] static bool getDirection();
	[[nodiscard]] static bool getFirstDirection();
	static void setLives(uint16_t val);
	static void setScore(uint64_t val);
	[[nodiscard]] static uint16_t getLives();
	[[nodiscard]] static uint64_t getScore();
	[[nodiscard]] static bool isPrejump();
	[[nodiscard]] static bool isFalling();
	[[nodiscard]] static int getPowerState();
	static void setCanControl(bool val);
	[[nodiscard]] static bool getCanControl();
	[[nodiscard]] static sf::Vector2f getInterpolatedPosition();
	static void setHolding(bool val);
	[[nodiscard]] static bool getHolding();
	[[nodiscard]] static bool getCurrentFalling();

	static void MarioEffectActivate();

	static void storeAlpha(float alpha);

	// extern float player_speed;
	// extern bool MarioCrouchDown;
	// extern bool MarioAppearing;
	// extern bool Holding;
private:
	static auto UpdateSequenceAnimation() -> void;

	static void CheckXCollision();

	static sf::FloatRect m_hitboxFloor;
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
	static int8_t m_MarioState;
	static int8_t m_lastMarioState;
	static int8_t m_PowerState;
	static int8_t m_lastPowerState;

	static uint16_t m_Lives;

	static bool m_OverSpeed;
	static uint64_t m_Score;
	static sf::Clock m_AppearingTimer;
	static sf::Clock m_InvincibleTimer;
	static bool m_Invincible;
	static bool m_InvincibleState;
	static bool m_MarioAppearing;

	static bool m_OutsideWallLeft;
	static bool m_OutsideWallRight;

	static float m_FireTime;
	static float m_FireTimeCounting;
	static bool m_isFireHolding;

	static bool m_CanControlMario;

	static constexpr uint16_t MARIO_IMAGE_WIDTH = 310;
	static constexpr uint16_t MARIO_WIDTH = 31;
	static constexpr uint16_t MARIO_HEIGHT = 62;
	static constexpr uint16_t MARIO_OFFSET_X = 4;

	static float m_temp_alpha;
};

// 0 for right; 1 for left

#endif //MARIO_HPP