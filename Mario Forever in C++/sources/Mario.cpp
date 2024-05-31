#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <algorithm>
#include <set>

#include "../headers/Mario.hpp"
#include "../headers/Obstacles.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/AnimationManager.hpp"

//define
AnimationManager MarioAnimation;
Mario player;
float Xvelo = 0.0f;
float Yvelo = 0.0f;
bool MarioDirection;
bool MarioCanJump = false;
bool MarioCurrentFalling = false;
bool PreJump = false;
bool Holding;
float player_speed;
// 0 for right; 1 for left
//texture loading
sf::Texture SmallMario;
sf::SoundBuffer jumpSoundBuffer;
//Sound specific
sf::Sound SoundJump;
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
		isCollideSideBool = false;
		for (const auto& i : ObstaclesList) {
			hitbox_loop = i.getGlobalHitbox();
			if (player.isCollideLeft2(hitbox_loop)) {
				isCollideSideBool = true;
				break;
			}
		}
		if (!isCollideSideBool) {
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
		isCollideSideBool = false;
		for (const auto& i : ObstaclesList) {
			hitbox_loop = i.getGlobalHitbox();
			if (player.isCollideRight2(hitbox_loop)) {
				isCollideSideBool = true;
				break;
			}
		}
		if (!isCollideSideBool) {
			if (!MarioDirection) {
				//Xvelo += (Xvelo > player_speed ? 0.0f : 0.125f * deltaTime);
				Xvelo += (Xvelo > player_speed ? 0.0f : 0.125f * deltaTime);
				player.property.move(Xvelo * deltaTime, 0.0f);
			}
		}
	}
	if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Right))) {
		Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.125f * deltaTime);
		if (!MarioDirection) {
			player.property.move(Xvelo * deltaTime, 0.0f);
		}
		else {
			player.property.move((0 - Xvelo) * deltaTime, 0.0f);
		}
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
	bool isCollideSideBool;
	bool isCollideLeftBool, isCollideRightBool;
	// Check if Mario collide with left or right
	bool isLeft = false, isRight = false;
	for (const auto& i : ObstaclesList) {
		hitbox_loop = i.getGlobalHitbox();
		if (player.isCollideRight2(hitbox_loop)) isRight = true;
		if (player.isCollideLeft2(hitbox_loop)) isLeft = true;
		if (isRight && isLeft) break;
	}
	bool isTrueCollide = false;
	if (isRight || isLeft) {
		// Stop on wall
		if (isRight && sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || (isLeft && sf::Keyboard::isKeyPressed(sf::Keyboard::Left))) {
			Xvelo = 0.0f;
		}
		else if ((isRight && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || !MarioDirection)) || (isLeft && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || MarioDirection))) {
			Xvelo = 0.0f;
		}
		std::set<std::pair<int, int>> AllCollideList;
		// Count if size AllCollideList equal to CollideAddCounter
		int CollideAddCounter = 0;
		float CurrPosXCollide = 0, CurrPosYCollide = 0;
		bool isCollideSide = false;
		// 0 for right; 1 for left
		bool NoAdd = false;
		while (true) {
			isCollideLeftBool = false;
			isCollideRightBool = false;
			isCollideSideBool = false;
			// Loop through obstacles
			for (const auto& i : ObstaclesList) {
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
			// Break if size AllCollideList is not equal to CollideAddCounter
			if (AllCollideList.size() != CollideAddCounter) break;
			// Adjust Position if collide
			if (isCollideLeftBool && !isCollideRightBool) {
				player.property.move(1.0f * deltaTime, 0.0f);
				isCollideSideBool = true;
			}
			else if (isCollideRightBool && !isCollideLeftBool) {
				player.property.move(-1.0f * deltaTime, 0.0f);
				isCollideSideBool = false;
			}
			// Break if no collide
			else break;
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
	sf::FloatRect hitbox_loop;
	int UpTimeCounter = 0;
	// bottom update
	bool isCollideBotBool = false;
	bool isCollideLeft = false, isCollideRight = false;
	for (const auto& i : ObstaclesList) {
		hitbox_loop = i.getGlobalHitbox();
		if (player.isCollideBot(hitbox_loop) && !isCollideBotBool) isCollideBotBool = true;
		if (player.isCollideRight(hitbox_loop) && !isCollideRight) isCollideRight = true;
		if (player.isCollideLeft(hitbox_loop) && !isCollideLeft) isCollideLeft = true;
		if (isCollideBotBool && isCollideRight && isCollideLeft) break;
	}
	if (Yvelo >= 0.0f && MarioCanJump) MarioCanJump = false;
	if (!isCollideBotBool || MarioCanJump) {
		MarioCurrentFalling = true;
		UpTimeCounter = 0;
		Yvelo += (Yvelo >= 10.0f ? 0.0f : 1.0f * deltaTime);
		player.property.move(0.0f, Yvelo * deltaTime);
		if (MarioLockedView) view.move(0.0f, Yvelo * deltaTime);
		isCollideBotBool = false;
	}
	for (const auto& i : ObstaclesList) {
		hitbox_loop = i.getGlobalHitbox();
		if (player.isCollideBot(hitbox_loop) && !isCollideBotBool) isCollideBotBool = true;
		if (player.isCollideRight(hitbox_loop) && !isCollideRight) isCollideRight = true;
		if (player.isCollideLeft(hitbox_loop) && !isCollideLeft) isCollideLeft = true;
		if (isCollideBotBool && isCollideRight && isCollideLeft) break;
	}
	//recolide
	if (isCollideBotBool) {
		MarioCurrentFalling = false;
		bool isLanding;
		UpTimeCounter = 0;
		if (Yvelo > 0.0f) {
			Yvelo = 0.0f;
			isLanding = true;
		}
		else isLanding = false;
		while (true) {
			isCollideBotBool = false;
			isCollideLeft = false;
			isCollideRight = false;
			for (const auto& i : ObstaclesList) {
				hitbox_loop = i.getGlobalHitbox();
				if (player.isCollideBot(hitbox_loop)) {
					isCollideBotBool = true;
				}
				if (player.isCollideRight(hitbox_loop)) isCollideRight = true;
				if (player.isCollideLeft(hitbox_loop)) isCollideLeft = true;
				if (isCollideBotBool && isCollideRight && isCollideLeft) break;
			}
			if ((isCollideBotBool && (!isCollideRight && !isCollideLeft) && isLanding) || (isCollideBotBool && (isCollideRight || isCollideLeft) && isLanding)) {
				++UpTimeCounter;
				player.property.move(0.0f, -1.0f * deltaTime);
			}
			else if (!isCollideBotBool && (!isCollideRight || !isCollideLeft)) {
				if (UpTimeCounter != 0) {
					player.property.move(0.0f, 1.0f * deltaTime);
				}
				break;
			}
			else break;
		}
	}
	// top update
	bool isCollideTopBool = false;
	for (const auto& i : ObstaclesList) {
		hitbox_loop = i.getGlobalHitbox();
		if (player.isCollideTop(hitbox_loop)) {
			isCollideTopBool = true;
			break;
		}
	}
	if (isCollideTopBool && Yvelo < 0.0f) {
		Yvelo = 0.0f;
		UpTimeCounter = 0;
		while (true) {
			isCollideTopBool = false;
			isCollideLeft = false;
			isCollideRight = false;
			for (const auto& i : ObstaclesList) {
				hitbox_loop = i.getGlobalHitbox();
				if (player.isCollideTop(hitbox_loop)) isCollideTopBool = true;
				if (player.isCollideRight(hitbox_loop)) isCollideRight = true;
				if (player.isCollideLeft(hitbox_loop)) isCollideLeft = true;
				if (isCollideTopBool && isCollideRight && isCollideLeft) break;
			}
			if ((isCollideTopBool && !isCollideRight && !isCollideLeft) || (isCollideTopBool && (isCollideRight || isCollideLeft))) {
				++UpTimeCounter;
				player.property.move(0.0f, 1.0f * deltaTime);
				if (MarioLockedView) view.move(0.0f, 1.0f * deltaTime);
			}
			else if (!isCollideTopBool && (!isCollideRight || !isCollideLeft)) {
				if (UpTimeCounter != 0) {
					player.property.move(0.0f, -1.0f * deltaTime);
					if (MarioLockedView) view.move(0.0f, -1.0f * deltaTime);
				}
				break;
			}
			else break;
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
	if (!MarioDirection) {
		if (Yvelo == 0.0f && !MarioCurrentFalling) {
			if (Xvelo == 0.0f) MarioAnimation.update("IdleLeft", player.property);
			else {
				MarioAnimation.setAnimationFrequency("RunLeft", std::max(12.0f, std::min(Xvelo * 8.0f, 60.0f)));
				MarioAnimation.update("RunLeft", player.property);
			}
		}
		else MarioAnimation.update("JumpLeft", player.property);
	}
	else {
		if (Yvelo == 0.0f && !MarioCurrentFalling) {
			if (Xvelo == 0.0f) MarioAnimation.update("IdleRight", player.property);
			else {
				MarioAnimation.setAnimationFrequency("RunRight", std::max(12.0f, std::min(Xvelo * 8.0f, 60.0f)));
				MarioAnimation.update("RunRight", player.property);
			}
		}
		else MarioAnimation.update("JumpRight", player.property);
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