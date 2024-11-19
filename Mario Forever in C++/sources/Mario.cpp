#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <algorithm>
#include <utility>
#include <array>
#include <set>

#include "../headers/Object/Mario.hpp"
#include "../headers/Block/Obstacles.hpp"
#include "../headers/Block/Brick.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/Animate/AnimationManager.hpp"
#include "../headers/Block/LuckyBlock.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Core/Sound.hpp"
#include "../headers/Core/Level.hpp"
#include "../headers/Block/Slopes.hpp"
#include "../headers/Core/Collision/Collision.hpp"

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
int PowerState = 0;
int lastPowerState = 0;

int Lives = 4;

bool OverSpeed = false;
bool isMarioOverlapping = false;
long long int Score = 0;
sf::Clock AppearingTimer;
sf::Clock InvincibleTimer;
bool Invincible = false;
bool InvincibleState = false;
bool MarioAppearing = false;
// 0 for right; 1 for left
//texture loading
sf::Texture SmallMario;
sf::Texture BigMario;
std::array<float, 2> PowerOffset = { 30.0f, 7.0f };
int loadMarioRes() {
	AppearingTimer.restart().asMilliseconds();
	// Resources Loader;
	LoadTexture(SmallMario, SMALLMARIO_TEXTURE);
	LoadTexture(BigMario, BIGMARIO_TEXTURE);

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
				Sounds.PlaySound("Jump");
				Yvelo = -13.5f;
				MarioCanJump = true;
			}
		}
		else if (PreJump) {
			if (Yvelo == 0.0f) {
				Sounds.PlaySound("Jump");
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
	//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) player.property.move(0.0f, 1.0f * deltaTime);
	//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) player.property.move(0.0f, -1.0f * deltaTime);
	std::pair<bool, std::pair<bool, bool>> ObstacleCollide, BrickCollide, LuckyCollide, SlopeCollide;
	bool ObstacleCheck, BrickCheck, LuckyCheck, SlopeCheck;
	float CurrPosYCollide, CurrPosXCollide, ID, Diff, OldY;
	bool NoAdd;
	//bottom update (slopes)
	//SlopeCheck = false;
	//for (const auto& i : SlopesList) {
	//	if (Collision::pixelPerfectTest(player.property, i.property)) {
	//		SlopeCheck = true;
	//		break;
	//	}
	//}
	SlopeCheck = isCollideBotSlope(player);
	std::vector<std::array<float, 3>> SlopeTemp;
	if (SlopeCheck) {
		MarioCurrentFalling = false;
		bool isLanding;
		NoAdd = false;
		isMarioOverlapping = true;
		CurrPosXCollide = 0.0f, CurrPosYCollide = 0.0f;
		if (Yvelo >= 0.0f) {
			isLanding = true;
			Yvelo = 0.0f;
		}
		else if (Yvelo < 0.0f && !isMarioOverlapping) {
			isLanding = true;
			Yvelo = 0.0f;
		}
		if (Yvelo >= 0.0f) {
			SlopeTemp.clear();
			OldY = player.property.getPosition().y;
			SlopeCollide = isAccuratelyCollideBotSlope(player, CurrPosXCollide, CurrPosYCollide, NoAdd, ID, SlopeTemp);
			for (const auto& i : SlopeTemp) {
				if (true) {
					if (i[2] == 0) player.property.setPosition(player.property.getPosition().x, std::max(i[1] + i[0] + 32.0f - player.property.getPosition().x - 3.0f - (52.0f - player.property.getOrigin().y + 7.0f) * 2.25f, i[1] - (52.0f - player.property.getOrigin().y + 7.0f)));
					else if (i[2] == 1) player.property.setPosition(player.property.getPosition().x, std::max(i[1] + 0.5f * (i[0] + 32.0f - player.property.getPosition().x - 3.0f + (52.0f - player.property.getOrigin().y + 5.0f) * 1.15f), i[1] - (52.0f - player.property.getOrigin().y + 5.0f) + 16.0f));
					else if (i[2] == 2) player.property.setPosition(player.property.getPosition().x, std::max(i[1] + 0.5f * (i[0] + 32.0f - player.property.getPosition().x - 3.0f + (52.0f - player.property.getOrigin().y + 5.0f) * 1.15f) - 16.0f, i[1] - (52.0f - player.property.getOrigin().y + 5.0f)));
				}
			}
			//Diff = abs(player.property.getPosition().y - OldY);
			//std::cout << Diff << "\n";
			//SlopeCheck = isCollideBot(player, SlopesList);
			//if (!SlopeCheck) player.property.move(0.0f, -Diff);
		}
	}
	else isMarioOverlapping = false;
	// bottom update (obstacles)
	ObstacleCheck = isCollideBot(player, ObstaclesList);
	BrickCheck = isCollideBot(player, Bricks);
	LuckyCheck = isCollideBot(player, LuckyBlock);
	//SlopeCheck = false;
	//for (const auto& i : SlopesList) {
	//	if (Collision::pixelPerfectTest(player.property, i.property)) {
	//		SlopeCheck = true;
	//		break;
	//	}
	//}
	if (Yvelo >= 0.0f && MarioCanJump) MarioCanJump = false;
	if ((!ObstacleCheck && !BrickCheck && !LuckyCheck && !SlopeCheck) || MarioCanJump) {
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
		setHitbox(player.hitboxBot2, { 1.0f + 4.0f, 50.0f + PowerOffset[PowerState], 21.0f, 3.0f });
		setHitbox(player.hitboxRight2, { 21.0f + 4.0f, 2.0f + PowerOffset[PowerState], 4.0f, 43.0f });
		setHitbox(player.hitboxLeft2, { -2.0f + 4.0f, 2.0f + PowerOffset[PowerState], 4.0f, 43.0f });
		setHitbox(player.hitboxRight, { 21.0f + 4.0f, 2.0f + PowerOffset[PowerState], 2.0f, 43.0f });
		setHitbox(player.hitboxLeft, { 0.0f + 4.0f, 2.0f + PowerOffset[PowerState], 2.0f, 43.0f });
	}
	else if ((PowerState > 0 && MarioCrouchDown) || (PowerState == 0 && MarioAppearing) || (PowerState == 0 && !MarioCrouchDown)) {
		setHitbox(player.hitboxMain, { 0.0f + 4.0f, 0.0f + 30.0f, 23.0f, 29.0f }); // 30
		setHitbox(player.hitboxTop, { 1.0f + 4.0f, 0.0f + 30.0f, 21.0f, 2.0f });
		setHitbox(player.hitboxBot, { 1.0f + 4.0f, 27.0f + 30.0f, 21.0f, 2.0f });
		setHitbox(player.hitboxBot2, { 1.0f + 4.0f, 27.0f + 30.0f, 21.0f, 3.0f });
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
						MarioAnimation.setAnimationFrequency("RunSmallLeft", f_max(24.0f, f_min(Xvelo * 8.0f, 75.0f)));
						MarioAnimation.update("RunSmallLeft", player.property);
					}
					else if (PowerState == 1) {
						MarioAnimation.setAnimationFrequency("RunBigLeft", f_max(24.0f, f_min(Xvelo * 8.0f, 75.0f)));
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
						MarioAnimation.setAnimationFrequency("RunSmallRight", f_max(24.0f, f_min(Xvelo * 8.0f, 75.0f)));
						MarioAnimation.update("RunSmallRight", player.property);
					}
					else if (PowerState == 1) {
						MarioAnimation.setAnimationFrequency("RunBigRight", f_max(24.0f, f_min(Xvelo * 8.0f, 75.0f)));
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
void PowerDown() {
	if (!Invincible) {
		if (PowerState == 1) {
			Sounds.PlaySound("Pipe");
			PowerState = 0;
			Invincible = true;
			InvincibleTimer.restart().asSeconds();
			InvincibleState = false;
		}
		else if (PowerState == 0) Death();
	}
}
void Death() {
	if (Lives <= 0) window.close();
	else --Lives;
	Objectbuilding();
	Xvelo = 0.0f;
	Yvelo = 0.0f;
	PowerState = 0;
	lastPowerState = 0;
}
void CheckForDeath() {
	if (isOutScreenY(player.property.getPosition().y, 64)) {
		Death();
	}
}
inline void MarioDraw() {
	// check power state here
	if (AppearingTimer.getElapsedTime().asMilliseconds() > (3000.0f / 91.0f) * 15.0f) MarioAppearing = false;
	if (PowerState != lastPowerState) {
		MarioAppearing = true;
		AppearingTimer.restart().asMilliseconds();
		lastPowerState = PowerState;
	}
	//then draw
	if (InvincibleTimer.getElapsedTime().asSeconds() > 2.0f) Invincible = false;
	if (!Invincible) window.draw(player.property);
	else {
		if (!InvincibleState) {
			window.draw(player.property);
			InvincibleState = true;
		}
		else InvincibleState = false;
	}
}