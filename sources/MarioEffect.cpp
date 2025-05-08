#include "../headers/Effect/MarioEffect.hpp"
#include "../headers/Core/TextureManager.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Object/Mario.hpp"
#include "../headers/Core/Music.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Object/ExitGate.hpp"
#include "../headers/Core/Interpolation.hpp"

#include "../resource.h"

TextureManager MarioEffectTextureManager;
sf::Sprite playerEffect(tempTex);
sf::Vector2f MarioEffectCurr;
sf::Vector2f MarioEffectPrev;
bool EffectActive = false;
sf::Clock MarioEffectTimer;
float MarioEffectYVelo = 0.0f;
void MarioEffectInit() {
	MarioEffectTextureManager.Loadingtexture(DEAD_MARIO_TEXTURE, "DEADMario", 0, 0, 32, 32);
	playerEffect.setTexture(*MarioEffectTextureManager.GetTexture("DEADMario"), true);
}
void SetPrevMarioEffectPos() {
	MarioEffectPrev = MarioEffectCurr;
}
void InterpolateMarioEffectPos(float alpha) {
	playerEffect.setPosition(linearInterpolation(MarioEffectPrev, MarioEffectCurr, alpha));
}
void MarioEffectStatusUpdate(float deltaTime) {
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
		Music.StopAllMODMusic();
		Music.StopAllOGGMusic();
		Music.PlayMODMusic("MarioDeath");
		EffectActive = true;
		playerEffect.setPosition({ player.property.getPosition().x - 10.0f, player.property.getPosition().y - 23.0f });
		MarioEffectCurr = MarioEffectPrev = playerEffect.getPosition();
		MarioEffectTimer.restart();
		MarioEffectYVelo = -10.0f;
		//prevent any bug occur
		ExitGateClock.reset();
	}
}
void MarioEffectDraw() {
	if (EffectActive) {
		if (!isOutScreen(playerEffect.getPosition().x, playerEffect.getPosition().y, 32, 32)) rTexture.draw(playerEffect);
	}
}