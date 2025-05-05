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
#include "../headers/Core/Animate/LocalAnimationManager.hpp"
#include "../headers/Core/TextureManager.hpp"
#include "../headers/Block/LuckyBlock.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Core/Sound.hpp"
#include "../headers/Core/Level.hpp"
#include "../headers/Block/Slopes.hpp"
#include "../headers/Effect/MarioEffect.hpp"
#include "../headers/Object/ExitGate.hpp"
#include "../headers/Core/Music.hpp"
#include "../headers/Core/Interpolation.hpp"

#include "../resource.h"

//define here
LocalAnimationManager MarioAnimation;
MovableObject player;
TextureManager MarioTexture;
float Xvelo = 0.0f;
float Yvelo = 0.0f;
bool FirstMarioDirection = false;
bool MarioDirection = FirstMarioDirection;
bool MarioCurrentFalling = true;
bool PreJump = false;
bool Holding;
bool MarioCrouchDown = false;
float player_speed;
int MarioState = 0;
int lastMarioState = -1;
int PowerState = 0;
int lastPowerState = 0;

sf::Vector2f prevMarioPos(0, 0);

int Lives = 4;

bool OverSpeed = false;
bool isMarioOverlapping = false;
long long int Score = 0;
sf::Clock AppearingTimer;
sf::Clock InvincibleTimer;
bool Invincible = false;
bool InvincibleState = false;
bool MarioAppearing = false;

