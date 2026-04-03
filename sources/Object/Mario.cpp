#include "Object/Mario.hpp"
#include "Block/Brick.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/ImageManager.hpp"
#include "Core/HitboxUtils.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Sound.hpp"
#include "Core/Level.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/ExitGate.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Class/CollisionObjectClass.hpp"

#include "Core/Time.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Projectile/MarioBeetroot.hpp"
#include "Object/Projectile/MarioFireball.hpp"

sf::FloatRect Mario::m_hitboxFloor;
sf::FloatRect Mario::m_hitboxTop;
sf::FloatRect Mario::m_hitboxWall;

MFCPP::SingleAnimationObject Mario::m_MarioAnimation;
MFCPP::ActiveObject<float> Mario::m_player;
sf::Vector2f Mario::m_velocity(0.f, 0.f);
bool Mario::m_FirstMarioDirection = false;
bool Mario::m_MarioDirection = Mario::m_FirstMarioDirection;
bool Mario::m_MarioCurrentFalling = true;
bool Mario::m_PreJump = false;
bool Mario::m_Holding;
bool Mario::m_MarioCrouchDown = false;
float Mario::m_player_speed;
int Mario::m_MarioState = 0;
int Mario::m_lastMarioState = -1;
int Mario::m_PowerState = 0;
int Mario::m_lastPowerState = 0;
int Mario::m_Lives = PLAYER_LIVES;
bool Mario::m_OverSpeed = false;
long long int Mario::m_Score = 0;
sf::Clock Mario::m_AppearingTimer;
sf::Clock Mario::m_InvincibleTimer;
bool Mario::m_Invincible = false;
bool Mario::m_InvincibleState = false;
bool Mario::m_MarioAppearing = false;
bool Mario::m_OutsideWallLeft = false;
float Mario::m_FireTime = 4.f;
float Mario::m_FireTimeCounting = m_FireTime;
bool Mario::m_isFireHolding = false;
bool Mario::m_CanControlMario = true;

