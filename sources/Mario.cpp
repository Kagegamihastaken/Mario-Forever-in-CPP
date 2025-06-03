#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <utility>
#include <array>

#include "Object/Mario.hpp"
#include "Block/Obstacles.hpp"
#include "Block/Brick.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/Animate/LocalAnimationManager.hpp"
#include "Core/ImageManager.hpp"
#include "Block/LuckyBlock.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Sound.hpp"
#include "Core/Level.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/ExitGate.hpp"
#include "Core/Music.hpp"
#include "Core/Interpolation.hpp"

//define here
LocalAnimationManager MarioAnimation;
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
static int lastMarioState = -1;
int PowerState = 0;
static int lastPowerState = 0;

int Lives = 4;

static bool OverSpeed = false;
long long int Score = 0;
static sf::Clock AppearingTimer;
static sf::Clock InvincibleTimer;
bool Invincible = false;
bool InvincibleState = false;
bool MarioAppearing = false;

bool CanControlMario = true;
// 0 for right; 1 for left
//texture loading
sf::Texture SmallMario;
sf::Texture BigMario;
static constexpr std::array<float, 2> PowerOffset = { 30.0f, 7.0f };
static constexpr int MARIO_IMAGE_WIDTH = 248;
static constexpr int MARIO_WIDTH = 31;
static constexpr int MARIO_HEIGHT = 59;

static std::vector<std::string> SmallMarioLeft;
static std::vector<std::string> SmallMarioRight;
static std::vector<std::string> BigMarioLeft;
static std::vector<std::string> BigMarioRight;

