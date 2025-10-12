#define USE_MATH_DEFINES
#include <cmath>
#include "Object/ExitGate.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Collision/Collide.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/Interpolation.hpp"
#include "Core/MusicManager.hpp"
#include "Core/Loading/enum.hpp"
#include "Projectiles/MarioProjectile.hpp"
#include "Object/Mario.hpp"

SingleAnimationObject ExitGateIndicatorAnimation;
sf::Sprite ExitGateBack(tempTex);
sf::Sprite ExitGateFore(tempTex);
sf::Sprite ExitGateForeEffect(tempTex);
sf::Sprite ExitGateIndicator(tempTex);
sf::Clock ExitGateClock;

sf::Vector2f ExitGateForeCurr = sf::Vector2f(0.0f, 0.0f);
sf::Vector2f ExitGateForePrev = sf::Vector2f(0.0f, 0.0f);
sf::Vector2f ExitGateForeEffectCurr = sf::Vector2f(0.0f, 0.0f);
sf::Vector2f ExitGateForeEffectPrev = sf::Vector2f(0.0f, 0.0f);

constexpr float ExitGateForeYLimit = 222.0f;
float ExitGateForeY = 0.0f;
constexpr float ExitGateForeYSpeed = 3.05f;
bool ExitGateState = false;
bool ExitGateForeActive = true;
bool LevelCompleteEffect = false;
float ExitGateForeEffectSpeed = 0.0f;
float ExitGateForeEffectYSpeed = 0.0f;
bool ExitGateForeRender = true;
static std::vector<std::string> ExitIndicatorAnimName;
static constexpr int EXIT_INDICATOR_IMAGE_WIDTH = 93;
static constexpr int EXIT_INDICATOR_WIDTH = 31;
static constexpr int EXIT_INDICATOR_HEIGHT = 32;