//texture loading
void Mario::UpdateSequenceAnimation() {
	switch (m_PowerState) {
		case 0:
			m_MarioAnimation.setAnimationSequence(SmallMario);
			break;
		case 1:
			m_MarioAnimation.setAnimationSequence(BigMario);
			break;
		case 2:
			m_MarioAnimation.setAnimationSequence(FireMario);
			break;
		case 3:
			m_MarioAnimation.setAnimationSequence(BeetrootMario);
			break;
		default: ;
	}
}
void Mario::SetPowerState(const int ps) {
	m_PowerState = ps;
	UpdateSequenceAnimation();
}
void Mario::loadMarioRes() {
	m_AppearingTimer.restart();
	// Resources Loader;
	m_MarioAnimation.setAnimation(0, 0, 0, true);
	m_player.setOrigin({ 15, 62 });
	for (int i = 0; i < MARIO_IMAGE_WIDTH / MARIO_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("SmallMario_{}", i), "data/resources/SmallMario.png", sf::IntRect({MARIO_WIDTH*i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}));
		SmallMario.push_back(fmt::format("SmallMario_{}", i));
		ImageManager::AddTexture(fmt::format("BigMario_{}", i), "data/resources/BigMario.png", sf::IntRect({MARIO_WIDTH*i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}));
		BigMario.push_back(fmt::format("BigMario_{}", i));
		ImageManager::AddTexture(fmt::format("FireMario_{}", i), "data/resources/FireMario.png", sf::IntRect({MARIO_WIDTH*i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}));
		FireMario.push_back(fmt::format("FireMario_{}", i));
		ImageManager::AddTexture(fmt::format("BeetrootMario_{}", i), "data/resources/BeetrootMario.png", sf::IntRect({MARIO_WIDTH*i, 0}, {MARIO_WIDTH, MARIO_HEIGHT}));
		BeetrootMario.push_back(fmt::format("BeetrootMario_{}", i));
	}

	UpdateSequenceAnimation();
}
//sprite function
void Mario::SetPrevMarioPos() {
	m_player.setPreviousPosition(m_player.getCurrentPosition());
}
void Mario::InterpolateMarioPos(const float alpha) {
	m_player.setInterpolatedPosition(linearInterpolation(m_player.getPreviousPosition(), m_player.getCurrentPosition(), alpha));
}
void Mario::MarioOutSideScreen() {
	if (m_player.getCurrentPosition().x <= m_player.getOrigin().x + ViewX) {
		m_player.setCurrentPosition({m_player.getOrigin().x + ViewX, m_player.getCurrentPosition().y});
		m_OutsideWallLeft = true;
		if (m_velocity.x > 0.f) m_velocity.x = 0.f;
	}
	else if (m_player.getCurrentPosition().x > m_player.getOrigin().x + ViewX) m_OutsideWallLeft = false;
}
//false: right, true: left
void Mario::KeyboardMovement(const float deltaTime) {
	if (m_CanControlMario && !LevelCompleteEffect) {
		if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) && ((!m_MarioCrouchDown && !m_MarioCurrentFalling) || m_MarioCurrentFalling) && window.hasFocus()) {
			if (m_velocity.x == 0) m_MarioDirection = true;
			else if (!m_MarioDirection) {
				m_velocity.x -= (m_velocity.x <= 0.0f ? 0.0f : 0.375f * deltaTime);
				if (m_velocity.x < 0.f) m_velocity.x = 0.f;
			}
			//init speed
			if (m_velocity.x < 1.0f && m_MarioDirection) m_velocity.x = 1.0f;
			if (m_MarioDirection) {
				m_velocity.x += (m_velocity.x > m_player_speed ? 0.0f : 0.125f * deltaTime);
				if (m_velocity.x > 7.5f) m_velocity.x = 7.5f;
			}
		}
		else if ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)) && ((!m_MarioCrouchDown && !m_MarioCurrentFalling) || m_MarioCurrentFalling) && window.hasFocus()) {
			if (m_velocity.x == 0) m_MarioDirection = false;
			else if (m_MarioDirection) {
				m_velocity.x -= (m_velocity.x <= 0.0f ? 0.0f : 0.375f * deltaTime);
				if (m_velocity.x < 0.f) m_velocity.x = 0.f;
			}
			if (m_velocity.x < 1.0f && !m_MarioDirection) m_velocity.x = 1.0f;
			if (!m_MarioDirection) {
				m_velocity.x += (m_velocity.x > m_player_speed ? 0.0f : 0.125f * deltaTime);
				if (m_velocity.x > 7.5f) m_velocity.x = 7.5f;
			}
		}
		else if ((((!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && !sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)) || ((sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left) && sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)))) && (!m_MarioCrouchDown || m_MarioCurrentFalling)) || (m_MarioCrouchDown && !m_MarioCurrentFalling)) {
			if (!(m_MarioCrouchDown && !m_MarioCurrentFalling)) m_velocity.x -= (m_velocity.x <= 0.0f ? 0.0f : 0.125f * deltaTime);
			else m_velocity.x -= (m_velocity.x <= 0.0f ? 0.0f : 0.25f * deltaTime);
		}
		if (m_velocity.x < 0.0f) m_velocity.x = 0.0f;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) && !m_MarioCurrentFalling && window.hasFocus()) {
			if (!m_PreJump && !m_Holding) {
				SoundManager::PlaySound("Jump");
				m_velocity.y = -13.0f;
				m_Holding = true;
			}
			else if (m_PreJump) {
				SoundManager::PlaySound("Jump");
				m_velocity.y = -13.0f;
				m_PreJump = false;
				m_Holding = true;
			}
		}
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down) && !m_MarioCurrentFalling && m_PowerState > 0 && window.hasFocus()) {
			m_MarioCrouchDown = true;
		}
		else if (!m_MarioCurrentFalling) m_MarioCrouchDown = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) && window.hasFocus()) {
			if (m_velocity.x < 0.625f && m_velocity.y < 0.0f) m_velocity.y -= 0.4f * deltaTime;
			if (m_velocity.x >= 0.625f && m_velocity.y < 0.0f) m_velocity.y -= 0.5f * deltaTime;
			if (m_velocity.y >= 0.0f && !m_Holding) m_PreJump = true;
		}
		if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) && window.hasFocus()) m_Holding = false;
		if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) && !m_MarioCrouchDown && window.hasFocus()) m_player_speed = 7.5f;
		else m_player_speed = 4.375f;
		if (!m_MarioCurrentFalling && m_PreJump) m_PreJump = false;

		//Fire
		//MFCPP::Log::InfoPrint(fmt::format("Speed: {}, Limit: {}, Comp: {}", Xvelo, player_speed, Xvelo > player_speed));
	}
	else if (LevelCompleteEffect) {
		if (m_MarioDirection) m_MarioDirection = false;
		if (m_MarioCrouchDown) m_MarioCrouchDown = false;
		m_velocity.x = 2.5f;
	}
	else if (!m_CanControlMario) {
		m_velocity.x -= (m_velocity.x <= 0.0f ? 0.0f : 0.625f * deltaTime);
		if (m_velocity.x < 0.0f) m_velocity.x = 0.0f;
	}
	if (m_FireTimeCounting < m_FireTime) m_FireTimeCounting += 1.f * deltaTime;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) && window.hasFocus() && !m_isFireHolding && m_CanControlMario && !LevelCompleteEffect) {
		if (m_FireTimeCounting >= m_FireTime && m_PowerState > 1 && GameScene::projectileManager.getMarioProjectileList().size() < 2 && !m_MarioCrouchDown) {
			SoundManager::PlaySound("Fireball");
			m_FireTimeCounting = 0.f;
			switch (m_PowerState) {
				case 2:
					GameScene::projectileManager.addProjectile<MarioFireball>(m_MarioDirection, sf::Vector2f(m_player.getCurrentPosition().x + (4.f * (m_MarioDirection ? -1.f : 1.f)), m_player.getCurrentPosition().y - 23.f));
					break;
				case 3:
					GameScene::projectileManager.addProjectile<MarioBeetroot>(m_MarioDirection, sf::Vector2f(m_player.getCurrentPosition().x + (4.f * (m_MarioDirection ? -1.f : 1.f)), m_player.getCurrentPosition().y - 11.f));
					break;
				default: ;
			}
		}
	}
	m_isFireHolding = sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X);
}
void Mario::MarioPosXUpdate(const float deltaTime) {
	if (!EffectActive) {
		if (!m_MarioDirection) m_player.setCurrentPosition({m_player.getCurrentPosition().x + m_velocity.x * deltaTime, m_player.getCurrentPosition().y});
		else {
			if (!m_OutsideWallLeft)
				m_player.setCurrentPosition({ m_player.getCurrentPosition().x + (0 - m_velocity.x) * deltaTime, m_player.getCurrentPosition().y });
			else m_velocity.x = 0.f;
		}

		if (m_CanControlMario) {
			if ((!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) || m_MarioCrouchDown) && m_velocity.x > 4.375f) {
				m_OverSpeed = true;
			}
			if (m_velocity.x > m_player_speed && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) || m_MarioCrouchDown)) {
				if (m_OverSpeed) {
					m_velocity.x -= 0.125f * deltaTime;
					if (m_velocity.x <= m_player_speed) m_OverSpeed = false;
				}
				else m_velocity.x = m_player_speed;
			}
			if (m_velocity.x > 7.5f && (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X) || m_MarioCrouchDown)) m_velocity.x = 7.5f;
		}
	}
}
void Mario::MarioVertXUpdate() {
	if (m_CanControlMario) {
		if (float PlatDistance; PlatformXCollision(MFCPP::CollisionObject(m_player.getCurrentPosition(), m_player.getOrigin(), m_hitboxFloor), PlatDistance, m_velocity.y)) {
			m_player.setCurrentPosition({m_player.getCurrentPosition().x + PlatDistance, m_player.getCurrentPosition().y});
		}
		float CurrPosXCollide = 0, CurrPosYCollide = 0;
		const auto [fst, snd] = QuickCheckSideCollision(
			MFCPP::CollisionObject(m_player.getCurrentPosition(), m_player.getOrigin(), m_hitboxWall), m_MarioDirection, CurrPosXCollide, CurrPosYCollide);
		//snap back
		if (m_MarioDirection) {
			if (fst) {
				m_velocity.x = 0.0f;
				m_player.setCurrentPosition({CurrPosXCollide + 28.f + m_player.getOrigin().x, m_player.getCurrentPosition().y});
			}
			else if (snd) {
				m_velocity.x = 0.0f;
				m_player.setCurrentPosition({CurrPosXCollide - (0.0f + (27 - m_player.getOrigin().x)), m_player.getCurrentPosition().y});
			}
		}
		else {
			if (snd) {
				m_velocity.x = 0.0f;
				m_player.setCurrentPosition({CurrPosXCollide - (0.0f + (27 - m_player.getOrigin().x)), m_player.getCurrentPosition().y});
			}
			else if (fst) {
				m_velocity.x = 0.0f;
				m_player.setCurrentPosition({CurrPosXCollide + 28.f + m_player.getOrigin().x, m_player.getCurrentPosition().y});
			}
		}
	}
}
void Mario::MarioPosYUpdate(const float deltaTime) {
	if (m_CanControlMario) {
		m_MarioCurrentFalling = true;
		m_player.setCurrentPosition({m_player.getCurrentPosition().x, m_player.getCurrentPosition().y + m_velocity.y * deltaTime});
		m_velocity.y += (m_velocity.y >= 10.0f ? 0.0f : 1.f * deltaTime);
		if (m_velocity.y > 10.0f) m_velocity.y = 10.0f;
	}
}
void Mario::MarioVertYBottomUpdate() {
	if (m_CanControlMario) {
		float CurrPosYCollide, CurrPosXCollide;
		//Collision With Platform
		if (float PlatPosY; PlatformYCollision(MFCPP::CollisionObject(m_player.getCurrentPosition(), m_player.getOrigin(), m_hitboxFloor), PlatPosY, m_velocity.y, true)) {
			m_player.setCurrentPosition({m_player.getCurrentPosition().x, PlatPosY + 1.f});
			m_MarioCurrentFalling = false;
			m_velocity.y = 0.f;
		}
		//Collision With Obstacles
		if (QuickCheckBotCollision(MFCPP::CollisionObject({m_player.getCurrentPosition().x, m_player.getCurrentPosition().y + 1.0f}, m_player.getOrigin(), m_hitboxFloor), CurrPosXCollide, CurrPosYCollide)) {
			if (const float offset = std::min(m_velocity.x + 1.f, 3.f); m_velocity.y >= -m_velocity.x) {
				const float floorY = GetCurrFloorY(m_player.getCurrentPosition(), CurrPosXCollide, CurrPosYCollide);
				if (m_player.getCurrentPosition().y < CurrPosYCollide + floorY - offset) return;
				if (m_MarioCurrentFalling) m_MarioCurrentFalling = false;
				m_player.setCurrentPosition({m_player.getCurrentPosition().x, CurrPosYCollide + floorY - (MARIO_HEIGHT - m_player.getOrigin().y)});
				m_velocity.y = 0.f;
			}
		}
	}
}
void Mario::MarioVertYTopUpdate() {
	if (m_CanControlMario) {
		float CurrPosYCollide, CurrPosXCollide;
		bool NoAdd = false;
		if (QuickCheckTopCollision(MFCPP::CollisionObject(m_player.getCurrentPosition(), m_player.getOrigin(), m_hitboxFloor), CurrPosXCollide, CurrPosYCollide)) {
			//snap back
			if (m_PowerState > 0 && !m_MarioCrouchDown)
				m_player.setCurrentPosition({m_player.getCurrentPosition().x, CurrPosYCollide + (32.0f + m_player.getOrigin().y - 10.f)});
			else if ((m_PowerState > 0 && m_MarioCrouchDown) || (m_PowerState == 0 && m_MarioAppearing) || (m_PowerState == 0 && !m_MarioCrouchDown))
				m_player.setCurrentPosition({m_player.getCurrentPosition().x, CurrPosYCollide + (32.0f + m_player.getOrigin().y - 10.f - 23.0f)});
			m_velocity.y = 0.0f;
		}
	}
}
void Mario::MarioUpdateHitbox() {
	if (m_PowerState > 0 && !m_MarioCrouchDown) {
		m_player.setHitbox(sf::FloatRect({ 0.0f + 4.f, 0.f + 10.f }, { 23.0f, 52.0f }));
		m_hitboxWall = sf::FloatRect({ 0.0f + 4.f, 0.f + 10.f}, { 23.0f, 46.0f });
		m_hitboxFloor = sf::FloatRect({ 0.0f + 4.f, 0.f + 10.f }, { 23.0f, 52.0f });
		m_hitboxTop = sf::FloatRect({ 0.0f + 4.f, -1.0f + 10.f }, { 23.0f, 2.0f });
	}
	else if ((m_PowerState > 0 && m_MarioCrouchDown) || (m_PowerState == 0 && m_MarioAppearing) || (m_PowerState == 0 && !m_MarioCrouchDown)) {
		m_player.setHitbox(sf::FloatRect({ 0.0f + 4.f, 0.0f + 10.f + 23.0f }, { 23.0f, 29.0f })); // 30
		m_hitboxWall = sf::FloatRect({ 0.0f + 4.f, 0.0f + 10.f + 23.0f }, { 23.0f, 23.0f });
		m_hitboxFloor = sf::FloatRect({ 0.0f + 4.f, 0.0f + 10.f + 23.0f }, { 23.0f, 29.0f }); // 30
		m_hitboxTop = sf::FloatRect({ 0.0f + 4.f, -1.0f + 10.f + 23.0f }, { 23.0f, 2.0f });
	}
}
void Mario::MarioUpdateAnimation() {
	//animation update

	//mariostate:
	// 0: idle, 1: run, 2: jump = fall, 3: crouch, 4: appear
	if (m_CanControlMario) {
		if (!m_MarioAppearing) {
			if (m_MarioCurrentFalling && !m_MarioCrouchDown) {
				m_MarioState = 2;
				if (m_lastMarioState != m_MarioState) {
					m_MarioAnimation.setAnimation(3, 3, 100, true);
					m_lastMarioState = m_MarioState;
				}
				m_MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(m_MarioDirection));
			}
			else if (m_MarioCrouchDown && m_PowerState > 0) {
				m_MarioState = 3;
				if (m_lastMarioState != m_MarioState) {
					m_MarioAnimation.setAnimation(4, 4, 50, true);
					m_lastMarioState = m_MarioState;
				}
				m_MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(m_MarioDirection));
			}
			else if (m_velocity.y == 0.f && !(!m_MarioCurrentFalling && m_MarioCrouchDown && m_PowerState > 0)) {
				if (m_velocity.x == 0.f && m_FireTimeCounting >= m_FireTime) {
					m_MarioState = 0;
					if (m_lastMarioState != m_MarioState) {
						m_MarioAnimation.setAnimation(2, 2, 0, true);
						m_lastMarioState = m_MarioState;
					}
					m_MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(m_MarioDirection));
				}
				else if (m_velocity.x != 0.f && m_FireTimeCounting >= m_FireTime){
					m_MarioState = 1;
					if (m_lastMarioState != m_MarioState) {
						m_MarioAnimation.setAnimation(0, 2, 50, true);
						m_lastMarioState = m_MarioState;
					}
					m_MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(m_MarioDirection));
					m_MarioAnimation.setFrequencyAnimation(f_max(12.0f, f_min(m_velocity.x * 8.0f, 45.0f)));
					//MarioAnimation.setAnimationFrequency("RunSmallLeft", f_max(24.0f, f_min(Xvelo * 8.0f, 75.0f)));
				}
				else if (m_FireTimeCounting < m_FireTime && m_PowerState > 1) {
					m_MarioState = 5;
					if (m_lastMarioState != m_MarioState) {
						m_MarioAnimation.setAnimation(9, 9, 0, true);
						m_lastMarioState = m_MarioState;
					}
					m_MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(m_MarioDirection));
				}
			}
		}
		else {
			m_MarioState = 4;
			if (m_lastMarioState != m_MarioState) {
				m_MarioAnimation.setAnimation(5, 7 + (m_PowerState > 1 ? 1 : 0), 100, true);
				m_lastMarioState = m_MarioState;
			}
			m_MarioAnimation.setAnimationDirection(static_cast<AnimationDirection>(m_MarioDirection));
		}
	}
}
void Mario::PowerDown() {
	//return; //Skip death, remove later
	if (LevelCompleteEffect || m_Invincible) return;

	if (!m_Invincible) {
		if (m_PowerState > 1) {
			SoundManager::PlaySound("Pipe");
			SetPowerState(1);
			m_Invincible = true;
			m_InvincibleTimer.restart();
			m_InvincibleState = false;
		}
		else if (m_PowerState == 1) {
			SoundManager::PlaySound("Pipe");
			SetPowerState(0);
			m_Invincible = true;
			m_InvincibleTimer.restart();
			m_InvincibleState = false;
		}
		else if (m_PowerState == 0 && m_CanControlMario && !LevelCompleteEffect) {
			m_CanControlMario = false;
			ActiveMarioEffect();
		}
	}
}
void Mario::Death() {
	if (m_Lives <= 0) window.close();
	else --m_Lives;
	Objectbuilding();
	m_velocity = {0.f, 0.f};
	SetPowerState(0);
	m_lastPowerState = 0;
	LevelCompleteEffect = false;
	m_MarioDirection = m_FirstMarioDirection;
	ExitGateForeActive = true;
	if (m_MarioCrouchDown) m_MarioCrouchDown = false;
	TimeReset();
	ExitGateEffectReset();
}
void Mario::CheckForDeath() {
	if (isOutOfScreenYBottom(MFCPP::CollisionObject(m_player.getCurrentPosition(), m_player.getOrigin(), m_player.getHitbox()), 64.f)) {
		m_CanControlMario = false;
		ActiveMarioEffect();
	}
}
void Mario::MarioDraw() {
	// check power state
	HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(m_player.getHitbox(), m_player.getCurrentPosition() - m_player.getOrigin(), sf::Color::Blue));
	if (static_cast<float>(m_AppearingTimer.getElapsedTime().asMilliseconds()) > (3000.0f / 91.0f) * 15.0f) m_MarioAppearing = false;
	if (m_PowerState != m_lastPowerState) {
		m_MarioAppearing = true;
		m_AppearingTimer.restart();
		m_lastPowerState = m_PowerState;
	}
	//draw
	m_MarioAnimation.AnimationUpdate(m_player.getInterpolatedPosition(), m_player.getOrigin());
	if (m_InvincibleTimer.getElapsedTime().asSeconds() > 2.0f) m_Invincible = false;
	if (!m_Invincible) {
		if (m_CanControlMario) m_MarioAnimation.AnimationDraw();
	}
	else {
		if (!m_InvincibleState)
			if (m_CanControlMario) m_MarioAnimation.AnimationDraw();
	}
}
void Mario::InvincibleStateUpdate() {
	if (m_Invincible) {
		if (!m_InvincibleState) m_InvincibleState = true;
		else m_InvincibleState = false;
	}
}
void Mario::setXvelocity(float val) {
	m_velocity.x = val;
}
void Mario::setYvelocity(float val) {
	m_velocity.y = val;
}
float Mario::getXvelocity() {
	return m_velocity.x;
}
float Mario::getYvelocity() {
	return m_velocity.y;
}
sf::FloatRect Mario::getHitbox() {
	return m_player.getHitbox();
}

