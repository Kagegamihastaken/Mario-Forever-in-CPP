#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <algorithm>
#include <utility>
#include <set>

#include "../headers/Mario.hpp"
#include "../headers/Obstacles.hpp"
#include "../headers/Brick.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/AnimationManager.hpp"
#include "../headers/LuckyBlock.hpp"
#include "../headers/Loading.hpp"

#include "../resource.h"

//define here
AnimationManager MarioAnimation;
Mario player;
float Xvelo = 0.0f;
float Yvelo = 0.0f;
bool MarioDirection;
bool MarioCanJump = false;
bool MarioCurrentFalling = false;
bool PreJump = false;
bool Holding;
bool MarioCrouchDown = false;
float player_speed;
int MarioState = 0;
int PowerState = 1;
int lastPowerState = 1;
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
std::vector<float> PowerOffset = { 30.0f, 7.0f };
//Function For Checking Collision
//X
bool isPlayerCollideLeft2(const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList = {}) {
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = OL[i].getGlobalHitbox();
		if (SaveList.size() > 0) {
			hitbox_loop.top = SaveList[i].second;
			hitbox_loop.left = SaveList[i].first;
		}
		if (player.isCollideLeft2(hitbox_loop)) return true;
	}
	return false;
}
bool isPlayerCollideRight2(const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList = {}) {
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = OL[i].getGlobalHitbox();
		if (SaveList.size() > 0) {
			hitbox_loop.top = SaveList[i].second;
			hitbox_loop.left = SaveList[i].first;
		}
		if (player.isCollideRight2(hitbox_loop)) return true;
	}
	return false;
}
std::pair<bool, bool> isPlayerOrCollideSide(const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList = {}) {
	sf::FloatRect hitbox_loop;
	bool isLeft = false, isRight = false;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = OL[i].getGlobalHitbox();
		if (SaveList.size() > 0) {
			hitbox_loop.top = SaveList[i].second;
			hitbox_loop.left = SaveList[i].first;
		}
		if (player.isCollideRight2(hitbox_loop)) isRight = true;
		if (player.isCollideLeft2(hitbox_loop)) isLeft = true;
		if (isRight && isLeft) break;
	}
	return { isRight, isLeft };
}
std::pair<bool, bool> isPlayerAccurateCollideSide(const std::vector<Obstacles>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, int& CollideAddCounter, std::set<std::pair<int, int>>& AllCollideList, const std::vector<std::pair<float, float>>& SaveList = {}) {
	bool isCollideLeftBool = false, isCollideRightBool = false;
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = OL[i].getGlobalHitbox();
		if (SaveList.size() > 0) {
			hitbox_loop.top = SaveList[i].second;
			hitbox_loop.left = SaveList[i].first;
		}
		// Check if collide
		if (player.isCollideLeft(hitbox_loop)) {
			isCollideLeftBool = true;
			if (CurrPosXCollide != hitbox_loop.getPosition().x || CurrPosYCollide != hitbox_loop.getPosition().y) {
				if (!NoAdd) {
					CurrPosXCollide = hitbox_loop.getPosition().x;
					CurrPosYCollide = hitbox_loop.getPosition().y;
					NoAdd = true;
				}
				else {
					AllCollideList.insert({ CurrPosXCollide, CurrPosYCollide });
					++CollideAddCounter;
				}
			}
			break;
		}
		if (player.isCollideRight(hitbox_loop)) {
			isCollideRightBool = true;
			if (CurrPosXCollide != hitbox_loop.getPosition().x || CurrPosYCollide != hitbox_loop.getPosition().y) {
				if (!NoAdd) {
					CurrPosXCollide = hitbox_loop.getPosition().x;
					CurrPosYCollide = hitbox_loop.getPosition().y;
					NoAdd = true;
				}
				else {
					AllCollideList.insert({ CurrPosXCollide, CurrPosYCollide });
					++CollideAddCounter;
				}
			}
			break;
		}
	}
	return { isCollideLeftBool, isCollideRightBool };
}
//Y
bool isPlayerCollideBot(const std::vector<Obstacles>& OL) {
	sf::FloatRect hitbox_loop;
	for (const auto& i : OL) {
		hitbox_loop = i.getGlobalHitbox();
		if (player.isCollideBot(hitbox_loop)) return true;
	}
	return false;
}
std::pair<bool, std::pair<bool, bool>> isPlayerAccurateCollideBot(const std::vector<Obstacles>& OL) {
	bool isCollideBotBool = false, isCollideRight = false, isCollideLeft = false;
	sf::FloatRect hitbox_loop;
	for (const auto& i : OL) {
		hitbox_loop = i.getGlobalHitbox();
		if (player.isCollideBot(hitbox_loop)) {
			isCollideBotBool = true;
		}
		if (player.isCollideRight(hitbox_loop)) isCollideRight = true;
		if (player.isCollideLeft(hitbox_loop)) isCollideLeft = true;
		if (isCollideBotBool && isCollideRight && isCollideLeft) break;
	}
	return { isCollideBotBool, { isCollideRight, isCollideLeft } };
}
bool isPlayerCollideTop(const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList = {}) {
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = OL[i].getGlobalHitbox();
		if (SaveList.size() > 0) hitbox_loop.top = SaveList[i].second;
		if (player.isCollideTop(hitbox_loop)) return true;
	}
	return false;
}
std::vector<std::pair<float, float>> isPlayerCollideTopDetailed(const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList = {}) {
	std::vector<std::pair<float, float>> result;
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = OL[i].getGlobalHitbox();
		if (SaveList.size() > 0) hitbox_loop.top = SaveList[i].second;
		if (player.isCollideTop(hitbox_loop)) result.push_back({ hitbox_loop.getPosition().x, hitbox_loop.getPosition().y });
	}
	return result;
}
std::pair<bool, std::pair<bool, bool>> isPlayerAccurateCollideTop(const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList = {}) {
	bool isCollideTopBool = false, isCollideRight = false, isCollideLeft = false;
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = OL[i].getGlobalHitbox();
		if (SaveList.size() > 0) hitbox_loop.top = SaveList[i].second;
		if (player.isCollideTop(hitbox_loop)) {
			isCollideTopBool = true;
		}
		if (player.isCollideRight(hitbox_loop)) isCollideRight = true;
		if (player.isCollideLeft(hitbox_loop)) isCollideLeft = true;
		if (isCollideTopBool && isCollideRight && isCollideLeft) break;
	}
	return { isCollideTopBool, { isCollideRight, isCollideLeft } };
}
//back to normal
int loadMarioRes() {
	AppearingTimer.restart().asMilliseconds();
	// Resources Loader;
	LoadTexture(SmallMario, SMALLMARIO_TEXTURE, IMAGEFILE);
	LoadTexture(BigMario, BIGMARIO_TEXTURE, IMAGEFILE);

	// Sound Loader
	LoadAudio(jumpSoundBuffer, JUMP_SOUND, SOUNDFILE);

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
		if (!isPlayerCollideLeft2(ObstaclesList) && !isPlayerCollideLeft2(Bricks, BrickSaveList) && !isPlayerCollideLeft2(LuckyBlock, LuckyBlockSaveList)) {
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
		if (!isPlayerCollideRight2(ObstaclesList) && !isPlayerCollideRight2(Bricks, BrickSaveList) && !isPlayerCollideRight2(LuckyBlock, LuckyBlockSaveList)) {
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
	std::pair<bool, bool> ObstacleCheck = isPlayerOrCollideSide(ObstaclesList);
	std::pair<bool, bool> BrickCheck = isPlayerOrCollideSide(Bricks, BrickSaveList);
	std::pair<bool, bool> LuckyCheck = isPlayerOrCollideSide(LuckyBlock, LuckyBlockSaveList);
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
		std::set<std::pair<int, int>> AllCollideList;
		// Count if size AllCollideList equal to CollideAddCounter
		int CollideAddCounter = 0;
		float CurrPosXCollide = 0, CurrPosYCollide = 0;
		bool isCollideSide = false;
		// 0 for right; 1 for left
		bool NoAdd = false;
		std::pair<bool, bool> ObstacleCollide, BrickCollide, LuckyCollide;
		while (true) {
			isCollideLeftBool = false;
			isCollideRightBool = false;
			// Loop through obstacles
			ObstacleCollide = isPlayerAccurateCollideSide(ObstaclesList, CurrPosXCollide, CurrPosYCollide, NoAdd, CollideAddCounter, AllCollideList);
			BrickCollide = isPlayerAccurateCollideSide(Bricks, CurrPosXCollide, CurrPosYCollide, NoAdd, CollideAddCounter, AllCollideList, BrickSaveList);
			LuckyCollide = isPlayerAccurateCollideSide(LuckyBlock, CurrPosXCollide, CurrPosYCollide, NoAdd, CollideAddCounter, AllCollideList, LuckyBlockSaveList);
			// Break if size AllCollideList is not equal to CollideAddCounter
			if (AllCollideList.size() != CollideAddCounter) break;
			// Adjust Position if collide
			if (ObstacleCollide.first) player.property.move(1.0f * deltaTime, 0.0f);
			if (ObstacleCollide.second) player.property.move(-1.0f * deltaTime, 0.0f);
			if (BrickCollide.first) player.property.move(1.0f * deltaTime, 0.0f);
			if (BrickCollide.second) player.property.move(-1.0f * deltaTime, 0.0f);
			if (LuckyCollide.first) player.property.move(1.0f * deltaTime, 0.0f);
			if (LuckyCollide.second) player.property.move(-1.0f * deltaTime, 0.0f);

			if (!ObstacleCollide.first && !ObstacleCollide.second && !BrickCollide.first && !BrickCollide.second && !LuckyCollide.first && !LuckyCollide.second) break;
			// Break if no collide
		}
	}
	if (Xvelo > player_speed) Xvelo -= 0.125f * deltaTime;
}
void MarioVertYUpdate() {
	std::pair<bool, std::pair<bool, bool>> ObstacleCollide, BrickCollide, LuckyCollide;
	int UpTimeCounter = 0;
	// bottom update
	bool ObstacleCheck, BrickCheck, LuckyCheck;
	ObstacleCheck = isPlayerCollideBot(ObstaclesList);
	BrickCheck = isPlayerCollideBot(Bricks);
	LuckyCheck = isPlayerCollideBot(LuckyBlock);
	if (Yvelo >= 0.0f && MarioCanJump) MarioCanJump = false;
	if ((!ObstacleCheck && !BrickCheck && !LuckyCheck) || MarioCanJump) {
		MarioCurrentFalling = true;
		UpTimeCounter = 0;
		Yvelo += (Yvelo >= 10.0f ? 0.0f : 1.0f * deltaTime);
		player.property.move(0.0f, Yvelo * deltaTime);
	}
	ObstacleCheck = isPlayerCollideBot(ObstaclesList);
	BrickCheck = isPlayerCollideBot(Bricks);
	LuckyCheck = isPlayerCollideBot(LuckyBlock);
	//recolide
	if (ObstacleCheck || BrickCheck || LuckyCheck) {
		MarioCurrentFalling = false;
		bool isLanding;
		UpTimeCounter = 0;
		if (Yvelo > 0.0f) {
			Yvelo = 0.0f;
			isLanding = true;
		}
		else isLanding = false;
		while (true) {
			ObstacleCollide = isPlayerAccurateCollideBot(ObstaclesList);
			BrickCollide = isPlayerAccurateCollideBot(Bricks);
			LuckyCollide = isPlayerAccurateCollideBot(LuckyBlock);
			if ((ObstacleCollide.first || BrickCollide.first || LuckyCollide.first) && isLanding) {
				if (ObstacleCollide.second.first || ObstacleCollide.second.second || BrickCollide.second.first || BrickCollide.second.second || LuckyCollide.second.first || LuckyCollide.second.second) {
					++UpTimeCounter;
					player.property.move(0.0f, -1.0f * deltaTime);
				}
				if (!ObstacleCollide.second.first && !ObstacleCollide.second.second && !BrickCollide.second.first && !BrickCollide.second.second && !LuckyCollide.second.first && !LuckyCollide.second.second) {
					++UpTimeCounter;
					player.property.move(0.0f, -1.0f * deltaTime);
				}
			}
			else if (!ObstacleCollide.first && !BrickCollide.first && !LuckyCollide.first) {
				if (UpTimeCounter != 0) player.property.move(0.0f, 1.0f * deltaTime);
				break;
			}
			else break;
		}
	}
	// top update
	ObstacleCheck = isPlayerCollideTop(ObstaclesList);
	BrickCheck = isPlayerCollideTop(Bricks, BrickSaveList);
	LuckyCheck = isPlayerCollideTop(LuckyBlock, LuckyBlockSaveList);
	std::vector<std::pair<float, float>> BrickPos, LuckyPos;
	if ((ObstacleCheck || BrickCheck || LuckyCheck) && Yvelo < 0.0f) {
		Yvelo = 0.0f;
		UpTimeCounter = 0;
		while (true) {
			ObstacleCollide = isPlayerAccurateCollideTop(ObstaclesList);
			BrickCollide = isPlayerAccurateCollideTop(Bricks, BrickSaveList);
			LuckyCollide = isPlayerAccurateCollideTop(LuckyBlock, LuckyBlockSaveList);
			if ((ObstacleCollide.first || BrickCollide.first || LuckyCollide.first)) {
				if (ObstacleCollide.second.first || ObstacleCollide.second.second || BrickCollide.second.first || BrickCollide.second.second || LuckyCollide.second.first || LuckyCollide.second.second) {
					++UpTimeCounter;
					player.property.move(0.0f, 1.0f * deltaTime);
				}
				if (!ObstacleCollide.second.first && !ObstacleCollide.second.second && !BrickCollide.second.first && !BrickCollide.second.second && !LuckyCollide.second.first && !LuckyCollide.second.second) {
					++UpTimeCounter;
					player.property.move(0.0f, 1.0f * deltaTime);
				}
			}
			else if (!ObstacleCollide.first && !BrickCollide.first && !LuckyCollide.first) {
				if (UpTimeCounter != 0) player.property.move(0.0f, -1.0f * deltaTime);
				break;
			}
			else break;
		}
		// Start event Brick
		BrickPos = isPlayerCollideTopDetailed(Bricks);
		LuckyPos = isPlayerCollideTopDetailed(LuckyBlock);
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
	if (PowerState > 0 && !MarioCrouchDown) {
		player.setHitboxMain({ 0.0f + 4.0f, 0.0f + PowerOffset[PowerState], 23.0f, 52.0f });
		player.setHitboxTop({ 1.0f + 4.0f, 0.0f + PowerOffset[PowerState], 22.0f, 2.0f });
		player.setHitboxBot({ 1.0f + 4.0f, 50.0f + PowerOffset[PowerState], 22.0f, 2.0f });
		player.setHitboxRight2({ 21.0f + 4.0f, 2.0f + PowerOffset[PowerState], 3.0f, 43.0f });
		player.setHitboxLeft2({ -1.0f + 4.0f, 2.0f + PowerOffset[PowerState], 3.0f, 43.0f });
		player.setHitboxRight({ 21.0f + 4.0f, 2.0f + PowerOffset[PowerState], 2.0f, 43.0f });
		player.setHitboxLeft({ 0.0f + 4.0f, 2.0f + PowerOffset[PowerState], 2.0f, 43.0f });
	}
	else if ((PowerState > 0 && MarioCrouchDown) || (PowerState == 0 && MarioAppearing) || (PowerState == 0 && !MarioCrouchDown)) {
		player.setHitboxMain({ 0.0f + 4.0f, 0.0f + 30.0f, 23.0f, 29.0f });
		player.setHitboxTop({ 1.0f + 4.0f, 0.0f + 30.0f, 22.0f, 2.0f });
		player.setHitboxBot({ 1.0f + 4.0f, 27.0f + 30.0f, 22.0f, 2.0f });
		player.setHitboxRight2({ 21.0f + 4.0f, 2.0f + 30.0f, 3.0f, 20.0f });
		player.setHitboxLeft2({ -1.0f + 4.0f, 2.0f + 30.0f, 3.0f, 20.0f });
		player.setHitboxRight({ 21.0f + 4.0f, 2.0f + 30.0f, 2.0f, 20.0f });
		player.setHitboxLeft({ 0.0f + 4.0f, 2.0f + 30.0f, 2.0f, 20.0f });
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