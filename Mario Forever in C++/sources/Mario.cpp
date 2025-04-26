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
#include "../headers/Effect/MarioEffect.hpp"
#include "../headers/Object/ExitGate.hpp"
#include "../headers/Core/Music.hpp"

#include "../resource.h"

//define here
AnimationManager MarioAnimation;
MovableObject player;
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
int PowerState = 1;
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

bool CanControlMario = true;
// 0 for right; 1 for left
//texture loading
sf::Texture SmallMario;
sf::Texture BigMario;
std::array<float, 2> PowerOffset = { 30.0f, 7.0f };
int loadMarioRes() {
	AppearingTimer.restart();
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
	if (CanControlMario && !LevelCompleteEffect) {
		sf::FloatRect hitbox_loop;
		bool isCollideSideBool = false;
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) && !MarioCrouchDown && window.hasFocus()) {
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !MarioCurrentFalling && PowerState > 0)) {
				if (Xvelo == 0) MarioDirection = true;
				else if (!MarioDirection) {
					Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.375f * deltaTime);
					player.property.move({ Xvelo * deltaTime, 0.0f });
				}
				//init speed
				if (Xvelo < 1.0f && MarioDirection && !MarioCrouchDown) Xvelo = 1.0f;
				if (!isCollideLeft2(player, ObstaclesList, {}) && !isCollideLeft2(player, Bricks, BrickSaveList) && !isCollideLeft2(player, LuckyBlock, LuckyBlockSaveList)) {
					if (MarioDirection) {
						Xvelo += (Xvelo > player_speed ? 0.0f : 0.125f * deltaTime);
						player.property.move({ (0 - Xvelo) * deltaTime, 0.0f });
					}
				}
			}
		}
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) && !MarioCrouchDown && window.hasFocus()) {
			if (!(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !MarioCurrentFalling && PowerState > 0)) {
				if (Xvelo == 0) MarioDirection = false;
				else if (MarioDirection) {
					Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.375f * deltaTime);
					player.property.move({ (0 - Xvelo) * deltaTime, 0.0f });
				}
				if (Xvelo < 1.0f && !MarioDirection && !MarioCrouchDown) Xvelo = 1.0f;
				if (!isCollideRight2(player, ObstaclesList, {}) && !isCollideRight2(player, Bricks, BrickSaveList) && !isCollideRight2(player, LuckyBlock, LuckyBlockSaveList)) {
					//init speed
					if (!MarioDirection) {
						Xvelo += (Xvelo > player_speed ? 0.0f : 0.125f * deltaTime);
						player.property.move({ Xvelo * deltaTime, 0.0f });
					}
				}
			}
		}
		else if (((!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right))) && !MarioCrouchDown) || MarioCrouchDown) {
			if (!MarioCrouchDown) Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.125f * deltaTime);
			else Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.28125f * deltaTime);
			//if (!MarioCrouchDown) Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.001f * deltaTime);
			//else Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.0001f * deltaTime);
			if (!MarioDirection) player.property.move({ Xvelo * deltaTime, 0.0f });
			else player.property.move({ (0 - Xvelo) * deltaTime, 0.0f });
		}
		if (Xvelo < 0.0f) Xvelo = 0.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) && !MarioCurrentFalling && window.hasFocus()) {
			if (!PreJump && !Holding) {
				Sounds.PlaySound("Jump");
				Yvelo = -13.5f;
			}
			else if (PreJump) {
				Sounds.PlaySound("Jump");
				Yvelo = -13.5f;
				PreJump = false;
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
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) && window.hasFocus()) Holding = true;
		else Holding = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) && window.hasFocus()) player_speed = 7.5f;
		else player_speed = 4.375f;

		if (!MarioCurrentFalling && PreJump) PreJump = false;
	}
	else if (LevelCompleteEffect) {
		if (MarioDirection) MarioDirection = false;
		if (!isCollideRight2(player, ObstaclesList, {}) && !isCollideRight2(player, Bricks, BrickSaveList) && !isCollideRight2(player, LuckyBlock, LuckyBlockSaveList) && !EffectActive) {
			Xvelo = 2.5f;
			player.property.move({ Xvelo * deltaTime, 0.0f });
		}
		else Xvelo = 0.0f;
		//if (!MarioDirection) player.property.move({ Xvelo * deltaTime, 0.0f });
		//else player.property.move({ (0 - Xvelo) * deltaTime, 0.0f });
	}
}
void MarioVertXUpdate() {
	if (CanControlMario) {
		int be, nd;
		sf::FloatRect hitbox_loop;
		std::pair<bool, bool> ObstacleCheck, BrickCheck, LuckyCheck;
		bool isCollideLeftBool, isCollideRightBool;
		// Check if Mario collide with left or right
		if (!MarioDirection) {
			be = find_min_inx(player, ObstaclesList);
			nd = find_max_inx_dist(player, ObstaclesList, 64.0f + (Xvelo) * 16.0f);
			ObstacleCheck = isOrCollideSidet(player, ObstaclesList, be, nd, {});
			be = find_min_inx(player, Bricks);
			nd = find_max_inx_dist(player, Bricks, 64.0f + (Xvelo) * 16.0f);
			BrickCheck = isOrCollideSidet(player, Bricks, be, nd, BrickSaveList);
			be = find_min_inx(player, LuckyBlock);
			nd = find_max_inx_dist(player, LuckyBlock, 64.0f + (Xvelo) * 16.0f);
			LuckyCheck = isOrCollideSidet(player, LuckyBlock, be, nd, LuckyBlockSaveList);
		}
		else {
			be = find_max_inx(player, ObstaclesList);
			nd = find_min_inx_dist(player, ObstaclesList, 64.0f + (Xvelo) * 16.0f);
			ObstacleCheck = isOrCollideSidet(player, ObstaclesList, nd, be, {});
			be = find_max_inx(player, Bricks);
			nd = find_min_inx_dist(player, Bricks, 64.0f + (Xvelo) * 16.0f);
			BrickCheck = isOrCollideSidet(player, Bricks, nd, be, BrickSaveList);
			be = find_max_inx(player, LuckyBlock);
			nd = find_min_inx_dist(player, LuckyBlock, 64.0f + (Xvelo) * 16.0f);
			LuckyCheck = isOrCollideSidet(player, LuckyBlock, nd, be, LuckyBlockSaveList);
		}
		bool isTrueCollide = false;
		if (ObstacleCheck.first || ObstacleCheck.second || BrickCheck.first || BrickCheck.second || LuckyCheck.first || LuckyCheck.second) {
			// Stop on wall
			if (ObstacleCheck.first && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || (ObstacleCheck.second && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))) {
				Xvelo = 0.0f;
			}
			else if ((ObstacleCheck.first && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || !MarioDirection)) || (ObstacleCheck.second && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || MarioDirection))) {
				Xvelo = 0.0f;
			}
			if (BrickCheck.first && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || (BrickCheck.second && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))) {
				Xvelo = 0.0f;
			}
			else if ((BrickCheck.first && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || !MarioDirection)) || (BrickCheck.second && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || MarioDirection))) {
				Xvelo = 0.0f;
			}
			if (LuckyCheck.first && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || (LuckyCheck.second && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left))) {
				Xvelo = 0.0f;
			}
			else if ((LuckyCheck.first && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) || !MarioDirection)) || (LuckyCheck.second && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) || MarioDirection))) {
				Xvelo = 0.0f;
			}
			// Count if size AllCollideList equal to CollideAddCounter
			float CurrPosXCollide = 0, CurrPosYCollide = 0;
			bool isCollideSide = false;
			// 0 for right; 1 for left
			bool NoAdd = false;
			int NoAddCase = 0;
			std::pair<bool, bool> ObstacleCollide, BrickCollide, LuckyCollide;
			isCollideLeftBool = false;
			isCollideRightBool = false;
			// snap back w/ detailed check
			// Loop through obstacles
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

			//ObstacleCollide = isAccurateCollideSidet(player, ObstaclesList, CurrPosXCollide, CurrPosYCollide, NoAdd, {});
			//BrickCollide = isAccurateCollideSide(player, Bricks, CurrPosXCollide, CurrPosYCollide, NoAdd, BrickSaveList);
			//LuckyCollide = isAccurateCollideSide(player, LuckyBlock, CurrPosXCollide, CurrPosYCollide, NoAdd, LuckyBlockSaveList);

			// Break if size AllCollideList is not equal to CollideAddCounter
			//if (AllCollideList.size() != CollideAddCounter) break;
			// Adjust Position if collide
			if (ObstacleCollide.first) player.property.setPosition({ CurrPosXCollide + 32.0f - 4.0f + player.property.getOrigin().x, player.property.getPosition().y });
			if (ObstacleCollide.second) player.property.setPosition({ CurrPosXCollide - (1.0f + 4.0f + (23 - player.property.getOrigin().x)), player.property.getPosition().y });
			if (BrickCollide.first) player.property.setPosition({ CurrPosXCollide + 32.0f - 4.0f + player.property.getOrigin().x, player.property.getPosition().y });
			if (BrickCollide.second) player.property.setPosition({ CurrPosXCollide - (1.0f + 4.0f + (23 - player.property.getOrigin().x)), player.property.getPosition().y });
			if (LuckyCollide.first) player.property.setPosition({ CurrPosXCollide + 32.0f - 4.0f + player.property.getOrigin().x, player.property.getPosition().y });
			if (LuckyCollide.second) player.property.setPosition({ CurrPosXCollide - (1.0f + 4.0f + (23 - player.property.getOrigin().x)), player.property.getPosition().y });

			//if (!ObstacleCollide.first && !ObstacleCollide.second && !BrickCollide.first && !BrickCollide.second && !LuckyCollide.first && !LuckyCollide.second) break;
			// Break if no collide
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
void MarioVertYUpdate() {
	if (CanControlMario) {
		//if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) player.property.move(0.0f, 1.0f * deltaTime);
		//else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) player.property.move(0.0f, -1.0f * deltaTime);
		std::pair<bool, bool> ObstacleCollide, BrickCollide, LuckyCollide, SlopeCollide;
		bool SlopeCheck;
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

		/*SlopeCheck = isCollideBotSlope(player, Yvelo);
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
			SlopeCheck = isCollideBotSlope(player, Yvelo);
			if (SlopeCheck) {
				SlopeTemp.clear();
				OldY = player.property.getPosition().y;
				SlopeCollide = isAccuratelyCollideBotSlope(player, CurrPosXCollide, CurrPosYCollide, NoAdd, ID, SlopeTemp);
				for (const auto& i : SlopeTemp) {
					if (true) {
						if (i[2] == 0) player.property.setPosition(player.property.getPosition().x, std::max(i[1] + i[0] + 32.0f - player.property.getPosition().x - 3.0f - (52.0f - player.property.getOrigin().y + 20.0f), i[1] - 3.0f - (52.0f - player.property.getOrigin().y + 4.0f)));
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
		*/

		// bottom update (obstacles)

		//ObstacleCheck = isCollideBot(player, ObstaclesList);
		//BrickCheck = isCollideBot(player, Bricks);
		//LuckyCheck = isCollideBot(player, LuckyBlock);

		//if ((!ObstacleCheck && !BrickCheck && !LuckyCheck && !SlopeCheck) || MarioCanJump) {
		//if ((!ObstacleCheck && !BrickCheck && !LuckyCheck) || MarioCanJump) {
		MarioCurrentFalling = true;
		Yvelo += (Yvelo >= 10.0f ? 0.0f : 0.5f * deltaTime);
		player.property.move({ 0.0f, Yvelo * deltaTime });
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
		//LuckyCollide = isAccurateCollideBot(player, LuckyBlock, CurrPosYCollide, NoAdd);
		//recolide
		if (ObstacleCollide.first || BrickCollide.first || LuckyCollide.first) {
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
				if (ObstacleCollide.second || BrickCollide.second || LuckyCollide.second) {
					player.property.setPosition({ player.property.getPosition().x, CurrPosYCollide - (52.0f - player.property.getOrigin().y + 7.0f) });
				}
				if (!ObstacleCollide.second && !BrickCollide.second && !LuckyCollide.second) {
					player.property.setPosition({ player.property.getPosition().x, CurrPosYCollide - (52.0f - player.property.getOrigin().y + 7.0f) });
				}
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
		//ObstacleCollide = isAccurateCollideTop(player, ObstaclesList, CurrPosYCollide, NoAdd, {});
		//BrickCollide = isAccurateCollideTop(player, Bricks, CurrPosYCollide, NoAdd, BrickSaveList);
		//LuckyCollide = isAccurateCollideTop(player, LuckyBlock, CurrPosYCollide, NoAdd, LuckyBlockSaveList);
		std::vector<std::pair<float, float>> BrickPos, LuckyPos;
		if ((ObstacleCollide.first || BrickCollide.first || LuckyCollide.first) && Yvelo < 0.0f) {
			Yvelo = 0.0f;
			NoAdd = false;
			//snap back
			if (ObstacleCollide.second || BrickCollide.second || LuckyCollide.second) {
				if (PowerState > 0 && !MarioCrouchDown)
					player.property.setPosition({ player.property.getPosition().x, CurrPosYCollide + (31.0f + player.property.getOrigin().y - PowerOffset[PowerState]) });
				else if ((PowerState > 0 && MarioCrouchDown) || (PowerState == 0 && MarioAppearing) || (PowerState == 0 && !MarioCrouchDown))
					player.property.setPosition({ player.property.getPosition().x, CurrPosYCollide + (31.0f + player.property.getOrigin().y - 30.0f) });
			}
			if (!ObstacleCollide.second && !BrickCollide.second && !LuckyCollide.second) {
				if (PowerState > 0 && !MarioCrouchDown)
					player.property.setPosition({ player.property.getPosition().x, CurrPosYCollide + (31.0f + player.property.getOrigin().y - PowerOffset[PowerState]) });
				else if ((PowerState > 0 && MarioCrouchDown) || (PowerState == 0 && MarioAppearing) || (PowerState == 0 && !MarioCrouchDown))
					player.property.setPosition({ player.property.getPosition().x, CurrPosYCollide + (31.0f + player.property.getOrigin().y - 30.0f) });
			}
			// Start event Brick
			if (BrickCollide.first) {
				BrickPos = isCollideTopDetailed(player, Bricks, BrickSaveList);
				if (BrickPos.size() > 0) {
					for (const auto& i : BrickPos) {
						HitEvent(i.first, i.second);
					}
				}
			}
			if (LuckyCollide.first) {
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

	player.property.setOrigin({ 11, 51 });
	if (PowerState > 0 && !MarioCrouchDown) {
		setHitbox(player.hitboxMain, sf::FloatRect({ 0.0f + 4.0f, 0.0f + PowerOffset[PowerState] }, { 23.0f, 52.0f }));
		setHitbox(player.hitboxTop, sf::FloatRect({ 1.0f + 4.0f, 0.0f + PowerOffset[PowerState] }, { 21.0f, 2.0f }));
		setHitbox(player.hitboxBot, sf::FloatRect({ 4.0f + 4.0f, 50.0f + PowerOffset[PowerState] }, { 18.0f, 2.0f }));
		setHitbox(player.hitboxBot2, sf::FloatRect({ 4.0f + 4.0f, 50.0f + PowerOffset[PowerState] }, { 18.0f, 3.0f }));
		setHitbox(player.hitboxRight2, sf::FloatRect({ 21.0f + 4.0f, 2.0f + PowerOffset[PowerState] }, { 4.0f, 43.0f }));
		setHitbox(player.hitboxLeft2, sf::FloatRect({ -2.0f + 4.0f, 2.0f + PowerOffset[PowerState] }, { 4.0f, 43.0f }));
		setHitbox(player.hitboxRight, sf::FloatRect({ 21.0f + 4.0f, 2.0f + PowerOffset[PowerState] }, { 2.0f, 43.0f }));
		setHitbox(player.hitboxLeft, sf::FloatRect({ 0.0f + 4.0f, 2.0f + PowerOffset[PowerState] }, { 2.0f, 43.0f }));
		setHitbox(player.hitboxSlopeBot, sf::FloatRect({ 1.0f + 4.0f, 50.0f + PowerOffset[PowerState] }, { 21.0f, 10.0f }));
	}
	else if ((PowerState > 0 && MarioCrouchDown) || (PowerState == 0 && MarioAppearing) || (PowerState == 0 && !MarioCrouchDown)) {
		setHitbox(player.hitboxMain, sf::FloatRect({ 0.0f + 4.0f, 0.0f + 30.0f }, { 23.0f, 29.0f })); // 30
		setHitbox(player.hitboxTop, sf::FloatRect({ 1.0f + 4.0f, 0.0f + 30.0f }, { 21.0f, 2.0f }));
		setHitbox(player.hitboxBot, sf::FloatRect({ 4.0f + 4.0f, 27.0f + 30.0f }, { 18.0f, 2.0f }));
		setHitbox(player.hitboxBot2, sf::FloatRect({ 4.0f + 4.0f, 27.0f + 30.0f }, { 18.0f, 3.0f }));
		setHitbox(player.hitboxRight2, sf::FloatRect({ 21.0f + 4.0f, 2.0f + 30.0f }, { 4.0f, 20.0f }));
		setHitbox(player.hitboxLeft2, sf::FloatRect({ -2.0f + 4.0f, 2.0f + 30.0f }, { 4.0f, 20.0f }));
		setHitbox(player.hitboxRight, sf::FloatRect({ 21.0f + 4.0f, 2.0f + 30.0f }, { 2.0f, 20.0f }));
		setHitbox(player.hitboxLeft, sf::FloatRect({ 0.0f + 4.0f, 2.0f + 30.0f }, { 2.0f, 20.0f }));
		setHitbox(player.hitboxSlopeBot, sf::FloatRect({ 1.0f + 4.0f, 27.0f + 30.0f }, { 21.0f, 10.0f }));
	}
	//animation update

	//mariostate:
	// 0: idle, 1: run, 2: jump = fall, 3: crouch
	if (CanControlMario) {
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
		if (CanControlMario) window.draw(player.property);
	}
	else {
		if (!InvincibleState) {
			if (CanControlMario) window.draw(player.property);
			InvincibleState = true;
		}
		else InvincibleState = false;
	}
}