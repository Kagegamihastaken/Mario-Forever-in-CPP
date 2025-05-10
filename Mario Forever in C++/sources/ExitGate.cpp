#define _USE_MATH_DEFINES
#include <SFML/Graphics.hpp>

#include "../headers/Object/ExitGate.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/TextureManager.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Effect/ScoreEffect.hpp"
#include "../headers/Core/Music.hpp"
#include "../headers/Core/Animate/LocalAnimationManager.hpp"
#include "../headers/Core/Interpolation.hpp"

#include "../headers/Core/Loading/enum.hpp"

#include <random>
#include <cmath>

TextureManager ExitGateTextureManager;
LocalAnimationManager ExitGateIndicatorAnimation;
sf::Sprite ExitGateBack(tempTex);
sf::Sprite ExitGateFore(tempTex);
sf::Sprite ExitGateForeEffect(tempTex);
sf::Sprite ExitGateIndicator(tempTex);
sf::Clock ExitGateClock;

sf::Vector2f ExitGateForeCurr = sf::Vector2f(0.0f, 0.0f);
sf::Vector2f ExitGateForePrev = sf::Vector2f(0.0f, 0.0f);
sf::Vector2f ExitGateForeEffectCurr = sf::Vector2f(0.0f, 0.0f);
sf::Vector2f ExitGateForeEffectPrev = sf::Vector2f(0.0f, 0.0f);

const float ExitGateForeYLimit = 222.0f;
float ExitGateForeY = 0.0f;
const float ExitGateForeYSpeed = 3.05f;
bool ExitGateState = false;
bool ExitGateForeActive = true;
bool LevelCompleteEffect = false;
float ExitGateForeEffectSpeed = 0.0f;
float ExitGateForeEffectYSpeed = 0.0f;
float ExitGateForeRender = true;

std::random_device seed;
std::uniform_real_distribution<float> dis(123.75f, 146.25f);

