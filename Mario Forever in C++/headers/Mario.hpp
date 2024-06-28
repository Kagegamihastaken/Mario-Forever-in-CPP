#pragma once

#ifndef MARIO_HPP
#define MARIO_HPP

#include "AnimationManager.hpp"

class Mario {
public:
	sf::FloatRect hitboxMain;
	sf::FloatRect hitboxBot;
	sf::FloatRect hitboxLeft;
	sf::FloatRect hitboxRight;
	sf::FloatRect hitboxLeft2;
	sf::FloatRect hitboxRight2;
	sf::FloatRect hitboxTop;
	sf::Sprite property;
	// The main hitbox of the mario
	void setHitboxMain(const sf::FloatRect& Sethitbox);
	sf::FloatRect getGlobalHitboxMain() const;
	bool isCollideMain(sf::FloatRect& other);
	// The bottom hitbox of the mario
	void setHitboxBot(const sf::FloatRect& Sethitbox);
	sf::FloatRect getGlobalHitboxBot() const;
	bool isCollideBot(sf::FloatRect& other);
	// The left hitbox of the mario
	void setHitboxLeft(const sf::FloatRect& Sethitbox);
	sf::FloatRect getGlobalHitboxLeft() const;
	bool isCollideLeft(sf::FloatRect& other);
	// The right hitbox of the mario
	void setHitboxRight(const sf::FloatRect& Sethitbox);
	sf::FloatRect getGlobalHitboxRight() const;
	bool isCollideRight(sf::FloatRect& other);
	// The top hitbox of the mario
	void setHitboxTop(const sf::FloatRect& Sethitbox);
	sf::FloatRect getGlobalHitboxTop() const;
	bool isCollideTop(sf::FloatRect& other);

	// The left hitbox of the mario
	void setHitboxLeft2(const sf::FloatRect& Sethitbox);
	sf::FloatRect getGlobalHitboxLeft2() const;
	bool isCollideLeft2(sf::FloatRect& other);
	// The right hitbox of the mario
	void setHitboxRight2(const sf::FloatRect& Sethitbox);
	sf::FloatRect getGlobalHitboxRight2() const;
	bool isCollideRight2(sf::FloatRect& other);
};
extern AnimationManager MarioAnimation;
extern void UpdateAnimation();
extern Mario player;
extern float player_speed;
extern float Xvelo;
extern float Yvelo;
extern bool MarioDirection;
extern bool MarioCurrentFalling;
extern bool MarioCrouchDown;
extern bool MarioAppearing;
extern long long int Score;
extern void KeyboardMovement();
extern inline void MarioDraw();
extern void MarioSetSmall();
extern void MarioVertYUpdate();
extern void MarioVertXUpdate();
extern void CheckForDeath();
extern int PowerState;

#endif //MARIO_HPP