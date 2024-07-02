#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <algorithm>
#include <utility>
#include <array>
#include <set>

#include "../headers/Mario.hpp"
#include "../headers/Obstacles.hpp"
#include "../headers/Brick.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/AnimationManager.hpp"
#include "../headers/LuckyBlock.hpp"
#include "../headers/Loading.hpp"
#include "../headers/Collide.hpp"

#include "../resource.h"

//define here
AnimationManager MarioAnimation;
MovableObject player;
float Xvelo = 0.0f;
float Yvelo = 0.0f;
bool MarioDirection;
bool MarioCanJump = true;
bool MarioCurrentFalling = true;
bool PreJump = false;
bool Holding;
bool MarioCrouchDown = false;
float player_speed;
int MarioState = 0;
int PowerState = 1;
int lastPowerState = 1;
bool OverSpeed = false;
long long int Score = 0;
sf::Clock AppearingTimer;
bool MarioAppearing = false;
// 0 for right; 1 for left
//texture loading
sf::Texture SmallMario;
sf::Texture BigMario;
sf::SoundBuffer jumpSoundBuffer;
//Sound specific
sf::Sound SoundJump;
std::array<float, 2> PowerOffset = { 30.0f, 7.0f };
int loadMarioRes() {
	AppearingTimer.restart().asMilliseconds();
	// Resources Loader;
	LoadTexture(SmallMario, SMALLMARIO_TEXTURE);
	LoadTexture(BigMario, BIGMARIO_TEXTURE);

	// Sound Loader
	LoadAudio(jumpSoundBuffer, JUMP_SOUND);

	//set Texture
	//left (small)
	MarioAnimation.addAnimation("RunSmallLeft", &SmallMario, { 6,2 }, { 31,59 }, { 0,0 }, 0, { 0,0 }, { 3,0 });
	MarioAnimation.addAnimation("IdleSmallLeft", &SmallMario, { 6,2 }, { 31,59 }, { 2,0 }, 0, { 2,0 }, { 3,0 });
	MarioAnimation.addAnimation("JumpSmallLeft", &SmallMario, { 6,2 }, { 31,59 }, { 3,0 }, 0, { 3,0 }, { 4,0 });
	MarioAnimation.addAnimation("AppearSmallLeft", &SmallMario, { 6,2 }, { 31,59 }, { 0,2 }, 91, { 0,2 }, { 3,2 });

	//left (big)
	MarioAnimation.addAnimation("RunBigLeft", &BigMario, { 6,2 }, { 31,59 }, { 0,0 }, 0, { 0,0 }, { 3,0 });
	MarioAnimation.addAnimation("IdleBigLeft", &BigMario, { 6,2 }, { 31,59 }, { 2,0 }, 0, { 2,0 }, { 3,0 });
	MarioAnimation.addAnimation("JumpBigLeft", &BigMario, { 6,2 }, { 31,59 }, { 3,0 }, 0, { 3,0 }, { 4,0 });
	MarioAnimation.addAnimation("DownBigLeft", &BigMario, { 6,2 }, { 31,59 }, { 4,0 }, 0, { 4,0 }, { 5,0 });
	MarioAnimation.addAnimation("AppearBigLeft", &BigMario, { 6,2 }, { 31,59 }, { 0,2 }, 91, { 0,2 }, { 3,2 });

	//right (small)
	MarioAnimation.addAnimation("RunSmallRight", &SmallMario, { 6,2 }, { 31,59 }, { 0,1 }, 0, { 0,1 }, { 3,1 });
	MarioAnimation.addAnimation("IdleSmallRight", &SmallMario, { 6,2 }, { 31,59 }, { 2,1 }, 0, { 2,1 }, { 3,1 });
	MarioAnimation.addAnimation("JumpSmallRight", &SmallMario, { 6,2 }, { 31,59 }, { 3,1 }, 0, { 3,1 }, { 4,1 });
	MarioAnimation.addAnimation("AppearSmallRight", &SmallMario, { 6,2 }, { 31,59 }, { 3,2 }, 91, { 3,2 }, { 6,2 });

	//right (big)
	MarioAnimation.addAnimation("RunBigRight", &BigMario, { 6,2 }, { 31,59 }, { 0,1 }, 0, { 0,1 }, { 3,1 });
	MarioAnimation.addAnimation("IdleBigRight", &BigMario, { 6,2 }, { 31,59 }, { 2,1 }, 0, { 2,1 }, { 3,1 });
	MarioAnimation.addAnimation("JumpBigRight", &BigMario, { 6,2 }, { 31,59 }, { 3,1 }, 0, { 3,1 }, { 4,1 });
	MarioAnimation.addAnimation("DownBigRight", &BigMario, { 6,2 }, { 31,59 }, { 4,1 }, 0, { 4,1 }, { 5,1 });
	MarioAnimation.addAnimation("AppearBigRight", &BigMario, { 6,2 }, { 31,59 }, { 3,2 }, 91, { 3,2 }, { 6,2 });

	//set sound
	SoundJump.setBuffer(jumpSoundBuffer);

	//set property of Mario
	return 6;
}
int MarioInit = loadMarioRes();
//sprite function
void KeyboardMovement() {
	sf::FloatRect hitbox_loop;
	bool isCollideSideBool = false;
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && !MarioCrouchDown) {
		if (Xvelo == 0) MarioDirection = true;
		else if (!MarioDirection) {
			Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.375f * deltaTime);
			player.property.move(Xvelo * deltaTime, 0.0f);
		}
		//init speed
		if (Xvelo < 1.0f && MarioDirection) Xvelo = 1.0f;
		if (!isCollideLeft2(player, ObstaclesList, {}) && !isCollideLeft2(player, Bricks, BrickSaveList) && !isCollideLeft2(player, LuckyBlock, LuckyBlockSaveList)) {
			if (MarioDirection) {
				Xvelo += (Xvelo > player_speed ? 0.0f : 0.125f * deltaTime);
				player.property.move((0 - Xvelo) * deltaTime, 0.0f);
			}
		}
	}
	if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) && !MarioCrouchDown) {
		if (Xvelo == 0) MarioDirection = false;
		else if (MarioDirection) {
			Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.375f * deltaTime);
			player.property.move((0 - Xvelo) * deltaTime, 0.0f);
		}
		//init speed
		if (Xvelo < 1.0f && !MarioDirection) Xvelo = 1.0f;
		if (!isCollideRight2(player, ObstaclesList, {}) && !isCollideRight2(player, Bricks, BrickSaveList) && !isCollideRight2(player, LuckyBlock, LuckyBlockSaveList)) {
			if (!MarioDirection) {
				//Xvelo += (Xvelo > player_speed ? 0.0f : 0.125f * deltaTime);
				Xvelo += (Xvelo > player_speed ? 0.0f : 0.125f * deltaTime);
				player.property.move(Xvelo * deltaTime, 0.0f);
			}
		}
	}
	if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) && !MarioCrouchDown) || MarioCrouchDown) {
		if (!MarioCrouchDown) Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.125f * deltaTime);
		else Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.28125f * deltaTime);
		if (!MarioDirection) player.property.move(Xvelo * deltaTime, 0.0f);
		else player.property.move((0 - Xvelo) * deltaTime, 0.0f);
	}
	if (Xvelo < 0.0f) Xvelo = 0.0f;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z) && !MarioCanJump && !MarioCurrentFalling) {
		if (!PreJump && !Holding) {
			if (Yvelo == 0.0f) {
				SoundJump.play();
				Yvelo = -13.5f;
				MarioCanJump = true;
			}
		}
		else if (PreJump) {
			if (Yvelo == 0.0f) {
				SoundJump.play();
				Yvelo = -14.0f;
				MarioCanJump = true;
				PreJump = false;
			}
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) && PowerState > 0 && !MarioCurrentFalling) {
		MarioCrouchDown = true;
	}
	else MarioCrouchDown = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		if (Xvelo < 5.0f && Yvelo < 0.0f) Yvelo -= 0.4f * deltaTime;
		if (Xvelo >= 5.0f && Yvelo < 0.0f) Yvelo -= 0.5f * deltaTime;
		if (Yvelo > 0.0f && !Holding) PreJump = true;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) Holding = true;
	else Holding = false;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) player_speed = 7.5f;
	else player_speed = 4.375f;
}
void MarioVertXUpdate() {
	sf::FloatRect hitbox_loop;
	bool isCollideLeftBool, isCollideRightBool;
	// Check if Mario collide with left or right
	std::pair<bool, bool> ObstacleCheck = isOrCollideSide(player, ObstaclesList, {});
	std::pair<bool, bool> BrickCheck = isOrCollideSide(player, Bricks, BrickSaveList);
	std::pair<bool, bool> LuckyCheck = isOrCollideSide(player, LuckyBlock, LuckyBlockSaveList);
	bool isTrueCollide = false;
	if (ObstacleCheck.first || ObstacleCheck.second || BrickCheck.first || BrickCheck.second || LuckyCheck.first || LuckyCheck.second) {
		// Stop on wall
		if (ObstacleCheck.first && sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || (ObstacleCheck.second && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))) {
			Xvelo = 0.0f;
		}
		else if ((ObstacleCheck.first && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || !MarioDirection)) || (ObstacleCheck.second && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || MarioDirection))) {
			Xvelo = 0.0f;
		}
		if (BrickCheck.first && sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || (BrickCheck.second && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))) {
			Xvelo = 0.0f;
		}
		else if ((BrickCheck.first && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || !MarioDirection)) || (BrickCheck.second && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || MarioDirection))) {
			Xvelo = 0.0f;
		}
		if (LuckyCheck.first && sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || (LuckyCheck.second && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))) {
			Xvelo = 0.0f;
		}
		else if ((LuckyCheck.first && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || !MarioDirection)) || (LuckyCheck.second && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || MarioDirection))) {
			Xvelo = 0.0f;
		}
		// Count if size AllCollideList equal to CollideAddCounter
		float CurrPosXCollide = 0, CurrPosYCollide = 0;
		bool isCollideSide = false;
		// 0 for right; 1 for left
		bool NoAdd = false;

		std::pair<bool, bool> ObstacleCollide, BrickCollide, LuckyCollide;
		isCollideLeftBool = false;
		isCollideRightBool = false;
		// Loop through obstacles
		ObstacleCollide = isAccurateCollideSide(player, ObstaclesList, CurrPosXCollide, CurrPosYCollide, NoAdd, {});
		BrickCollide = isAccurateCollideSide(player, Bricks, CurrPosXCollide, CurrPosYCollide, NoAdd, BrickSaveList);
		LuckyCollide = isAccurateCollideSide(player, LuckyBlock, CurrPosXCollide, CurrPosYCollide, NoAdd, LuckyBlockSaveList);
		// Break if size AllCollideList is not equal to CollideAddCounter
		//if (AllCollideList.size() != CollideAddCounter) break;
		// Adjust Position if collide
		if (ObstacleCollide.first) player.property.setPosition(CurrPosXCollide + 32.0f - 4.0f + player.property.getOrigin().x, player.property.getPosition().y);
		if (ObstacleCollide.second) player.property.setPosition(CurrPosXCollide - (1.0f + 4.0f + (23 - player.property.getOrigin().x)), player.property.getPosition().y);
		if (BrickCollide.first) player.property.setPosition(CurrPosXCollide + 32.0f - 4.0f + player.property.getOrigin().x, player.property.getPosition().y);
		if (BrickCollide.second) player.property.setPosition(CurrPosXCollide - (1.0f + 4.0f + (23 - player.property.getOrigin().x)), player.property.getPosition().y);
		if (LuckyCollide.first) player.property.setPosition(CurrPosXCollide + 32.0f - 4.0f + player.property.getOrigin().x, player.property.getPosition().y);
		if (LuckyCollide.second) player.property.setPosition(CurrPosXCollide - (1.0f + 4.0f + (23 - player.property.getOrigin().x)), player.property.getPosition().y);

		//if (!ObstacleCollide.first && !ObstacleCollide.second && !BrickCollide.first && !BrickCollide.second && !LuckyCollide.first && !LuckyCollide.second) break;
		// Break if no collide
	}
	if (!sf::Keyboard::isKeyPressed(sf::Keyboard::X) && Xvelo > 4.475f) {
		OverSpeed = true;
	}
	if (Xvelo > player_speed && !sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
		if (OverSpeed) {
			Xvelo -= 0.125f * deltaTime;
			if (Xvelo <= player_speed) OverSpeed = false;
		}
		else Xvelo = player_speed;
	}
	if (Xvelo > 7.5f && sf::Keyboard::isKeyPressed(sf::Keyboard::X)) Xvelo = 7.5f;
}
void MarioVertYUpdate() {
	std::pair<bool, std::pair<bool, bool>> ObstacleCollide, BrickCollide, LuckyCollide;
	// bottom update
	bool ObstacleCheck, BrickCheck, LuckyCheck;
	float CurrPosYCollide;
	bool NoAdd;
	ObstacleCheck = isCollideBot(player, ObstaclesList);
	BrickCheck = isCollideBot(player, Bricks);
	LuckyCheck = isCollideBot(player, LuckyBlock);
	if (Yvelo >= 0.0f && MarioCanJump) MarioCanJump = false;
	if ((!ObstacleCheck && !BrickCheck && !LuckyCheck) || MarioCanJump) {
		MarioCurrentFalling = true;
		Yvelo += (Yvelo >= 10.0f ? 0.0f : 1.0f * deltaTime);
		player.property.move(0.0f, Yvelo * deltaTime);
	}
	ObstacleCheck = isCollideBot(player, ObstaclesList);
	BrickCheck = isCollideBot(player, Bricks);
	LuckyCheck = isCollideBot(player, LuckyBlock);
	//recolide
	if (ObstacleCheck || BrickCheck || LuckyCheck) {
		MarioCurrentFalling = false;
		bool isLanding;
		NoAdd = false;
		if (Yvelo >= 0.0f) {
			isLanding = true;
			Yvelo = 0.0f;
		}
		else {
			isLanding = false;
			Yvelo = 0.0f;
		}
		ObstacleCollide = isAccurateCollideBot(player, ObstaclesList, CurrPosYCollide, NoAdd);
		BrickCollide = isAccurateCollideBot(player, Bricks, CurrPosYCollide, NoAdd);
		LuckyCollide = isAccurateCollideBot(player, LuckyBlock, CurrPosYCollide, NoAdd);
		if ((ObstacleCollide.first || BrickCollide.first || LuckyCollide.first) && isLanding) {
			if (ObstacleCollide.second.first || ObstacleCollide.second.second || BrickCollide.second.first || BrickCollide.second.second || LuckyCollide.second.first || LuckyCollide.second.second) {
				player.property.setPosition(player.property.getPosition().x, CurrPosYCollide - (52.0f - player.property.getOrigin().y + 7.0f));
			}
			if (!ObstacleCollide.second.first && !ObstacleCollide.second.second && !BrickCollide.second.first && !BrickCollide.second.second && !LuckyCollide.second.first && !LuckyCollide.second.second) {
				player.property.setPosition(player.property.getPosition().x, CurrPosYCollide - (52.0f - player.property.getOrigin().y + 7.0f));
			}
		}
	}
	// top update
	ObstacleCheck = isCollideTop(player, ObstaclesList, {});
	BrickCheck = isCollideTop(player, Bricks, BrickSaveList);
	LuckyCheck = isCollideTop(player, LuckyBlock, LuckyBlockSaveList);
	std::vector<std::pair<float, float>> BrickPos, LuckyPos;
	if ((ObstacleCheck || BrickCheck || LuckyCheck) && Yvelo < 0.0f) {
		Yvelo = 0.0f;
		CurrPosYCollide;
		NoAdd = false;
		ObstacleCollide = isAccurateCollideTop(player, ObstaclesList, CurrPosYCollide, NoAdd, {});
		BrickCollide = isAccurateCollideTop(player, Bricks, CurrPosYCollide, NoAdd, BrickSaveList);
		LuckyCollide = isAccurateCollideTop(player, LuckyBlock, CurrPosYCollide, NoAdd, LuckyBlockSaveList);
		if ((ObstacleCollide.first || BrickCollide.first || LuckyCollide.first)) {
			if (ObstacleCollide.second.first || ObstacleCollide.second.second || BrickCollide.second.first || BrickCollide.second.second || LuckyCollide.second.first || LuckyCollide.second.second) {
				if (PowerState > 0 && !MarioCrouchDown)
					player.property.setPosition(player.property.getPosition().x, CurrPosYCollide + (31.0f + player.property.getOrigin().y - PowerOffset[PowerState]));
				else if ((PowerState > 0 && MarioCrouchDown) || (PowerState == 0 && MarioAppearing) || (PowerState == 0 && !MarioCrouchDown))
					player.property.setPosition(player.property.getPosition().x, CurrPosYCollide + (31.0f + player.property.getOrigin().y - 30.0f));
			}
			if (!ObstacleCollide.second.first && !ObstacleCollide.second.second && !BrickCollide.second.first && !BrickCollide.second.second && !LuckyCollide.second.first && !LuckyCollide.second.second) {
				if (PowerState > 0 && !MarioCrouchDown)
					player.property.setPosition(player.property.getPosition().x, CurrPosYCollide + (31.0f + player.property.getOrigin().y - PowerOffset[PowerState]));
				else if ((PowerState > 0 && MarioCrouchDown) || (PowerState == 0 && MarioAppearing) || (PowerState == 0 && !MarioCrouchDown))
					player.property.setPosition(player.property.getPosition().x, CurrPosYCollide + (31.0f + player.property.getOrigin().y - 30.0f));
			}
		}
		// Start event Brick
		BrickPos = isCollideTopDetailed(player, Bricks, {});
		LuckyPos = isCollideTopDetailed(player, LuckyBlock, {});
		for (const auto& i : BrickPos) {
			HitEvent(i.first, i.second);
		}
		for (const auto& i : LuckyPos) {
			LuckyHitEvent(i.first, i.second);
		}
	}
}
void resetAnimation() {
	MarioAnimation.resetAnimationIndex("RunSmallRight");
	MarioAnimation.resetAnimationIndex("RunSmallLeft");
	MarioAnimation.resetAnimationIndex("RunBigRight");
	MarioAnimation.resetAnimationIndex("RunBigLeft");
}
void UpdateAnimation() {
	//if (PowerState == 0 && !MarioCrouchDown) {
	//	player.setHitboxMain({ 0.0f + 5.0f, 0.0f + 2.0f, 23.0f, 29.0f });
	//	player.setHitboxTop({ 1.0f + 5.0f, 0.0f + 2.0f, 22.0f, 2.0f });
	//	player.setHitboxBot({ 1.0f + 5.0f, 27.0f + 2.0f, 22.0f, 2.0f });
	//	player.setHitboxRight2({ 21.0f + 5.0f, 2.0f + 2.0f, 3.0f, 20.0f });
	//	player.setHitboxLeft2({ -1.0f + 5.0f, 2.0f + 2.0f, 3.0f, 20.0f });
	//	player.setHitboxRight({ 21.0f + 5.0f, 2.0f + 2.0f, 2.0f, 20.0f });
	//	player.setHitboxLeft({ 0.0f + 5.0f, 2.0f + 2.0f, 2.0f, 20.0f });
	//}

	player.property.setOrigin(11, 51);
	if (PowerState > 0 && !MarioCrouchDown) {
		setHitbox(player.hitboxMain, { 0.0f + 4.0f, 0.0f + PowerOffset[PowerState], 23.0f, 52.0f });
		setHitbox(player.hitboxTop, { 1.0f + 4.0f, 0.0f + PowerOffset[PowerState], 21.0f, 2.0f });
		setHitbox(player.hitboxBot, { 1.0f + 4.0f, 50.0f + PowerOffset[PowerState], 21.0f, 2.0f });
		setHitbox(player.hitboxRight2, { 21.0f + 4.0f, 2.0f + PowerOffset[PowerState], 4.0f, 43.0f });
		setHitbox(player.hitboxLeft2, { -2.0f + 4.0f, 2.0f + PowerOffset[PowerState], 4.0f, 43.0f });
		setHitbox(player.hitboxRight, { 21.0f + 4.0f, 2.0f + PowerOffset[PowerState], 2.0f, 43.0f });
		setHitbox(player.hitboxLeft, { 0.0f + 4.0f, 2.0f + PowerOffset[PowerState], 2.0f, 43.0f });
	}
	else if ((PowerState > 0 && MarioCrouchDown) || (PowerState == 0 && MarioAppearing) || (PowerState == 0 && !MarioCrouchDown)) {
		setHitbox(player.hitboxMain, { 0.0f + 4.0f, 0.0f + 30.0f, 23.0f, 29.0f }); // 30
		setHitbox(player.hitboxTop, { 1.0f + 4.0f, 0.0f + 30.0f, 21.0f, 2.0f });
		setHitbox(player.hitboxBot, { 1.0f + 4.0f, 27.0f + 30.0f, 21.0f, 2.0f });
		setHitbox(player.hitboxRight2, { 21.0f + 4.0f, 2.0f + 30.0f, 4.0f, 20.0f });
		setHitbox(player.hitboxLeft2, { -2.0f + 4.0f, 2.0f + 30.0f, 4.0f, 20.0f });
		setHitbox(player.hitboxRight, { 21.0f + 4.0f, 2.0f + 30.0f, 2.0f, 20.0f });
		setHitbox(player.hitboxLeft, { 0.0f + 4.0f, 2.0f + 30.0f, 2.0f, 20.0f });
	}
	//animation update

	//mariostate:
	// 0: idle, 1: run, 2: jump = fall, 3: crouch
	if (!MarioDirection) {
		if (!MarioAppearing) {
			if (Yvelo == 0.0f && !MarioCurrentFalling && !MarioCrouchDown) {
				if (Xvelo == 0.0f) {
					resetAnimation();
					if (PowerState == 0) MarioAnimation.update("IdleSmallLeft", player.property);
					else if (PowerState == 1) MarioAnimation.update("IdleBigLeft", player.property);
					MarioState = 0;
				}
				else {
					MarioAnimation.resetAnimationIndex("RunSmallRight");
					MarioAnimation.resetAnimationIndex("RunBigRight");
					if (PowerState == 0) {
						MarioAnimation.setAnimationFrequency("RunSmallLeft", std::max(24.0f, std::min(Xvelo * 8.0f, 75.0f)));
						MarioAnimation.update("RunSmallLeft", player.property);
					}
					else if (PowerState == 1) {
						MarioAnimation.setAnimationFrequency("RunBigLeft", std::max(24.0f, std::min(Xvelo * 8.0f, 75.0f)));
						MarioAnimation.update("RunBigLeft", player.property);
					}
					MarioState = 1;
				}
			}
			else if ((Yvelo != 0.0f || MarioCurrentFalling) && !MarioCrouchDown) {
				resetAnimation();
				if (PowerState == 0) MarioAnimation.update("JumpSmallLeft", player.property);
				else if (PowerState == 1) MarioAnimation.update("JumpBigLeft", player.property);
				MarioState = 2;
			}
			else if (MarioCrouchDown && PowerState > 0) {
				resetAnimation();
				MarioAnimation.update("DownBigLeft", player.property);
				MarioState = 3;
			}
		}
		else {
			if (PowerState == 0) MarioAnimation.update("AppearSmallLeft", player.property);
			else if (PowerState == 1) MarioAnimation.update("AppearBigLeft", player.property);
		}
	}
	else {
		if (!MarioAppearing) {
			if (Yvelo == 0.0f && !MarioCurrentFalling && !MarioCrouchDown) {
				if (Xvelo == 0.0f) {
					resetAnimation();
					if (PowerState == 0) MarioAnimation.update("IdleSmallRight", player.property);
					else if (PowerState == 1) MarioAnimation.update("IdleBigRight", player.property);
					MarioState = 0;
				}
				else {
					MarioAnimation.resetAnimationIndex("RunSmallLeft");
					MarioAnimation.resetAnimationIndex("RunBigLeft");
					if (PowerState == 0) {
						MarioAnimation.setAnimationFrequency("RunSmallRight", std::max(24.0f, std::min(Xvelo * 8.0f, 75.0f)));
						MarioAnimation.update("RunSmallRight", player.property);
					}
					else if (PowerState == 1) {
						MarioAnimation.setAnimationFrequency("RunBigRight", std::max(24.0f, std::min(Xvelo * 8.0f, 75.0f)));
						MarioAnimation.update("RunBigRight", player.property);
					}
					MarioState = 1;
				}
			}
			else if ((Yvelo != 0.0f || MarioCurrentFalling) && !MarioCrouchDown) {
				resetAnimation();
				if (PowerState == 0) MarioAnimation.update("JumpSmallRight", player.property);
				else if (PowerState == 1) MarioAnimation.update("JumpBigRight", player.property);
				MarioState = 2;
			}
			else if (MarioCrouchDown && PowerState > 0) {
				resetAnimation();
				MarioAnimation.update("DownBigRight", player.property);
				MarioState = 3;
			}
		}
		else {
			if (PowerState == 0) MarioAnimation.update("AppearSmallRight", player.property);
			else if (PowerState == 1) MarioAnimation.update("AppearBigRight", player.property);
		}
	}
}
void CheckForDeath() {
	if (isOutScreenY(player.property.getPosition().y, 64)) {
		window.close();
	}
}
void MarioSetSmall() {
	player.property.setTexture(SmallMario);
}
inline void MarioDraw() {
	// check power state here
	if (AppearingTimer.getElapsedTime().asMilliseconds() > 329.67032967032964f) MarioAppearing = false;
	if (PowerState != lastPowerState) {
		MarioAppearing = true;
		AppearingTimer.restart().asMilliseconds();
		lastPowerState = PowerState;
	}
	//then draw
	window.draw(player.property);
}