sf::Vector2f Mario::getOrigin() {
	return m_player.getOrigin();
}

void Mario::setCurrentPosition(const sf::Vector2f& val) {
	m_player.setCurrentPosition(val);
}
sf::Vector2f Mario::getCurrentPosition() {
	return m_player.getCurrentPosition();
}
void Mario::resetPreviousPosition() {
	m_player.setPreviousPosition(m_player.getCurrentPosition());
	m_player.setInterpolatedPosition(m_player.getCurrentPosition());
}

void Mario::setDirection(bool val) {
	m_MarioDirection = val;
}

void Mario::setFirstDirection(bool val) {
	m_FirstMarioDirection = val;
}

bool Mario::getDirection() {
	return m_MarioDirection;
}

bool Mario::getFirstDirection() {
	return m_FirstMarioDirection;
}
void Mario::setLives(int val) {
	m_Lives = val;
}
void Mario::setScore(long long val) {
	m_Score = val;
}
int Mario::getLives() {
	return m_Lives;
}
int Mario::getScore() {
	return m_Score;
}

bool Mario::isFalling() {
	return m_MarioCurrentFalling;
}
bool Mario::isPrejump() {
	return m_PreJump;
}

int Mario::getPowerState() {
	return m_PowerState;
}

sf::FloatRect Mario::getHitboxWall() {
	return m_hitboxWall;
}

void Mario::setCanControl(bool val) {
	m_CanControlMario = val;
}

bool Mario::getCanControl() {
	return m_CanControlMario;
}

sf::Vector2f Mario::getInterpolatedPosition() {
	return m_player.getInterpolatedPosition();
}
