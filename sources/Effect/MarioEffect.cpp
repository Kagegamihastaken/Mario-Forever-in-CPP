#include "Effect/MarioEffect.hpp"
#include "Core/ImageManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Object/Mario.hpp"
#include "Core/Music.hpp"
#include "Core/Scroll.hpp"
#include "Object/ExitGate.hpp"
#include "Core/Interpolation.hpp"
#include "Core/MusicManager.hpp"

sf::Sprite playerEffect(tempTex);
sf::Vector2f MarioEffectCurr;
sf::Vector2f MarioEffectPrev;
bool EffectActive = false;
sf::Clock MarioEffectTimer;
float MarioEffectYVelo = 0.0f;
void MarioEffectInit() {
	ImageManager::AddTexture("DEADMario", "data/resources/MarioDead.png");
	playerEffect.setTexture(*ImageManager::GetReturnTexture("DEADMario"), true);
}
void SetPrevMarioEffectPos() {
	MarioEffectPrev = MarioEffectCurr;
}
void InterpolateMarioEffectPos(const float alpha) {
	playerEffect.setPosition(linearInterpolation(MarioEffectPrev, MarioEffectCurr, alpha));
}
void MarioEffectStatusUpdate(const float deltaTime) {
	if (EffectActive) {
		if (MarioEffectTimer.getElapsedTime().asSeconds() >= 4.0f) {
			CanControlMario = true;
			EffectActive = false;
			Death();
		}
		else if (MarioEffectTimer.getElapsedTime().asSeconds() >= 0.5f && MarioEffectTimer.getElapsedTime().asSeconds() < 4.0f) {
			MarioEffectYVelo += (MarioEffectYVelo >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f);
			MarioEffectCurr = { MarioEffectCurr.x, MarioEffectCurr.y + MarioEffectYVelo * deltaTime };
			MarioEffectYVelo += (MarioEffectYVelo >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f);
		}
	}
}
void ActiveMarioEffect() {
	if (!EffectActive) {
		MusicManager::StopAllMusic();
		MusicManager::PlayMusic("MarioDeath");
		EffectActive = true;
		playerEffect.setPosition({ player.property.getPosition().x - 14.0f, player.property.getPosition().y - 30.0f });
		MarioEffectCurr = MarioEffectPrev = playerEffect.getPosition();
		MarioEffectTimer.restart();
		MarioEffectYVelo = -10.0f;
		//prevent any bug occur
		ExitGateClock.reset();
	}
}
void MarioEffectDraw() {
	if (EffectActive) {
		if (!isOutScreen(playerEffect.getPosition().x, playerEffect.getPosition().y, 32, 32)) window.draw(playerEffect);
	}
}