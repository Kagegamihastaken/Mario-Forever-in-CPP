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
	void setHitboxMain(const sf::FloatRect& Sethitbox) { this->hitboxMain = Sethitbox; }
	sf::FloatRect getGlobalHitboxMain() const { return this->property.getTransform().transformRect(hitboxMain); }
	bool isCollideMain(sf::FloatRect& other) { return getGlobalHitboxMain().intersects(other); }
	// The bottom hitbox of the mario
	void setHitboxBot(const sf::FloatRect& Sethitbox) { this->hitboxBot = Sethitbox; }
	sf::FloatRect getGlobalHitboxBot() const { return this->property.getTransform().transformRect(hitboxBot); }
	bool isCollideBot(sf::FloatRect& other) { return getGlobalHitboxBot().intersects(other); }
	// The left hitbox of the mario
	void setHitboxLeft(const sf::FloatRect& Sethitbox) { this->hitboxLeft = Sethitbox; }
	sf::FloatRect getGlobalHitboxLeft() const { return this->property.getTransform().transformRect(hitboxLeft); }
	bool isCollideLeft(sf::FloatRect& other) { return getGlobalHitboxLeft().intersects(other); }
	// The right hitbox of the mario
	void setHitboxRight(const sf::FloatRect& Sethitbox) { this->hitboxRight = Sethitbox; }
	sf::FloatRect getGlobalHitboxRight() const { return this->property.getTransform().transformRect(hitboxRight); }
	bool isCollideRight(sf::FloatRect& other) { return getGlobalHitboxRight().intersects(other); }
	// The top hitbox of the mario
	void setHitboxTop(const sf::FloatRect& Sethitbox) { this->hitboxTop = Sethitbox; }
	sf::FloatRect getGlobalHitboxTop() const { return this->property.getTransform().transformRect(hitboxTop); }
	bool isCollideTop(sf::FloatRect& other) { return getGlobalHitboxTop().intersects(other); }

	// The left hitbox of the mario
	void setHitboxLeft2(const sf::FloatRect& Sethitbox) { this->hitboxLeft2 = Sethitbox; }
	sf::FloatRect getGlobalHitboxLeft2() const { return this->property.getTransform().transformRect(hitboxLeft2); }
	bool isCollideLeft2(sf::FloatRect& other) { return getGlobalHitboxLeft2().intersects(other); }
	// The right hitbox of the mario
	void setHitboxRight2(const sf::FloatRect& Sethitbox) { this->hitboxRight2 = Sethitbox; }
	sf::FloatRect getGlobalHitboxRight2() const { return this->property.getTransform().transformRect(hitboxRight2); }
	bool isCollideRight2(sf::FloatRect& other) { return getGlobalHitboxRight2().intersects(other); }
};
extern AnimationManager MarioAnimation;
extern void UpdateAnimation();
extern Mario player;
extern float player_speed;
extern float Xvelo;
extern float Yvelo;
extern bool MarioDirection;
extern bool MarioCurrentFalling;
extern void KeyboardMovement();
extern inline void MarioDraw();
extern void MarioSetSmall();
extern void MarioVertYUpdate();
extern void MarioVertXUpdate();
extern void CheckForDeath();
extern int PowerState;

#endif //MARIO_HPP