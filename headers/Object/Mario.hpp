#pragma once

#ifndef MARIO_HPP
#define MARIO_HPP

#include "../Core/ImageManager.hpp"
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

#endif //MARIO_HPP