bool CanControlMario = true;
// 0 for right; 1 for left
//texture loading
sf::Texture SmallMario;
sf::Texture BigMario;
std::array<float, 2> PowerOffset = { 30.0f, 7.0f };
void loadMarioRes() {
	AppearingTimer.restart();
	// Resources Loader;

	MarioTexture.Loadingtexture(SMALLMARIO_TEXTURE, "SmallMario", 0, 0, 248, 118);
	MarioTexture.Loadingtexture(BIGMARIO_TEXTURE, "BigMario", 0, 0, 248, 118);

	MarioAnimation.setAnimation(0, 0, 31, 59, 0, 0);
	player.property.setOrigin({ 11, 51 });
}
//sprite function
void SetPrevMarioPos() {
	prevMarioPos = player.property.getPosition();
}
void InterpolateMarioPos() {
	player.property.setPosition(linearInterpolation(prevMarioPos, player.property.getPosition(), timestep.getInterpolationAlphaAsFloat()));
	std::cout << timestep.getInterpolationAlphaAsFloat() << std::endl;
}
void KeyboardMovement(float deltaTime) {
	if (CanControlMario && !LevelCompleteEffect) {
		sf::FloatRect hitbox_loop;
		bool isCollideSideBool = false;
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) && !MarioCrouchDown && window.hasFocus()) {
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !MarioCurrentFalling && PowerState > 0)) {
				if (Xvelo == 0) MarioDirection = true;
				else if (!MarioDirection) {
					Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.375f * deltaTime);
				}
				//init speed
				if (Xvelo < 1.0f && MarioDirection && !MarioCrouchDown) Xvelo = 1.0f;
				if (MarioDirection) Xvelo += (Xvelo > player_speed ? 0.0f : 0.125f * deltaTime);
			}
		}
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) && !MarioCrouchDown && window.hasFocus()) {
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !MarioCurrentFalling && PowerState > 0)) {
				if (Xvelo == 0) MarioDirection = false;
				else if (MarioDirection) {
					Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.375f * deltaTime);
				}
				if (Xvelo < 1.0f && !MarioDirection && !MarioCrouchDown) Xvelo = 1.0f;
				if (!MarioDirection) Xvelo += (Xvelo > player_speed ? 0.0f : 0.125f * deltaTime);
			}
		}
		else if (((!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))) && !MarioCrouchDown) || MarioCrouchDown) {
			if (!MarioCrouchDown) Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.125f * deltaTime);
			else Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.28125f * deltaTime);
		}
		if (Xvelo < 0.0f) Xvelo = 0.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) && !MarioCurrentFalling && window.hasFocus()) {
			if (!PreJump && !Holding) {
				Sounds.PlaySound("Jump");
				Yvelo = -14.0f;
				Holding = true;
			}
			else if (PreJump) {
				Sounds.PlaySound("Jump");
				Yvelo = -14.0f;
				PreJump = false;
				Holding = true;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && PowerState > 0 && !MarioCurrentFalling && window.hasFocus()) {
			MarioCrouchDown = true;
		}
		else MarioCrouchDown = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) && window.hasFocus()) {
			if (Xvelo < 5.0f && Yvelo < 0.0f) Yvelo -= 0.4f * deltaTime;
			if (Xvelo >= 5.0f && Yvelo < 0.0f) Yvelo -= 0.5f * deltaTime;
			if (Yvelo >= 0.0f && !Holding) PreJump = true;
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) && window.hasFocus()) Holding = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) && window.hasFocus()) player_speed = 7.5f;
		else player_speed = 4.375f;

		if (!MarioCurrentFalling && PreJump) PreJump = false;
	}
	else if (LevelCompleteEffect) {
		if (MarioDirection) MarioDirection = false;
		Xvelo = 2.5f;
	}
}
void MarioVertXUpdate(float deltaTime) {
	if (CanControlMario) {
		int be, nd;
		sf::FloatRect hitbox_loop;
		std::pair<bool, bool> ObstacleCollide, BrickCollide, LuckyCollide;
		bool isCollideLeftBool, isCollideRightBool;
		float CurrPosXCollide = 0, CurrPosYCollide = 0;
		bool NoAdd = false;

		if (!MarioDirection) player.property.move({ Xvelo * deltaTime, 0.0f });
		else player.property.move({ (0 - Xvelo) * deltaTime, 0.0f });
		if (!MarioDirection) {
			be = find_min_inx(player, ObstaclesList);
			nd = find_max_inx_dist(player, ObstaclesList, 64.0f + (Xvelo) * 16.0f);
			ObstacleCollide = isAccurateCollideSidet(player, ObstaclesList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f, {});
			be = find_min_inx(player, Bricks);
			nd = find_max_inx_dist(player, Bricks, 64.0f + (Xvelo) * 16.0f);
			BrickCollide = isAccurateCollideSidet(player, Bricks, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f, BrickSaveList);
			be = find_min_inx(player, LuckyBlock);
			nd = find_max_inx_dist(player, LuckyBlock, 64.0f + (Xvelo) * 16.0f);
			LuckyCollide = isAccurateCollideSidet(player, LuckyBlock, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f, LuckyBlockSaveList);
		}
		else {
			be = find_max_inx(player, ObstaclesList);
			nd = find_min_inx_dist(player, ObstaclesList, 64.0f + (Xvelo) * 16.0f);
			ObstacleCollide = isAccurateCollideSidet(player, ObstaclesList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f, {});
			be = find_max_inx(player, Bricks);
			nd = find_min_inx_dist(player, Bricks, 64.0f + (Xvelo) * 16.0f);
			BrickCollide = isAccurateCollideSidet(player, Bricks, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f, BrickSaveList);
			be = find_max_inx(player, LuckyBlock);
			nd = find_min_inx_dist(player, LuckyBlock, 64.0f + (Xvelo) * 16.0f);
			LuckyCollide = isAccurateCollideSidet(player, LuckyBlock, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f, LuckyBlockSaveList);
		}
		//snap back
		if (ObstacleCollide.first || BrickCollide.first || LuckyCollide.first) {
			Xvelo = 0.0f;
			player.property.setPosition({ CurrPosXCollide + 32.0f - 3.0f + player.property.getOrigin().x, player.property.getPosition().y });
		}
		if (ObstacleCollide.second || BrickCollide.second || LuckyCollide.second) {
			Xvelo = 0.0f;
			player.property.setPosition({ CurrPosXCollide - (1.0f + 5.0f + (23 - player.property.getOrigin().x)), player.property.getPosition().y });
		}

		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) && Xvelo > 4.475f) {
			OverSpeed = true;
		}
		if (Xvelo > player_speed && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) {
			if (OverSpeed) {
				Xvelo -= 0.125f * deltaTime;
				if (Xvelo <= player_speed) OverSpeed = false;
			}
			else Xvelo = player_speed;
		}
		if (Xvelo > 7.5f && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X)) Xvelo = 7.5f;
	}
}
void MarioVertYUpdate(float deltaTime) {
	if (CanControlMario) {
		bool ObstacleCollide, BrickCollide, LuckyCollide, SlopeCollide;
		bool SlopeCheck;
		float CurrPosYCollide, CurrPosXCollide, ID, Diff, OldY;
		bool NoAdd;

		MarioCurrentFalling = true;
		Yvelo += (Yvelo >= 10.0f ? 0.0f : 0.5f * deltaTime);
		player.property.setPosition({ player.property.getPosition().x, player.property.getPosition().y + Yvelo * deltaTime });
		Yvelo += (Yvelo >= 10.0f ? 0.0f : 0.5f * deltaTime);
		if (Yvelo > 10.0f) Yvelo = 10.0f;
		//}
		bool isLanding;
		NoAdd = false;
		int be, nd;
		be = find_min_iny(player, ObstaclesVertPosList);
		nd = find_max_iny_dist(player, ObstaclesVertPosList, 64.0f + (Yvelo) * 16.0f);
		ObstacleCollide = isAccurateCollideBott(player, ObstaclesVertPosList, CurrPosYCollide, NoAdd, be, nd, 80.0f);
		be = find_min_iny(player, BricksVertPosList);
		nd = find_max_iny_dist(player, BricksVertPosList, 64.0f + (Yvelo) * 16.0f);
		BrickCollide = isAccurateCollideBott(player, BricksVertPosList, CurrPosYCollide, NoAdd, be, nd, 80.0f);
		be = find_min_iny(player, LuckyVertPosList);
		nd = find_max_iny_dist(player, LuckyVertPosList, 64.0f + (Yvelo) * 16.0f);
		LuckyCollide = isAccurateCollideBott(player, LuckyVertPosList, CurrPosYCollide, NoAdd, be, nd, 80.0f);
		//recolide
		if (ObstacleCollide || BrickCollide || LuckyCollide) {
			MarioCurrentFalling = false;
			if (Yvelo >= 0.0f) {
				isLanding = true;
				Yvelo = 0.0f;
			}
			else {
				isLanding = false;
				Yvelo = 0.0f;
			}
			if (isLanding) {
				player.property.setPosition({ player.property.getPosition().x, CurrPosYCollide - (52.0f - player.property.getOrigin().y + 7.0f) });
			}
		}
		// top update
		NoAdd = false;
		be = find_max_iny(player, ObstaclesVertPosList);
		nd = find_min_iny_dist(player, ObstaclesVertPosList, 64.0f - (Yvelo) * 16.0f);
		ObstacleCollide = isAccurateCollideTopt(player, ObstaclesVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		be = find_max_iny(player, BricksVertPosList);
		nd = find_min_iny_dist(player, BricksVertPosList, 64.0f - (Yvelo) * 16.0f);
		BrickCollide = isAccurateCollideTopt(player, BricksVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		be = find_max_iny(player, LuckyVertPosList);
		nd = find_min_iny_dist(player, LuckyVertPosList, 64.0f - (Yvelo) * 16.0f);
		LuckyCollide = isAccurateCollideTopt(player, LuckyVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		std::vector<std::pair<float, float>> BrickPos, LuckyPos;
		if ((ObstacleCollide || BrickCollide || LuckyCollide) && Yvelo < 0.0f) {
			Yvelo = 0.0f;
			NoAdd = false;
			//snap back
			if (PowerState > 0 && !MarioCrouchDown)
				player.property.setPosition({ player.property.getPosition().x, CurrPosYCollide + (31.0f + player.property.getOrigin().y - PowerOffset[PowerState]) });
			else if ((PowerState > 0 && MarioCrouchDown) || (PowerState == 0 && MarioAppearing) || (PowerState == 0 && !MarioCrouchDown))
				player.property.setPosition({ player.property.getPosition().x, CurrPosYCollide + (31.0f + player.property.getOrigin().y - 30.0f) });
			// Start event Brick
			if (BrickCollide) {
				BrickPos = isCollideTopDetailed(player, Bricks, BrickSaveList);
				if (BrickPos.size() > 0) {
					for (const auto& i : BrickPos) {
						HitEvent(i.first, i.second);
					}
				}
			}
			if (LuckyCollide) {
				LuckyPos = isCollideTopDetailed(player, LuckyBlock, LuckyBlockSaveList);
				if (LuckyPos.size() > 0) {
					for (const auto& i : LuckyPos) {
						LuckyHitEvent(i.first, i.second);
					}
				}
			}
		}
	}
}
void UpdateAnimation() {
	if (PowerState > 0 && !MarioCrouchDown) {
		setHitbox(player.hitboxMain, sf::FloatRect({ 0.0f + 4.0f, 0.0f + PowerOffset[PowerState] }, { 23.0f, 52.0f }));
		setHitbox(player.hitboxTop, sf::FloatRect({ 1.0f + 4.0f, 0.0f + PowerOffset[PowerState] }, { 21.0f, 2.0f }));
		setHitbox(player.hitboxBot, sf::FloatRect({ 4.0f + 4.0f, 50.0f + PowerOffset[PowerState] }, { 18.0f, 2.0f }));
		setHitbox(player.hitboxBot2, sf::FloatRect({ 4.0f + 4.0f, 50.0f + PowerOffset[PowerState] }, { 18.0f, 3.0f }));
		setHitbox(player.hitboxRight, sf::FloatRect({ 21.0f + 4.0f, 2.0f + PowerOffset[PowerState] }, { 4.0f, 43.0f }));
		setHitbox(player.hitboxLeft, sf::FloatRect({ -2.0f + 4.0f, 2.0f + PowerOffset[PowerState] }, { 4.0f, 43.0f }));
		setHitbox(player.hitboxSlopeBot, sf::FloatRect({ 1.0f + 4.0f, 50.0f + PowerOffset[PowerState] }, { 21.0f, 10.0f }));
	}
	else if ((PowerState > 0 && MarioCrouchDown) || (PowerState == 0 && MarioAppearing) || (PowerState == 0 && !MarioCrouchDown)) {
		setHitbox(player.hitboxMain, sf::FloatRect({ 0.0f + 4.0f, 0.0f + 30.0f }, { 23.0f, 29.0f })); // 30
		setHitbox(player.hitboxTop, sf::FloatRect({ 1.0f + 4.0f, 0.0f + 30.0f }, { 21.0f, 2.0f }));
		setHitbox(player.hitboxBot, sf::FloatRect({ 4.0f + 4.0f, 27.0f + 30.0f }, { 18.0f, 2.0f }));
		setHitbox(player.hitboxBot2, sf::FloatRect({ 4.0f + 4.0f, 27.0f + 30.0f }, { 18.0f, 3.0f }));
		setHitbox(player.hitboxRight, sf::FloatRect({ 21.0f + 4.0f, 2.0f + 30.0f }, { 4.0f, 20.0f }));
		setHitbox(player.hitboxLeft, sf::FloatRect({ -2.0f + 4.0f, 2.0f + 30.0f }, { 4.0f, 20.0f }));
		setHitbox(player.hitboxSlopeBot, sf::FloatRect({ 1.0f + 4.0f, 27.0f + 30.0f }, { 21.0f, 10.0f }));
	}
	//animation update

	//mariostate:
	// 0: idle, 1: run, 2: jump = fall, 3: crouch, 4: appear
	int ypos = (!MarioDirection) ? 0 : 1;
	//std::cout << ypos << "\n";
	switch (PowerState) {
	case 0:
		MarioAnimation.setTexture(player.property, MarioTexture.GetTexture("SmallMario"));
		break;
	case 1:
		MarioAnimation.setTexture(player.property, MarioTexture.GetTexture("BigMario"));
		break;
	}
	if (CanControlMario) {
		if (!MarioAppearing) {
			if (MarioCurrentFalling) {
				MarioState = 2;
				if (lastMarioState != MarioState && MarioState != 4) {
					MarioAnimation.setAnimation(3, 3, 31, 59, ypos, 100);
					lastMarioState = MarioState;
				}
				MarioAnimation.setYPos(ypos);
				if (PowerState == 0) MarioAnimation.update(player.property);
				else if (PowerState == 1) MarioAnimation.update(player.property);
			}
			else if (Yvelo == 0.0f && !(!MarioCurrentFalling && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && PowerState > 0)) {
				if (Xvelo == 0.0f) {
					MarioState = 0;
					if (lastMarioState != MarioState && MarioState != 4) {
						MarioAnimation.setAnimation(2, 2, 31, 59, ypos, 0);
						lastMarioState = MarioState;
					}
					MarioAnimation.setYPos(ypos);
					if (PowerState == 0) MarioAnimation.update(player.property);
					else if (PowerState == 1) MarioAnimation.update(player.property);
				}
				else {
					MarioState = 1;
					if (lastMarioState != MarioState && MarioState != 4) {
						MarioAnimation.setAnimation(0, 2, 31, 59, ypos);
						lastMarioState = MarioState;
					}
					MarioAnimation.setYPos(ypos);
					MarioAnimation.setFrequency(f_max(12.0f, f_min(Xvelo * 6.0f, 45.0f)));
					if (PowerState == 0) {
						//MarioAnimation.setAnimationFrequency("RunSmallLeft", f_max(24.0f, f_min(Xvelo * 8.0f, 75.0f)));
						MarioAnimation.update(player.property);
					}
					else if (PowerState == 1) {
						//MarioAnimation.setAnimationFrequency("RunBigLeft", f_max(24.0f, f_min(Xvelo * 8.0f, 75.0f)));
						MarioAnimation.update(player.property);
					}
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && PowerState > 0) {
				MarioState = 3;
				if (lastMarioState != MarioState && MarioState != 4) {
					MarioAnimation.setAnimation(4, 4, 31, 59, ypos);
					lastMarioState = MarioState;
				}
				MarioAnimation.setYPos(ypos);
				MarioAnimation.update(player.property);
			}
		}
		else {
			MarioState = 4;
			if (lastMarioState != MarioState && MarioState == 4) {
				MarioAnimation.setAnimation(5, 7, 31, 59, ypos, 100);
				lastMarioState = MarioState;
			}
			MarioAnimation.setYPos(ypos);
			if (PowerState == 0) MarioAnimation.update(player.property);
			else if (PowerState == 1) MarioAnimation.update(player.property);
		}
	}
}
void PowerDown() {
	if (!Invincible) {
		if (PowerState == 1) {
			Sounds.PlaySound("Pipe");
			PowerState = 0;
			Invincible = true;
			InvincibleTimer.restart();
			InvincibleState = false;
		}
		else if (PowerState == 0 && CanControlMario && !LevelCompleteEffect) {
			CanControlMario = false;
			ActiveMarioEffect();
		}
	}
}
void Death() {
	Music.StopAllMODMusic();
	Music.StopAllOGGMusic();
	if (Lives <= 0) window.close();
	else --Lives;
	Objectbuilding();
	Xvelo = 0.0f;
	Yvelo = 0.0f;
	PowerState = 0;
	lastPowerState = 0;
	LevelCompleteEffect = false;
	MarioDirection = FirstMarioDirection;
	ExitGateForeActive = true;
	ExitGateEffectReset();
}
void CheckForDeath() {
	if (isOutScreenYBottom(player.property.getPosition().y, 80)) {
		CanControlMario = false;
		ActiveMarioEffect();
	}
}
inline void MarioDraw() {
	// check power state here
	if (AppearingTimer.getElapsedTime().asMilliseconds() > (3000.0f / 91.0f) * 15.0f) MarioAppearing = false;
	if (PowerState != lastPowerState) {
		MarioAppearing = true;
		AppearingTimer.restart();
		lastPowerState = PowerState;
	}
	//then draw
	if (InvincibleTimer.getElapsedTime().asSeconds() > 2.0f) Invincible = false;
	if (!Invincible) {
		if (CanControlMario) rTexture.draw(player.property);
	}
	else {
		if (!InvincibleState) {
			if (CanControlMario) rTexture.draw(player.property);
			InvincibleState = true;
		}
		else InvincibleState = false;
	}
}