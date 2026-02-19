#include "Object/Mario.hpp"
#include "Block/Brick.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/ImageManager.hpp"
#include "Block/LuckyBlock.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Sound.hpp"
#include "Core/Level.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/ExitGate.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Projectiles/MarioProjectile.hpp"

#include "Core/Logging.hpp"
#include "Core/Time.hpp"
//texture loading
void UpdateSequenceAnimation() {
	switch (PowerState) {
		case 0:
			MarioAnimation.setAnimationSequence(SmallMario);
			break;
		case 1:
			MarioAnimation.setAnimationSequence(BigMario);
			break;
		case 2:
			MarioAnimation.setAnimationSequence(FireMario);
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
	MarioAnimation.setAnimation(0, 0, 0, true);
	player.property.setOrigin({ 11, 51 });
	for (int i = 0; i < MARIO_IMAGE_WIDTH / MARIO_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("SmallMario_{}", i), "data/resources/SmallMario.png", sf::IntRect({MARIO_WIDTH*i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}));
		SmallMario.push_back(fmt::format("SmallMario_{}", i));
		ImageManager::AddTexture(fmt::format("BigMario_{}", i), "data/resources/BigMario.png", sf::IntRect({MARIO_WIDTH*i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}));
		BigMario.push_back(fmt::format("BigMario_{}", i));
		ImageManager::AddTexture(fmt::format("FireMario_{}", i), "data/resources/FireMario.png", sf::IntRect({MARIO_WIDTH*i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}));
		FireMario.push_back(fmt::format("FireMario_{}", i));
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
void MarioOutSideScreen() {
	if (player.curr.x <= player.property.getOrigin().x + ViewX) {
		player.curr = {player.property.getOrigin().x + ViewX, player.curr.y};
		OutsideWallLeft = true;
		if (Xvelo > 0.f) Xvelo = 0.f;
	}
	else if (player.curr.x > player.property.getOrigin().x + ViewX) OutsideWallLeft = false;
}
//false: right, true: left
void KeyboardMovement(const float deltaTime) {
	if (CanControlMario && !LevelCompleteEffect) {
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) && ((!MarioCrouchDown && !MarioCurrentFalling) || MarioCurrentFalling) && window.hasFocus()) {
			if (Xvelo == 0) MarioDirection = true;
			else if (!MarioDirection) {
				Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.375f * deltaTime);
				if (Xvelo < 0.f) Xvelo = 0.f;
			}
			//init speed
			if (Xvelo < 1.0f && MarioDirection) Xvelo = 1.0f;
			if (MarioDirection) {
				Xvelo += (Xvelo > player_speed ? 0.0f : 0.125f * deltaTime);
				if (Xvelo > 7.5f) Xvelo = 7.5f;
			}
		}
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) && ((!MarioCrouchDown && !MarioCurrentFalling) || MarioCurrentFalling) && window.hasFocus()) {
			if (Xvelo == 0) MarioDirection = false;
			else if (MarioDirection) {
				Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.375f * deltaTime);
				if (Xvelo < 0.f) Xvelo = 0.f;
			}
			if (Xvelo < 1.0f && !MarioDirection) Xvelo = 1.0f;
			if (!MarioDirection) {
				Xvelo += (Xvelo > player_speed ? 0.0f : 0.125f * deltaTime);
				if (Xvelo > 7.5f) Xvelo = 7.5f;
			}
		}
		else if ((((!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)))) && (!MarioCrouchDown || MarioCurrentFalling)) || (MarioCrouchDown && !MarioCurrentFalling)) {
			if (!(MarioCrouchDown && !MarioCurrentFalling)) Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.125f * deltaTime);
			else Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.25f * deltaTime);
		}
		if (Xvelo < 0.0f) Xvelo = 0.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) && !MarioCurrentFalling && window.hasFocus()) {
			if (!PreJump && !Holding) {
				SoundManager::PlaySound("Jump");
				Yvelo = -13.0f;
				Holding = true;
			}
			else if (PreJump) {
				SoundManager::PlaySound("Jump");
				Yvelo = -13.0f;
				PreJump = false;
				Holding = true;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !MarioCurrentFalling && PowerState > 0 && window.hasFocus()) {
			MarioCrouchDown = true;
		}
		else if (!MarioCurrentFalling) MarioCrouchDown = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) && window.hasFocus()) {
			if (Xvelo < 0.625f && Yvelo < 0.0f) Yvelo -= 0.4f * deltaTime;
			if (Xvelo >= 0.625f && Yvelo < 0.0f) Yvelo -= 0.5f * deltaTime;
			if (Yvelo >= 0.0f && !Holding) PreJump = true;
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) && window.hasFocus()) Holding = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) && !MarioCrouchDown && window.hasFocus()) player_speed = 7.5f;
		else player_speed = 4.375f;
		if (!MarioCurrentFalling && PreJump) PreJump = false;

		//Fire
		//MFCPP::Log::InfoPrint(fmt::format("Speed: {}, Limit: {}, Comp: {}", Xvelo, player_speed, Xvelo > player_speed));
	}
	else if (LevelCompleteEffect) {
		if (MarioDirection) MarioDirection = false;
		if (MarioCrouchDown) MarioCrouchDown = false;
		Xvelo = 2.5f;
	}
	else if (!CanControlMario) {
		Xvelo -= (Xvelo <= 0.0f ? 0.0f : 0.625f * deltaTime);
		if (Xvelo < 0.0f) Xvelo = 0.0f;
	}
	if (FireTimeCounting < FireTime) FireTimeCounting += 1.f * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) && window.hasFocus() && !isFireHolding && CanControlMario && !LevelCompleteEffect) {
		if (FireTimeCounting >= FireTime && PowerState > 1 && getAmountProjectile() < 2 && !MarioCrouchDown) {
			SoundManager::PlaySound("Fireball");
			FireTimeCounting = 0.f;
			switch (PowerState) {
				case 2:
					AddMarioProjectile(MarioDirection, FIREBALL, player.curr.x + (4.f * (MarioDirection ? -1.f : 1.f)), player.curr.y - 23.f);
				default: ;
			}
		}
	}
	isFireHolding = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X);
}
void MarioPosXUpdate(const float deltaTime) {
	if (!EffectActive) {
		if (!MarioDirection) player.curr = { player.curr.x + Xvelo * deltaTime, player.curr.y };
		else {
			if (!OutsideWallLeft)
				player.curr = { player.curr.x + (0 - Xvelo) * deltaTime, player.curr.y };
			else Xvelo = 0.f;
		}

		if (CanControlMario) {
			if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) || MarioCrouchDown) && Xvelo > 4.375f) {
				OverSpeed = true;
			}
			if (Xvelo > player_speed && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) || MarioCrouchDown)) {
				if (OverSpeed) {
					Xvelo -= 0.125f * deltaTime;
					if (Xvelo <= player_speed) OverSpeed = false;
				}
				else Xvelo = player_speed;
			}
			if (Xvelo > 7.5f && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) || MarioCrouchDown)) Xvelo = 7.5f;
		}
	}
}
void MarioVertXUpdate() {
	if (CanControlMario) {
		if (float PlatDistance; PlatformXCollision(MFCPP::CollisionObject(player.curr, player.property.getOrigin(), player.hitboxFloor), PlatDistance, Yvelo)) {
			player.curr.x += PlatDistance;
		}
		float CurrPosXCollide = 0, CurrPosYCollide = 0;
		const auto [fst, snd] = QuickCheckSideCollision(
			MFCPP::CollisionObject(player.curr, player.property.getOrigin(), player.hitboxWall), MarioDirection, CurrPosXCollide, CurrPosYCollide);
		//snap back
		if (MarioDirection) {
			if (fst) {
				Xvelo = 0.0f;
				player.curr = { CurrPosXCollide + 30.0f + 2.0f + player.property.getOrigin().x, player.curr.y };
			}
			else if (snd) {
				Xvelo = 0.0f;
				player.curr = { CurrPosXCollide - (0.0f + (23 - player.property.getOrigin().x)), player.curr.y };
			}
		}
		else {
			if (snd) {
				Xvelo = 0.0f;
				player.curr = { CurrPosXCollide - (0.0f + (23 - player.property.getOrigin().x)), player.curr.y };
			}
			else if (fst) {
				Xvelo = 0.0f;
				player.curr = { CurrPosXCollide + 30.0f + 2.0f + player.property.getOrigin().x, player.curr.y };
			}
		}
	}
}
void MarioPosYUpdate(const float deltaTime) {
	if (CanControlMario) {
		MarioCurrentFalling = true;
		player.curr = { player.curr.x, player.curr.y + Yvelo * deltaTime };
		Yvelo += (Yvelo >= 10.0f ? 0.0f : 1.f * deltaTime);
		if (Yvelo > 10.0f) Yvelo = 10.0f;
	}
}
void MarioVertYUpdate() {
	if (CanControlMario) {
		float CurrPosYCollide, CurrPosXCollide;
		//Collision With Platform
		if (float PlatPosY; PlatformYCollision(MFCPP::CollisionObject(player.curr, player.property.getOrigin(), player.hitboxFloor), PlatPosY, Yvelo, true)) {
			player.curr.y = PlatPosY;
			MarioCurrentFalling = false;
			Yvelo = 0.f;
		}
		//Collision With Obstacles
		if (QuickCheckBotCollision(MFCPP::CollisionObject({player.curr.x, player.curr.y + 1.0f}, player.property.getOrigin(), player.hitboxFloor), CurrPosXCollide, CurrPosYCollide)) {
			if (const float offset = std::min(Xvelo + 1.f, 3.f); Yvelo >= -Xvelo) {
				const float floorY = GetCurrFloorY(player.curr, CurrPosXCollide, CurrPosYCollide);
				if (player.curr.y < CurrPosYCollide + floorY - offset) return;
				if (MarioCurrentFalling) MarioCurrentFalling = false;
				player.curr = { player.curr.x, CurrPosYCollide + floorY - (52.0f - player.property.getOrigin().y) };
				Yvelo = 0.0f;
				return;
			}
		}
		// top update
		bool NoAdd = false;
		const bool ObstacleCollision = QuickCheckOnlyObstacleTopCollision(MFCPP::CollisionObject(player.curr, player.property.getOrigin(), player.hitboxFloor), CurrPosXCollide, CurrPosYCollide, NoAdd);
		const std::vector<std::pair<float, float>> BrickCollision = CheckCollisionTopDetailed(MFCPP::CollisionObject(player.curr, player.property.getOrigin(), player.hitboxFloor), CurrPosXCollide, CurrPosYCollide, NoAdd, 1);
		const std::vector<std::pair<float, float>> LuckyCollision = CheckCollisionTopDetailed(MFCPP::CollisionObject(player.curr, player.property.getOrigin(), player.hitboxFloor), CurrPosXCollide, CurrPosYCollide, NoAdd, 2);
		if (ObstacleCollision || !BrickCollision.empty() || !LuckyCollision.empty()) {
			// Start event Brick
			if (!BrickCollision.empty()) {
				for (const auto&[fst, snd] : BrickCollision) {
					HitEvent(fst, snd);
				}
			}
			if (!LuckyCollision.empty()) {
				for (const auto&[fst, snd] : LuckyCollision) {
					LuckyHitEvent(fst, snd);
				}
			}
			//snap back
			if (PowerState > 0 && !MarioCrouchDown)
				player.curr = { player.curr.x, CurrPosYCollide + (32.0f + player.property.getOrigin().y) };
			else if ((PowerState > 0 && MarioCrouchDown) || (PowerState == 0 && MarioAppearing) || (PowerState == 0 && !MarioCrouchDown))
				player.curr = { player.curr.x, CurrPosYCollide + (32.0f + player.property.getOrigin().y - 23.0f) };
			Yvelo = 0.0f;
			return;
		}
	}
}
void MarioUpdateHitbox() {
	if (PowerState > 0 && !MarioCrouchDown) {
		setHitbox(player.hitboxMain, sf::FloatRect({ 0.0f, 0.0f }, { 23.0f, 52.0f }));
		setHitbox(player.hitboxWall, sf::FloatRect({ 0.0f, 0.0f }, { 23.0f, 46.0f }));
		setHitbox(player.hitboxFloor, sf::FloatRect({ 0.0f, 0.0f }, { 23.0f, 52.0f }));
		setHitbox(player.hitboxTop, sf::FloatRect({ 0.0f, -1.0f }, { 23.0f, 2.0f }));
	}
	else if ((PowerState > 0 && MarioCrouchDown) || (PowerState == 0 && MarioAppearing) || (PowerState == 0 && !MarioCrouchDown)) {
		setHitbox(player.hitboxMain, sf::FloatRect({ 0.0f, 0.0f + 23.0f }, { 23.0f, 29.0f })); // 30
		setHitbox(player.hitboxWall, sf::FloatRect({ 0.0f, 0.0f + 23.0f }, { 23.0f, 23.0f }));
		setHitbox(player.hitboxFloor, sf::FloatRect({ 0.0f, 0.0f + 23.0f }, { 23.0f, 29.0f })); // 30
		setHitbox(player.hitboxTop, sf::FloatRect({ 0.0f, -1.0f + 23.0f }, { 23.0f, 2.0f }));
	}
}
void MarioUpdateAnimation() {
	//animation update

	//mariostate:
	// 0: idle, 1: run, 2: jump = fall, 3: crouch, 4: appear
	if (CanControlMario) {
		if (!MarioAppearing) {
			if (MarioCurrentFalling && !MarioCrouchDown) {
				MarioState = 2;
				if (lastMarioState != MarioState) {
					MarioAnimation.setAnimation(3, 3, 100, true);
					lastMarioState = MarioState;
				}
				MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(MarioDirection));
			}
			else if (MarioCrouchDown && PowerState > 0) {
				MarioState = 3;
				if (lastMarioState != MarioState) {
					MarioAnimation.setAnimation(4, 4, 50, true);
					lastMarioState = MarioState;
				}
				MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(MarioDirection));
			}
			else if (Yvelo == 0.f && !(!MarioCurrentFalling && MarioCrouchDown && PowerState > 0)) {
				if (Xvelo == 0.f && FireTimeCounting >= FireTime) {
					MarioState = 0;
					if (lastMarioState != MarioState) {
						MarioAnimation.setAnimation(2, 2, 0, true);
						lastMarioState = MarioState;
					}
					MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(MarioDirection));
				}
				else if (Xvelo != 0.f && FireTimeCounting >= FireTime){
					MarioState = 1;
					if (lastMarioState != MarioState) {
						MarioAnimation.setAnimation(0, 2, 50, true);
						lastMarioState = MarioState;
					}
					MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(MarioDirection));
					MarioAnimation.setFrequencyAnimation(f_max(12.0f, f_min(Xvelo * 8.0f, 45.0f)));
					//MarioAnimation.setAnimationFrequency("RunSmallLeft", f_max(24.0f, f_min(Xvelo * 8.0f, 75.0f)));
				}
				else if (FireTimeCounting < FireTime && PowerState > 1) {
					MarioState = 5;
					if (lastMarioState != MarioState) {
						MarioAnimation.setAnimation(9, 9, 0, true);
						lastMarioState = MarioState;
					}
					MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(MarioDirection));
				}
			}
		}
		else {
			MarioState = 4;
			if (lastMarioState != MarioState) {
				MarioAnimation.setAnimation(5, 7 + (PowerState > 1 ? 1 : 0), 100, true);
				lastMarioState = MarioState;
			}
			MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(MarioDirection));
		}
	}
}
void PowerDown() {
	//return; //Skip death, remove later
	if (LevelCompleteEffect) return;

	if (!Invincible) {
		if (PowerState > 1) {
			SoundManager::PlaySound("Pipe");
			SetPowerState(1);
			Invincible = true;
			InvincibleTimer.restart();
			InvincibleState = false;
		}
		else if (PowerState == 1) {
			SoundManager::PlaySound("Pipe");
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
	if (MarioCrouchDown) MarioCrouchDown = false;
	TimeReset();
	ExitGateEffectReset();
}
void CheckForDeath() {
	if (isOutScreenYBottom(player.property.getPosition().y, 96)) {
		CanControlMario = false;
		ActiveMarioEffect();
	}
}
void MarioDraw() {
	// check power state here
	if (static_cast<float>(AppearingTimer.getElapsedTime().asMilliseconds()) > (3000.0f / 91.0f) * 15.0f) MarioAppearing = false;
	if (PowerState != lastPowerState) {
		MarioAppearing = true;
		AppearingTimer.restart();
		lastPowerState = PowerState;
	}
	//then draw
	MarioAnimation.AnimationUpdate(player.property.getPosition(), {player.property.getOrigin().x + 4.0f, player.property.getOrigin().y + 7.0f});
	if (InvincibleTimer.getElapsedTime().asSeconds() > 2.0f) Invincible = false;
	if (!Invincible) {
		if (CanControlMario) MarioAnimation.AnimationDraw();
	}
	else {
		if (!InvincibleState)
			if (CanControlMario) MarioAnimation.AnimationDraw();
	}
}
void InvincibleStateUpdate() {
	if (Invincible) {
		if (!InvincibleState) InvincibleState = true;
		else InvincibleState = false;
	}
}