void ExitGateInit() {
	ImageManager::AddTexture("ExitGateBack", "data/resources/ExitGateBack.png");
	ImageManager::AddTexture("ExitGateFore", "data/resources/ExitGateFore.png");
	ImageManager::AddTexture("ExitGateForeEffect", "data/resources/ExitGateForeEffect.png");
	for (int i = 0; i < EXIT_INDICATOR_IMAGE_WIDTH / EXIT_INDICATOR_WIDTH; i++) {
		ImageManager::AddTexture(fmt::format("ExitGateIndicator_{}", i), "data/resources/ExitGateIndicator.png", sf::IntRect({i * EXIT_INDICATOR_WIDTH, 0}, {EXIT_INDICATOR_WIDTH, EXIT_INDICATOR_HEIGHT}));
		ExitIndicatorAnimName.push_back(fmt::format("ExitGateIndicator_{}", i));
	}
	//ExitGateTextureManager.LoadingAnimatedTexture(EXIT_GATE_INDICATOR_TEXTURE, "ExitGateIndicator", 0, 2, 0, 31, 32);
	ExitGateBack.setTexture(ImageManager::GetTexture("ExitGateBack"), true);
	ExitGateBack.setOrigin({ 0.0f, 287.0f });

	ExitGateFore.setTexture(ImageManager::GetTexture("ExitGateFore"), true);
	ExitGateFore.setOrigin({ 23.0f, 0.0f });

	ExitGateForeEffect.setTexture(ImageManager::GetTexture("ExitGateForeEffect"), true);
	ExitGateForeEffect.setOrigin({ 21.0f, 7.0f });
	ExitGateClock.reset();

	ExitGateIndicatorAnimation.setAnimation(0, 2, 50);
	ExitGateIndicatorAnimation.setAnimationSequence(ExitIndicatorAnimName);
	ExitGateIndicator.setOrigin({ 0.0f, 31.0f });
}
void SetPrevExitGatePos() {
	ExitGateForePrev = ExitGateForeCurr;
	ExitGateForeEffectPrev = ExitGateForeEffectCurr;
}
void InterpolateExitGatePos(const float alpha) {
	ExitGateFore.setPosition(linearInterpolation(ExitGateForePrev, ExitGateForeCurr, alpha));
	ExitGateForeEffect.setPosition(linearInterpolation(ExitGateForeEffectPrev, ExitGateForeEffectCurr, alpha));
}
void ExitGateStatusUpdate(const float deltaTime) {
	if (ExitGateForeActive) {
		if (ExitGateIndicator.getPosition().x <= player.curr.x - 24.0f && !PreJump && !MarioCurrentFalling) {
			AddScoreEffect(SCORE_100, player.curr.x, player.curr.y);
			ExitGateClock.start();
			LevelCompleteEffect = true;
			MusicManager::StopAllMusic();
			MusicManager::PlayMusic("LevelComplete");
			ExitGateForeActive = false;
			LevelEndMarioProjectileCleanup();
		}
		if (isCollide(getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin()), getGlobalHitbox(sf::FloatRect({ 0.0f, 0.0f }, { 44.0f, 16.0f }), ExitGateFore))) {
			if (ExitGateFore.getPosition().y <= ExitGateBack.getPosition().y - 266.0f + 30.0f) AddScoreEffect(SCORE_10000, ExitGateFore.getPosition().x, ExitGateFore.getPosition().y);
			else if (ExitGateFore.getPosition().y >= ExitGateBack.getPosition().y - 266.0f + 30.0f && ExitGateFore.getPosition().y <= ExitGateBack.getPosition().y - 266.0f + 60.0f) AddScoreEffect(SCORE_5000, ExitGateFore.getPosition().x, ExitGateFore.getPosition().y);
			else if (ExitGateFore.getPosition().y >= ExitGateBack.getPosition().y - 266.0f + 60.0f && ExitGateFore.getPosition().y <= ExitGateBack.getPosition().y - 266.0f + 100.0f) AddScoreEffect(SCORE_2000, ExitGateFore.getPosition().x, ExitGateFore.getPosition().y);
			else if (ExitGateFore.getPosition().y >= ExitGateBack.getPosition().y - 266.0f + 100.0f && ExitGateFore.getPosition().y <= ExitGateBack.getPosition().y - 266.0f + 150.0f) AddScoreEffect(SCORE_1000, ExitGateFore.getPosition().x, ExitGateFore.getPosition().y);
			else if (ExitGateFore.getPosition().y >= ExitGateBack.getPosition().y - 266.0f + 150.0f && ExitGateFore.getPosition().y <= ExitGateBack.getPosition().y - 266.0f + 200.0f) AddScoreEffect(SCORE_500, ExitGateFore.getPosition().x, ExitGateFore.getPosition().y);
			else if (ExitGateFore.getPosition().y > ExitGateBack.getPosition().y - 266.0f + 200.0f) AddScoreEffect(SCORE_200, ExitGateFore.getPosition().x, ExitGateFore.getPosition().y);

			ExitGateClock.start();
			LevelCompleteEffect = true;
			MusicManager::StopAllMusic();
			MusicManager::PlayMusic("LevelComplete");
			ExitGateForeEffect.setPosition(ExitGateFore.getPosition());
			ExitGateForeEffectCurr = ExitGateForeEffectPrev = ExitGateFore.getPosition();
			LevelEndMarioProjectileCleanup();

			ExitGateForeEffectSpeed = RandomFloatNumberGenerator(123.75f, 146.25f) * M_PI / 180.0f;
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
		ExitGateForeEffectCurr = { ExitGateForeEffectCurr.x - sin(ExitGateForeEffectSpeed) * 5.0f * deltaTime, ExitGateForeEffectCurr.y + (cos(ExitGateForeEffectSpeed) * 5.0f + ExitGateForeEffectYSpeed) * deltaTime };
		//ExitGateForeEffect.move({ 0.0f - sin(ExitGateForeEffectSpeed) * 5.0f * deltaTime, (cos(ExitGateForeEffectSpeed) * 5.0f + ExitGateForeEffectYSpeed) * deltaTime });
		ExitGateForeEffectYSpeed += 1.f * deltaTime * 0.2f;
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
		ExitGateIndicatorAnimation.AnimationUpdate(ExitGateIndicator.getPosition(), ExitGateIndicator.getOrigin());
		ExitGateIndicatorAnimation.AnimationDraw();
		//window.draw(ExitGateIndicator);
	}
	if (!isOutScreen(ExitGateBack.getPosition().x, ExitGateBack.getPosition().y, 64, 64)) window.draw(ExitGateBack);
	if (!isOutScreen(ExitGateFore.getPosition().x, ExitGateFore.getPosition().y, 64, 64) && ExitGateForeRender) window.draw(ExitGateFore);
}
void ExitGateEffectDraw() {
	if (!isOutScreen(ExitGateFore.getPosition().x, ExitGateFore.getPosition().y, 64, 64) && !ExitGateForeRender) window.draw(ExitGateForeEffect);
}