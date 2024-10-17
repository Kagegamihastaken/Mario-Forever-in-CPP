#pragma once

#ifndef MARIO_HPP
#define MARIO_HPP

#include "../Core/Animate/AnimationManager.hpp"

class MovableObject {
public:
	sf::FloatRect hitboxMain;
	sf::FloatRect hitboxBot;
	sf::FloatRect hitboxBot2;
	sf::FloatRect hitboxLeft;
	sf::FloatRect hitboxRight;
	sf::FloatRect hitboxLeft2;
	sf::FloatRect hitboxRight2;
	sf::FloatRect hitboxTop;
	sf::Sprite property;
};
extern AnimationManager MarioAnimation;
extern void UpdateAnimation();
extern MovableObject player;
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
extern void PowerDown();
extern void MarioVertYUpdate();
extern void MarioVertXUpdate();
extern void CheckForDeath();
extern void Death();
extern int PowerState;

#endif //MARIO_HPP