void ExitGateInit() {
	ExitGateTextureManager.Loadingtexture("data/resources/ExitGateBack.png", "ExitGateBack", 0, 0, 95, 288);
	ExitGateTextureManager.Loadingtexture("data/resources/ExitGateFore.png", "ExitGateFore", 0, 0, 44, 16);
	ExitGateTextureManager.Loadingtexture("data/resources/ExitGateForeEffect.png", "ExitGateForeEffect", 0, 0, 44, 16);
	ExitGateTextureManager.Loadingtexture("data/resources/ExitGateIndicator.png", "ExitGateIndicator", 0, 0, 93, 32);
	//ExitGateTextureManager.LoadingAnimatedTexture(EXIT_GATE_INDICATOR_TEXTURE, "ExitGateIndicator", 0, 2, 0, 31, 32);
	ExitGateBack.setTexture(*ExitGateTextureManager.GetTexture("ExitGateBack"), true);
	ExitGateBack.setOrigin({ 0.0f, 287.0f });

	ExitGateFore.setTexture(*ExitGateTextureManager.GetTexture("ExitGateFore"), true);
	ExitGateFore.setOrigin({ 23.0f, 0.0f });

	ExitGateForeEffect.setTexture(*ExitGateTextureManager.GetTexture("ExitGateForeEffect"), true);
	ExitGateForeEffect.setOrigin({ 21.0f, 7.0f });
	ExitGateClock.reset();

	ExitGateIndicatorAnimation.setAnimation(0, 2, 31, 32, 0, 50);
	ExitGateIndicatorAnimation.setTexture(ExitGateIndicator, ExitGateTextureManager.GetTexture("ExitGateIndicator"));
	ExitGateIndicator.setOrigin({ 0.0f, 31.0f });
}
void SetPrevExitGatePos() {
	ExitGateForePrev = ExitGateForeCurr;
	ExitGateForeEffectPrev = ExitGateForeEffectCurr;
}
void InterpolateExitGatePos(float alpha) {
	ExitGateFore.setPosition(linearInterpolation(ExitGateForePrev, ExitGateForeCurr, alpha));
	ExitGateForeEffect.setPosition(linearInterpolation(ExitGateForeEffectPrev, ExitGateForeEffectCurr, alpha));
}
void ExitGateStatusUpdate(float deltaTime) {
	if (ExitGateForeActive) {
		if (ExitGateIndicator.getPosition().x <= player.curr.x - 24.0f && !PreJump && !MarioCurrentFalling) {
			AddScoreEffect(SCORE_100, player.curr.x, player.curr.y);
			ExitGateClock.start();
			LevelCompleteEffect = true;
			Music.StopAllMODMusic();
			Music.StopAllOGGMusic();
			Music.PlayOGGMusic("LevelComplete");
			ExitGateForeActive = false;
		}
		if (isCollide(player.hitboxMain, player.property, player.curr, getGlobalHitbox(sf::FloatRect({ 0.0f, 0.0f }, { 44.0f, 16.0f }), ExitGateFore))) {
			if (ExitGateFore.getPosition().y <= ExitGateBack.getPosition().y - 266.0f + 30.0f) AddScoreEffect(SCORE_10000, ExitGateFore.getPosition().x, ExitGateFore.getPosition().y);
			else if (ExitGateFore.getPosition().y >= ExitGateBack.getPosition().y - 266.0f + 30.0f && ExitGateFore.getPosition().y <= ExitGateBack.getPosition().y - 266.0f + 60.0f) AddScoreEffect(SCORE_5000, ExitGateFore.getPosition().x, ExitGateFore.getPosition().y);
			else if (ExitGateFore.getPosition().y >= ExitGateBack.getPosition().y - 266.0f + 60.0f && ExitGateFore.getPosition().y <= ExitGateBack.getPosition().y - 266.0f + 100.0f) AddScoreEffect(SCORE_2000, ExitGateFore.getPosition().x, ExitGateFore.getPosition().y);
			else if (ExitGateFore.getPosition().y >= ExitGateBack.getPosition().y - 266.0f + 100.0f && ExitGateFore.getPosition().y <= ExitGateBack.getPosition().y - 266.0f + 150.0f) AddScoreEffect(SCORE_1000, ExitGateFore.getPosition().x, ExitGateFore.getPosition().y);
			else if (ExitGateFore.getPosition().y >= ExitGateBack.getPosition().y - 266.0f + 150.0f && ExitGateFore.getPosition().y <= ExitGateBack.getPosition().y - 266.0f + 200.0f) AddScoreEffect(SCORE_500, ExitGateFore.getPosition().x, ExitGateFore.getPosition().y);
			else if (ExitGateFore.getPosition().y > ExitGateBack.getPosition().y - 266.0f + 200.0f) AddScoreEffect(SCORE_200, ExitGateFore.getPosition().x, ExitGateFore.getPosition().y);

			ExitGateClock.start();
			LevelCompleteEffect = true;
			Music.StopAllMODMusic();
			Music.StopAllOGGMusic();
			Music.PlayOGGMusic("LevelComplete");
			ExitGateForeEffect.setPosition(ExitGateFore.getPosition());
			ExitGateForeEffectCurr = ExitGateForeEffectPrev = ExitGateFore.getPosition();

			ExitGateForeEffectSpeed = dis(seed) * M_PI / 180.0f;
			ExitGateForeActive = false;
			ExitGateForeRender = false;
		}
		//if (ExitGateForeY <= 14.0f) AddScoreEffect(SCORE_10000, ExitGateFore.getPosition().x, ExitGateFore.getPosition().y);
	}
	if (ExitGateForeActive) {
		if (!ExitGateState) {
			if (ExitGateForeY <= ExitGateForeYLimit) ExitGateForeY += ExitGateForeYSpeed * deltaTime;
			else {
				ExitGateState = true;
				ExitGateForeY = ExitGateForeYLimit;
			}
		}
		else {
			if (ExitGateForeY >= 0.0f) ExitGateForeY -= ExitGateForeYSpeed * deltaTime;
			else {
				ExitGateState = false;
				ExitGateForeY = 0.0f;
			}
		}
		ExitGateForeCurr = { ExitGateForeCurr.x, ExitGateBack.getPosition().y - 250.0f + ExitGateForeY };
	}
	else if (!ExitGateForeRender) {
		ExitGateForeEffectYSpeed += 0.5f * deltaTime * 0.2f;
		ExitGateForeEffectCurr = { ExitGateForeEffectCurr.x - sin(ExitGateForeEffectSpeed) * 5.0f * deltaTime, ExitGateForeEffectCurr.y + (cos(ExitGateForeEffectSpeed) * 5.0f + ExitGateForeEffectYSpeed) * deltaTime };
		//ExitGateForeEffect.move({ 0.0f - sin(ExitGateForeEffectSpeed) * 5.0f * deltaTime, (cos(ExitGateForeEffectSpeed) * 5.0f + ExitGateForeEffectYSpeed) * deltaTime });
		ExitGateForeEffectYSpeed += 0.5f * deltaTime * 0.2f;
		ExitGateForeEffect.rotate(sf::degrees(-25.0f * deltaTime));
	}
}
void ExitGateEffectReset() {
	ExitGateFore.setPosition({ ExitGateBack.getPosition().x + 43.0f, ExitGateBack.getPosition().y - 250.0f });
	ExitGateForeCurr = ExitGateForePrev = ExitGateFore.getPosition();
	ExitGateForeEffectYSpeed = 0.0f;
	ExitGateForeEffectSpeed = 0.0f;
	ExitGateForeRender = true;
}
void ExitGateDraw() {
	if (!isOutScreen(ExitGateIndicator.getPosition().x, ExitGateIndicator.getPosition().y, 64, 64)) {
		ExitGateIndicatorAnimation.update(ExitGateIndicator);
		rTexture.draw(ExitGateIndicator);
	}
	if (!isOutScreen(ExitGateBack.getPosition().x, ExitGateBack.getPosition().y, 64, 64)) rTexture.draw(ExitGateBack);
	if (!isOutScreen(ExitGateFore.getPosition().x, ExitGateFore.getPosition().y, 64, 64) && ExitGateForeRender) rTexture.draw(ExitGateFore);
}
void ExitGateEffectDraw() {
	if (!isOutScreen(ExitGateFore.getPosition().x, ExitGateFore.getPosition().y, 64, 64) && !ExitGateForeRender) rTexture.draw(ExitGateForeEffect);
}