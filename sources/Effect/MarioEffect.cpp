#include "Effect/MarioEffect.hpp"
#include "Core/ImageManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Object/Mario.hpp"
#include "Core/Music.hpp"
#include "Core/Scroll.hpp"
#include "Object/ExitGate.hpp"
#include "Core/Interpolation.hpp"
#include "Core/MusicManager.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Class/ActiveObjectClass.hpp"

static MFCPP::StaticAnimationObject playerEffect;
static MFCPP::ActiveObject<float> playerPos;
bool EffectActive = false;
sf::Clock MarioEffectTimer;
float MarioEffectYVelo = 0.0f;
void MarioEffectInit() {
	ImageManager::AddTexture("DEADMario", "data/resources/MarioDead.png");
	playerEffect.setTexture("DEADMario");
}
void SetPrevMarioEffectPos() {
	playerPos.setPreviousPosition(playerPos.getCurrentPosition());
}
void InterpolateMarioEffectPos(const float alpha) {
	playerPos.setInterpolatedPosition(linearInterpolation(playerPos.getPreviousPosition(), playerPos.getCurrentPosition(), alpha));
}
void MarioEffectStatusUpdate(const float deltaTime) {
	if (EffectActive) {
		if (MarioEffectTimer.getElapsedTime().asSeconds() >= 4.0f) {
			CanControlMario = true;
			EffectActive = false;
			Death();
		}
		else if (MarioEffectTimer.getElapsedTime().asSeconds() >= 0.5f && MarioEffectTimer.getElapsedTime().asSeconds() < 4.0f) {
			MarioEffectYVelo += (MarioEffectYVelo >= 10.0f ? 0.0f : 1.f * deltaTime * 0.3f);
			playerPos.move(sf::Vector2f(0.f, MarioEffectYVelo * deltaTime));
		}
	}
}
void ActiveMarioEffect() {
	if (!EffectActive) {
		MusicManager::StopAllMusic();
		MusicManager::PlayMusic("MarioDeath");
		EffectActive = true;
		playerPos.setCurrentPosition({ player.property.getPosition().x - 14.0f, player.property.getPosition().y - 30.0f });
		playerPos.setPreviousPosition(playerPos.getCurrentPosition());
		playerPos.setInterpolatedPosition(playerPos.getCurrentPosition());
		MarioEffectTimer.restart();
		MarioEffectYVelo = -10.0f;
		//prevent any bug occur
		ExitGateClock.reset();
	}
}
void MarioEffectDraw() {
	if (EffectActive) {
		if (!isOutScreen(playerPos.getInterpolatedPosition().x, playerPos.getInterpolatedPosition().y, 32, 32)) {
			playerEffect.AnimationUpdate(playerPos.getInterpolatedPosition(), sf::Vector2f(0.f, 0.f));
			playerEffect.AnimationDraw();
		}
	}
}