void UpdateSequenceAnimation() {
	switch (PowerState) {
		case 0:
			MarioAnimation.SetSequence(SmallMarioLeft, SmallMarioRight);
			break;
		case 1:
			MarioAnimation.SetSequence(BigMarioLeft, BigMarioRight);
			break;
		default: ;
	}
}
void SetPowerState(const int ps) {
	PowerState = ps;
	UpdateSequenceAnimation();
}
void loadMarioRes() {
	AppearingTimer.restart();
	// Resources Loader;
	MarioAnimation.setAnimation(0, 0, 0);
	player.property.setOrigin({ 11, 51 });
	ImageManager::AddImage("SmallMarioImage", "data/resources/SmallMario.png");
	ImageManager::AddImage("BigMarioImage", "data/resources/BigMario.png");
	for (int i = 0; i < MARIO_IMAGE_WIDTH / MARIO_WIDTH; ++i) {
		//SmallMario
		ImageManager::AddTexture("SmallMarioImage", sf::IntRect({MARIO_WIDTH*i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}), "SmallMarioRight_" + std::to_string(i));
		SmallMarioRight.push_back("SmallMarioRight_" + std::to_string(i));
		ImageManager::AddTexture("SmallMarioImage", sf::IntRect({MARIO_WIDTH*i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}), "SmallMarioLeft_" + std::to_string(i), false, true);
		SmallMarioLeft.push_back("SmallMarioLeft_" + std::to_string(i));
		//BigMario
		ImageManager::AddTexture("BigMarioImage", sf::IntRect({MARIO_WIDTH*i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}), "BigMarioRight_" + std::to_string(i));
		BigMarioRight.push_back("BigMarioRight_" + std::to_string(i));
		ImageManager::AddTexture("BigMarioImage", sf::IntRect({MARIO_WIDTH*i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}), "BigMarioLeft_" + std::to_string(i), false, true);
		BigMarioLeft.push_back("BigMarioLeft_" + std::to_string(i));
	}
	UpdateSequenceAnimation();
}
//sprite function
void SetPrevMarioPos() {
	player.prev = player.curr;
}
void InterpolateMarioPos(const float alpha) {
	player.property.setPosition(linearInterpolation(player.prev, player.curr, alpha));
}
void KeyboardMovement(const float deltaTime) {
	if (CanControlMario && !LevelCompleteEffect) {
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
		else if ((((!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)))) && !MarioCrouchDown) || MarioCrouchDown) {
			if (!MarioCrouchDown) Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.125f * deltaTime);
			else Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.28125f * deltaTime);
		}
		if (Xvelo < 0.0f) Xvelo = 0.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) && !MarioCurrentFalling && window.hasFocus()) {
			if (!PreJump && !Holding) {
				Sounds.PlaySound("Jump");
				Yvelo = -13.0f;
				Holding = true;
			}
			else if (PreJump) {
				Sounds.PlaySound("Jump");
				Yvelo = -13.0f;
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
	else if (!CanControlMario) {
		Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.625f * deltaTime);
		if (Xvelo < 0.0f) Xvelo = 0.0f;
	}
}
void MarioPosXUpdate(const float deltaTime) {
	if (!MarioDirection) player.curr = { player.curr.x + Xvelo * deltaTime, player.curr.y };
	else player.curr = { player.curr.x + (0 - Xvelo) * deltaTime, player.curr.y };
	if (CanControlMario) {

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
void MarioVertXUpdate() {
	if (CanControlMario) {
		int be, nd;
		std::pair<bool, bool> ObstacleCollide, BrickCollide, LuckyCollide;
		float CurrPosXCollide = 0, CurrPosYCollide = 0;
		bool NoAdd = false;

		if (!MarioDirection) {
			be = find_min_inx(player, ObstaclesHorzPosList);
			nd = find_max_inx_dist(player, ObstaclesHorzPosList, 64.0f + (Xvelo) * 4.0f);
			ObstacleCollide = isAccurateCollideSidet(player, player.curr, ObstaclesHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
			be = find_min_inx(player, BricksHorzPosList);
			nd = find_max_inx_dist(player, BricksHorzPosList, 64.0f + (Xvelo) * 4.0f);
			BrickCollide = isAccurateCollideSidet(player, player.curr, BricksHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
			be = find_min_inx(player, LuckyHorzPosList);
			nd = find_max_inx_dist(player, LuckyHorzPosList, 64.0f + (Xvelo) * 4.0f);
			LuckyCollide = isAccurateCollideSidet(player, player.curr, LuckyHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
		}
		else {
			be = find_max_inx(player, ObstaclesHorzPosList);
			nd = find_min_inx_dist(player, ObstaclesHorzPosList, 64.0f + (Xvelo) * 4.0f);
			ObstacleCollide = isAccurateCollideSidet(player, player.curr, ObstaclesHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
			be = find_max_inx(player, BricksHorzPosList);
			nd = find_min_inx_dist(player, BricksHorzPosList, 64.0f + (Xvelo) * 4.0f);
			BrickCollide = isAccurateCollideSidet(player, player.curr, BricksHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
			be = find_max_inx(player, LuckyHorzPosList);
			nd = find_min_inx_dist(player, LuckyHorzPosList, 64.0f + (Xvelo) * 4.0f);
			LuckyCollide = isAccurateCollideSidet(player, player.curr, LuckyHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		}
		//snap back
		if (ObstacleCollide.first || BrickCollide.first || LuckyCollide.first) {
			Xvelo = 0.0f;
			player.curr = { CurrPosXCollide + 32.0f - 3.0f + player.property.getOrigin().x, player.curr.y };
		}
		if (ObstacleCollide.second || BrickCollide.second || LuckyCollide.second) {
			Xvelo = 0.0f;
			player.curr = { CurrPosXCollide - (1.0f + 5.0f + (23 - player.property.getOrigin().x)), player.curr.y };
		}
	}
}
void MarioPosYUpdate(const float deltaTime) {
	if (CanControlMario) {
		MarioCurrentFalling = true;
		player.curr = { player.curr.x, player.curr.y + Yvelo * deltaTime };
		Yvelo += (Yvelo >= 10.0f ? 0.0f : 1.0f * deltaTime);
		if (Yvelo > 10.0f) Yvelo = 10.0f;
	}
}
void MarioVertYUpdate() {
	if (CanControlMario) {
		float CurrPosYCollide;
		//}
		player.curr = { player.curr.x, player.curr.y + 1.0f };
		bool NoAdd = false;
		int be = find_min_iny(player, ObstaclesVertPosList);
		int nd = find_max_iny_dist(player, ObstaclesVertPosList, 64.0f + (Yvelo) * 16.0f);
		bool ObstacleCollide = isAccurateCollideBott(player, player.curr, ObstaclesVertPosList, CurrPosYCollide, NoAdd, be,
		                                             nd, 80.0f);
		be = find_min_iny(player, BricksVertPosList);
		nd = find_max_iny_dist(player, BricksVertPosList, 64.0f + (Yvelo) * 16.0f);
		bool BrickCollide = isAccurateCollideBott(player, player.curr, BricksVertPosList, CurrPosYCollide, NoAdd, be, nd,
		                                          80.0f);
		be = find_min_iny(player, LuckyVertPosList);
		nd = find_max_iny_dist(player, LuckyVertPosList, 64.0f + (Yvelo) * 16.0f);
		bool LuckyCollide = isAccurateCollideBott(player, player.curr, LuckyVertPosList, CurrPosYCollide, NoAdd, be, nd,
		                                          80.0f);
		//recolide
		if (ObstacleCollide || BrickCollide || LuckyCollide) {
			bool isLanding;
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
				player.curr = { player.curr.x, CurrPosYCollide - (52.0f - player.property.getOrigin().y + 7.0f) };
			}
		}
		else player.curr = { player.curr.x, player.curr.y - 1.0f };
		// top update
		NoAdd = false;
		be = find_max_iny(player, ObstaclesVertPosList);
		nd = find_min_iny_dist(player, ObstaclesVertPosList, 64.0f - (Yvelo) * 16.0f);
		ObstacleCollide = isAccurateCollideTopt(player, player.curr, ObstaclesVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		const int br_be = find_max_iny(player, BricksVertPosList);
		const int br_nd = find_min_iny_dist(player, BricksVertPosList, 64.0f - (Yvelo) * 16.0f);
		BrickCollide = isAccurateCollideTopt(player, player.curr, BricksVertPosList, CurrPosYCollide, NoAdd, br_nd, br_be, 80.0f);
		const int lu_be = find_max_iny(player, LuckyVertPosList);
		const int lu_nd = find_min_iny_dist(player, LuckyVertPosList, 64.0f - (Yvelo) * 16.0f);
		LuckyCollide = isAccurateCollideTopt(player, player.curr, LuckyVertPosList, CurrPosYCollide, NoAdd, lu_nd, lu_be, 80.0f);
		if ((ObstacleCollide || BrickCollide || LuckyCollide) && Yvelo < 0.0f) {
			Yvelo = 0.0f;
			//snap back
			if (PowerState > 0 && !MarioCrouchDown)
				player.curr = { player.curr.x, CurrPosYCollide + (31.0f + player.property.getOrigin().y - PowerOffset[PowerState]) };
			else if ((PowerState > 0 && MarioCrouchDown) || (PowerState == 0 && MarioAppearing) || (PowerState == 0 && !MarioCrouchDown))
				player.curr = { player.curr.x, CurrPosYCollide + (31.0f + player.property.getOrigin().y - 30.0f) };
			// Start event Brick
			if (BrickCollide) {
				if (const std::vector<std::pair<float, float> > BrickPos = isCollideTopDetailed(player, player.curr, BricksVertPosList, br_nd, br_be, 80.0f); !BrickPos.empty()) {
					for (const auto&[fst, snd] : BrickPos) {
						HitEvent(fst, snd);
					}
				}
			}
			if (LuckyCollide) {
				if (const std::vector<std::pair<float, float> > LuckyPos = isCollideTopDetailed(player, player.curr, LuckyVertPosList, lu_nd, lu_be, 80.0f); !LuckyPos.empty()) {
					for (const auto&[fst, snd] : LuckyPos) {
						LuckyHitEvent(fst, snd);
					}
				}
			}
		}
	}
}
void MarioUpdateHitbox() {
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
}
void MarioUpdateAnimation() {
	//animation update

	//mariostate:
	// 0: idle, 1: run, 2: jump = fall, 3: crouch, 4: appear
	if (CanControlMario) {
		if (!MarioAppearing) {
			if (MarioCurrentFalling) {
				MarioState = 2;
				if (lastMarioState != MarioState) {
					MarioAnimation.setAnimation(3, 3, 100);
					lastMarioState = MarioState;
				}
				MarioAnimation.setDirection(static_cast<AnimationDirection>(!MarioDirection));
				MarioAnimation.AnimationUpdate(player.property.getPosition(), player.property.getOrigin());
			}
			else if (Yvelo == 0.0f && !(!MarioCurrentFalling && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && PowerState > 0)) {
				if (Xvelo == 0.0f) {
					MarioState = 0;
					if (lastMarioState != MarioState) {
						MarioAnimation.setAnimation(2, 2, 0);
						lastMarioState = MarioState;
					}
					MarioAnimation.setDirection(static_cast<AnimationDirection>(!MarioDirection));
					MarioAnimation.AnimationUpdate(player.property.getPosition(), player.property.getOrigin());
				}
				else {
					MarioState = 1;
					if (lastMarioState != MarioState) {
						MarioAnimation.setAnimation(0, 2);
						lastMarioState = MarioState;
					}
					MarioAnimation.setDirection(static_cast<AnimationDirection>(!MarioDirection));
					MarioAnimation.setFrequency(f_max(12.0f, f_min(Xvelo * 6.0f, 45.0f)));
					//MarioAnimation.setAnimationFrequency("RunSmallLeft", f_max(24.0f, f_min(Xvelo * 8.0f, 75.0f)));
					MarioAnimation.AnimationUpdate(player.property.getPosition(), player.property.getOrigin());
				}
			}
			else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && PowerState > 0) {
				MarioState = 3;
				if (lastMarioState != MarioState) {
					MarioAnimation.setAnimation(4, 4);
					lastMarioState = MarioState;
				}
				MarioAnimation.setDirection(static_cast<AnimationDirection>(!MarioDirection));
				MarioAnimation.AnimationUpdate(player.property.getPosition(), player.property.getOrigin());
			}
		}
		else {
			MarioState = 4;
			if (lastMarioState != MarioState) {
				MarioAnimation.setAnimation(5, 7, 100);
				lastMarioState = MarioState;
			}
			MarioAnimation.setDirection(static_cast<AnimationDirection>(!MarioDirection));
			MarioAnimation.AnimationUpdate(player.property.getPosition(), player.property.getOrigin());
		}
	}
}
void PowerDown() {
	if (!Invincible) {
		if (PowerState == 1) {
			Sounds.PlaySound("Pipe");
			SetPowerState(0);
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
	SetPowerState(0);
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
void MarioDraw() {
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
		if (CanControlMario) MarioAnimation.AnimationDraw(window);
	}
	else {
		if (!InvincibleState)
			if (CanControlMario) MarioAnimation.AnimationDraw(window);
	}
}
void InvincibleStateUpdate() {
	if (Invincible) {
		if (!InvincibleState) InvincibleState = true;
		else InvincibleState = false;
	}
}