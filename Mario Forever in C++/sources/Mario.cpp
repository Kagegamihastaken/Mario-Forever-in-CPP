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

//define here
AnimationManager MarioAnimation;
Mario player;
float Xvelo = 0.0f;
float Yvelo = 0.0f;
int PowerState = 0;
bool MarioDirection;
bool MarioCanJump = false;
bool MarioCurrentFalling = false;
bool PreJump = false;
bool Holding;
float player_speed;
int MarioState = 0;
// 0 for right; 1 for left
//texture loading
sf::Texture SmallMario;
sf::SoundBuffer jumpSoundBuffer;
//Sound specific
sf::Sound SoundJump;
//Function For Checking Collision
//X
bool isPlayerCollideLeft2(std::vector<Obstacles>& OL) {
	sf::FloatRect hitbox_loop;
	for (const auto& i : OL) {
		hitbox_loop = i.getGlobalHitbox();
		if (player.isCollideLeft2(hitbox_loop)) return true;
	}
	return false;
}
bool isPlayerCollideRight2(std::vector<Obstacles>& OL) {
	sf::FloatRect hitbox_loop;
	for (const auto& i : OL) {
		hitbox_loop = i.getGlobalHitbox();
		if (player.isCollideRight2(hitbox_loop)) return true;
	}
	return false;
}
std::pair<bool, bool> isPlayerOrCollideSide(std::vector<Obstacles>& OL) {
	sf::FloatRect hitbox_loop;
	bool isLeft = false, isRight = false;
	for (const auto& i : OL) {
		hitbox_loop = i.getGlobalHitbox();
		if (player.isCollideRight2(hitbox_loop)) isRight = true;
		if (player.isCollideLeft2(hitbox_loop)) isLeft = true;
		if (isRight && isLeft) break;
	}
	return { isRight, isLeft };
}
std::pair<bool, bool> isPlayerAccurateCollideSide(std::vector<Obstacles>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, int& CollideAddCounter, std::set<std::pair<int, int>>& AllCollideList) {
	bool isCollideLeftBool = false, isCollideRightBool = false;
	sf::FloatRect hitbox_loop;
	for (const auto& i : OL) {
		hitbox_loop = i.getGlobalHitbox();
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
bool isPlayerCollideBot(std::vector<Obstacles>& OL) {
	sf::FloatRect hitbox_loop;
	for (const auto& i : OL) {
		hitbox_loop = i.getGlobalHitbox();
		if (player.isCollideBot(hitbox_loop)) return true;
	}
	return false;
}
std::pair<bool, std::pair<bool, bool>> isPlayerAccurateCollideBot(std::vector<Obstacles>& OL) {
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
bool isPlayerCollideTop(std::vector<Obstacles>& OL) {
	sf::FloatRect hitbox_loop;
	for (const auto& i : OL) {
		hitbox_loop = i.getGlobalHitbox();
		if (player.isCollideTop(hitbox_loop)) return true;
	}
	return false;
}
std::vector<std::pair<float, float>> isPlayerCollideTopDetailed(std::vector<Obstacles>& OL) {
	std::vector<std::pair<float, float>> result;
	sf::FloatRect hitbox_loop;
	for (const auto& i : OL) {
		hitbox_loop = i.getGlobalHitbox();
		if (player.isCollideTop(hitbox_loop)) result.push_back({ hitbox_loop.getPosition().x, hitbox_loop.getPosition().y });
	}
	return result;
}
std::pair<bool, std::pair<bool, bool>> isPlayerAccurateCollideTop(std::vector<Obstacles>& OL) {
	bool isCollideTopBool = false, isCollideRight = false, isCollideLeft = false;
	sf::FloatRect hitbox_loop;
	for (const auto& i : OL) {
		hitbox_loop = i.getGlobalHitbox();
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
	// Resources Loader;
	if (!SmallMario.loadFromFile("data/resources/SmallMario.png")) {
		std::cout << "Cannot load data/resources/SmallMario.png" << "\n";
	}

	// Sound Loader
	if (!jumpSoundBuffer.loadFromFile("data/sounds/jump.wav")) {
		std::cout << "Cannot load data/sounds/jump.wav" << "\n";
	}

	//set Texture
	//left
	MarioAnimation.addAnimation("RunLeft", SmallMario, { 4,1 }, { 31,31 }, { 0,0 }, 0, { 0,0 }, { 3,0 });
	MarioAnimation.addAnimation("IdleLeft", SmallMario, { 4,1 }, { 31,31 }, { 2,0 }, 0, { 2,0 }, { 3,0 });
	MarioAnimation.addAnimation("JumpLeft", SmallMario, { 4,1 }, { 31,31 }, { 3,0 }, 0, { 3,0 }, { 4,0 });

	//right
	MarioAnimation.addAnimation("RunRight", SmallMario, { 4,1 }, { 31,31 }, { 0,1 }, 0, { 0,1 }, { 3,1 });
	MarioAnimation.addAnimation("IdleRight", SmallMario, { 4,1 }, { 31,31 }, { 2,1 }, 0, { 2,1 }, { 3,1 });
	MarioAnimation.addAnimation("JumpRight", SmallMario, { 4,1 }, { 31,31 }, { 3,1 }, 0, { 3,1 }, { 4,1 });

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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		if (Xvelo == 0) MarioDirection = true;
		else if (!MarioDirection) {
			Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.375f * deltaTime);
			player.property.move(Xvelo * deltaTime, 0.0f);
		}
		//init speed
		if (Xvelo < 1.0f && MarioDirection) Xvelo = 1.0f;
		if (!isPlayerCollideLeft2(ObstaclesList) && !isPlayerCollideLeft2(Bricks)) {
			if (MarioDirection) {
				Xvelo += (Xvelo > player_speed ? 0.0f : 0.125f * deltaTime);
				player.property.move((0 - Xvelo) * deltaTime, 0.0f);
			}
		}
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		if (Xvelo == 0) MarioDirection = false;
		else if (MarioDirection) {
			Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.375f * deltaTime);
			player.property.move((0 - Xvelo) * deltaTime, 0.0f);
		}
		//init speed
		if (Xvelo < 1.0f && !MarioDirection) Xvelo = 1.0f;
		if (!isPlayerCollideRight2(ObstaclesList) && !isPlayerCollideRight2(Bricks)) {
			if (!MarioDirection) {
				//Xvelo += (Xvelo > player_speed ? 0.0f : 0.125f * deltaTime);
				Xvelo += (Xvelo > player_speed ? 0.0f : 0.125f * deltaTime);
				player.property.move(Xvelo * deltaTime, 0.0f);
			}
		}
	}
	if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))) {
		Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.125f * deltaTime);
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
	std::pair<bool, bool> BrickCheck = isPlayerOrCollideSide(Bricks);
	bool isTrueCollide = false;
	if (ObstacleCheck.first || ObstacleCheck.second || BrickCheck.first || BrickCheck.second) {
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
		std::set<std::pair<int, int>> AllCollideList;
		// Count if size AllCollideList equal to CollideAddCounter
		int CollideAddCounter = 0;
		float CurrPosXCollide = 0, CurrPosYCollide = 0;
		bool isCollideSide = false;
		// 0 for right; 1 for left
		bool NoAdd = false;
		std::pair<bool, bool> ObstacleCollide, BrickCollide;
		while (true) {
			isCollideLeftBool = false;
			isCollideRightBool = false;
			// Loop through obstacles
			ObstacleCollide = isPlayerAccurateCollideSide(ObstaclesList, CurrPosXCollide, CurrPosYCollide, NoAdd, CollideAddCounter, AllCollideList);
			BrickCollide = isPlayerAccurateCollideSide(ObstaclesList, CurrPosXCollide, CurrPosYCollide, NoAdd, CollideAddCounter, AllCollideList);
			// Break if size AllCollideList is not equal to CollideAddCounter
			if (AllCollideList.size() != CollideAddCounter) break;
			// Adjust Position if collide
			if (ObstacleCollide.first) player.property.move(1.0f * deltaTime, 0.0f);
			if (ObstacleCollide.second) player.property.move(-1.0f * deltaTime, 0.0f);
			if (BrickCollide.first) player.property.move(1.0f * deltaTime, 0.0f);
			if (BrickCollide.second) player.property.move(-1.0f * deltaTime, 0.0f);

			if (!ObstacleCollide.first && !ObstacleCollide.second && !BrickCollide.first && !BrickCollide.second) break;
			// Break if no collide
		}
	}
	if (Xvelo > player_speed) Xvelo -= 0.125f * deltaTime;
}
std::pair<bool, bool> CheckTest() {
	bool isLeftCollide = false;
	bool isRightCollide = false;
	sf::FloatRect hitbox_loop;
	for (const auto& i : ObstaclesList) {
		hitbox_loop = i.getGlobalHitbox();
		if (player.isCollideLeft2(hitbox_loop)) {
			isLeftCollide = true;
		}
		if (player.isCollideRight2(hitbox_loop)) {
			isRightCollide = true;
		}
		if (isLeftCollide && isRightCollide) break;
	}
	return { isLeftCollide, isRightCollide };
}
void MarioVertYUpdate() {
	std::pair<bool, std::pair<bool, bool>> ObstacleCollide, BrickCollide;
	int UpTimeCounter = 0;
	// bottom update
	bool ObstacleCheck, BrickCheck;
	ObstacleCheck = isPlayerCollideBot(ObstaclesList);
	BrickCheck = isPlayerCollideBot(Bricks);
	if (Yvelo >= 0.0f && MarioCanJump) MarioCanJump = false;
	if ((!ObstacleCheck && !BrickCheck) || MarioCanJump) {
		MarioCurrentFalling = true;
		UpTimeCounter = 0;
		Yvelo += (Yvelo >= 10.0f ? 0.0f : 1.0f * deltaTime);
		player.property.move(0.0f, Yvelo * deltaTime);
	}
	ObstacleCheck = isPlayerCollideBot(ObstaclesList);
	BrickCheck = isPlayerCollideBot(Bricks);
	//recolide
	if (ObstacleCheck || BrickCheck) {
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
			if ((ObstacleCollide.first || BrickCollide.first) && isLanding) {
				if (ObstacleCollide.second.first || ObstacleCollide.second.second || BrickCollide.second.first || BrickCollide.second.second) {
					++UpTimeCounter;
					player.property.move(0.0f, -1.0f * deltaTime);
				}
				if (!ObstacleCollide.second.first && !ObstacleCollide.second.second && !BrickCollide.second.first && !BrickCollide.second.second) {
					++UpTimeCounter;
					player.property.move(0.0f, -1.0f * deltaTime);
				}
			}
			else if ((!ObstacleCollide.first && !BrickCollide.first)) {
				if (UpTimeCounter != 0) player.property.move(0.0f, 1.0f * deltaTime);
				break;
			}
			else break;
		}
	}
	// top update
	ObstacleCheck = isPlayerCollideTop(ObstaclesList);
	BrickCheck = isPlayerCollideTop(Bricks);
	std::vector<std::pair<float, float>> BrickPos;
	if ((ObstacleCheck || BrickCheck) && Yvelo < 0.0f) {
		Yvelo = 0.0f;
		UpTimeCounter = 0;
		while (true) {
			ObstacleCollide = isPlayerAccurateCollideTop(ObstaclesList);
			BrickCollide = isPlayerAccurateCollideTop(Bricks);
			if ((ObstacleCollide.first || BrickCollide.first)) {
				if (ObstacleCollide.second.first || ObstacleCollide.second.second || BrickCollide.second.first || BrickCollide.second.second) {
					++UpTimeCounter;
					player.property.move(0.0f, 1.0f * deltaTime);
				}
				if (!ObstacleCollide.second.first && !ObstacleCollide.second.second && !BrickCollide.second.first && !BrickCollide.second.second) {
					++UpTimeCounter;
					player.property.move(0.0f, 1.0f * deltaTime);
				}
			}
			else if ((!ObstacleCollide.first && !BrickCollide.first)) {
				if (UpTimeCounter != 0) player.property.move(0.0f, -1.0f * deltaTime);
				break;
			}
			else break;
		}
		// Start event Brick
		BrickPos = isPlayerCollideTopDetailed(Bricks);
		for (const auto& i : BrickPos) {
			HitEvent(i.first, i.second);
		}
	}
}
void UpdateAnimation() {
	player.setHitboxMain({ 0.0f + 5.0f, 0.0f + 2.0f, 23.0f, 29.0f });
	player.setHitboxTop({ 1.0f + 5.0f, 0.0f + 2.0f, 22.0f, 2.0f });
	player.setHitboxBot({ 1.0f + 5.0f, 27.0f + 2.0f, 22.0f, 2.0f });
	player.setHitboxRight2({ 21.0f + 5.0f, 2.0f + 2.0f, 3.0f, 20.0f });
	player.setHitboxLeft2({ -1.0f + 5.0f, 2.0f + 2.0f, 3.0f, 20.0f });
	player.setHitboxRight({ 21.0f + 5.0f, 2.0f + 2.0f, 2.0f, 20.0f });
	player.setHitboxLeft({ 0.0f + 5.0f, 2.0f + 2.0f, 2.0f, 20.0f });
	//animation update

	//mariostate:
	// 0: idle, 1: run, 2: jump
	if (!MarioDirection) {
		if (Yvelo == 0.0f && !MarioCurrentFalling) {
			if (Xvelo == 0.0f) {
				MarioAnimation.update("IdleLeft", player.property);
				MarioAnimation.resetAnimationIndex("RunLeft");
				MarioState = 0;
			}
			else {
				MarioAnimation.setAnimationFrequency("RunLeft", std::max(12.0f, std::min(Xvelo * 8.0f, 60.0f)));
				MarioAnimation.update("RunLeft", player.property);
				MarioState = 1;
			}
		}
		else {
			MarioAnimation.update("JumpLeft", player.property);
			MarioState = 2;
		}
	}
	else {
		if (Yvelo == 0.0f && !MarioCurrentFalling) {
			if (Xvelo == 0.0f) {
				MarioAnimation.resetAnimationIndex("RunRight");
				MarioAnimation.update("IdleRight", player.property);
				MarioState = 0;
			}
			else {
				MarioAnimation.setAnimationFrequency("RunRight", std::max(12.0f, std::min(Xvelo * 8.0f, 60.0f)));
				MarioAnimation.update("RunRight", player.property);
				MarioState = 1;
			}
		}
		else {
			MarioAnimation.update("JumpRight", player.property);
			MarioState = 2;
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
void MarioDraw() {
	window.draw(player.property);
}