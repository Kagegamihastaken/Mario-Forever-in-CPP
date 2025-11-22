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
#include "Core/Time.hpp"
#include "Core/Loading/enum.hpp"
#include "Effect/MarioEffect.hpp"
#include "Projectiles/MarioProjectile.hpp"
#include "Object/Mario.hpp"

static MFCPP::SingleAnimationObject ExitGateIndicatorAnimation;
static MFCPP::StaticAnimationObject ExitGateForeObject;
static MFCPP::StaticAnimationObject ExitGateBackObject;
static MFCPP::StaticAnimationObject ExitGateForeEffectObject;

MFCPP::ActiveObject<float> ExitGateForeEffect;
MFCPP::ActiveObject<float> ExitGateIndicator;
MFCPP::ActiveObject<float> ExitGateFore;
MFCPP::ActiveObject<float> ExitGateBack;
float ExitGateClock;

static constexpr float ExitGateForeYLimit = 222.0f;
static constexpr float ExitGateForeYSpeed = 3.05f;
float ExitGateForeY = 0.0f;

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
	ExitGateBackObject.setTexture("ExitGateBack");
	ExitGateBack.setOrigin(sf::Vector2f(0.0f, 287.0f));

	ExitGateForeObject.setTexture("ExitGateFore");
	ExitGateFore.setOrigin(sf::Vector2f(23.0f, 0.0f));

	ExitGateForeEffectObject.setTexture("ExitGateForeEffect", true);
	ExitGateForeEffect.setOrigin({ 21.0f, 7.0f });

	ExitGateIndicatorAnimation.setAnimation(0, 2, 50);
	ExitGateIndicatorAnimation.setAnimationSequence(ExitIndicatorAnimName);
	ExitGateIndicator.setOrigin(sf::Vector2f( 0.0f, 31.0f ));
}
void SetPrevExitGatePos() {
	ExitGateFore.setPreviousPosition(ExitGateFore.getCurrentPosition());
	ExitGateForeEffect.setPreviousPosition(ExitGateForeEffect.getCurrentPosition());
	ExitGateForeEffect.setPreviousAngle(ExitGateForeEffect.getCurrentAngle());
}
void InterpolateExitGatePos(const float alpha) {
	ExitGateFore.setInterpolatedPosition(linearInterpolation(ExitGateFore.getPreviousPosition(), ExitGateFore.getCurrentPosition(), alpha));
	ExitGateForeEffect.setInterpolatedPosition(linearInterpolation(ExitGateForeEffect.getPreviousPosition(), ExitGateForeEffect.getCurrentPosition(), alpha));
	ExitGateForeEffect.setInterpolatedAngle(linearInterpolation(ExitGateForeEffect.getPreviousAngle(), ExitGateForeEffect.getCurrentAngle(), alpha));
}
void ExitGateStatusUpdate(const float deltaTime) {
	if (ExitGateForeActive) {
		if (ExitGateIndicator.getCurrentPosition().x <= player.curr.x - 24.0f && !PreJump && !MarioCurrentFalling) {
			AddScoreEffect(SCORE_100, player.curr.x, player.curr.y);
			LevelCompleteEffect = true;
			MusicManager::StopAllMusic();
			MusicManager::PlayMusic("LevelComplete");
			ExitGateForeActive = false;
			LevelEndMarioProjectileCleanup();
		}
		if (isCollide(getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin()), getGlobalHitbox(sf::FloatRect({ 0.0f, 0.0f }, { 44.0f, 16.0f }), ExitGateFore.getCurrentPosition(), ExitGateFore.getOrigin()))) {
			if (ExitGateFore.getCurrentPosition().y <= ExitGateBack.getCurrentPosition().y - 266.0f + 30.0f) AddScoreEffect(SCORE_10000, ExitGateFore.getCurrentPosition().x, ExitGateFore.getCurrentPosition().y);
			else if (ExitGateFore.getCurrentPosition().y >= ExitGateBack.getCurrentPosition().y - 266.0f + 30.0f && ExitGateFore.getCurrentPosition().y <= ExitGateBack.getCurrentPosition().y - 266.0f + 60.0f) AddScoreEffect(SCORE_5000, ExitGateFore.getCurrentPosition().x, ExitGateFore.getCurrentPosition().y);
			else if (ExitGateFore.getCurrentPosition().y >= ExitGateBack.getCurrentPosition().y - 266.0f + 60.0f && ExitGateFore.getCurrentPosition().y <= ExitGateBack.getCurrentPosition().y - 266.0f + 100.0f) AddScoreEffect(SCORE_2000, ExitGateFore.getCurrentPosition().x, ExitGateFore.getCurrentPosition().y);
			else if (ExitGateFore.getCurrentPosition().y >= ExitGateBack.getCurrentPosition().y - 266.0f + 100.0f && ExitGateFore.getCurrentPosition().y <= ExitGateBack.getCurrentPosition().y - 266.0f + 150.0f) AddScoreEffect(SCORE_1000, ExitGateFore.getCurrentPosition().x, ExitGateFore.getCurrentPosition().y);
			else if (ExitGateFore.getCurrentPosition().y >= ExitGateBack.getCurrentPosition().y - 266.0f + 150.0f && ExitGateFore.getCurrentPosition().y <= ExitGateBack.getCurrentPosition().y - 266.0f + 200.0f) AddScoreEffect(SCORE_500, ExitGateFore.getCurrentPosition().x, ExitGateFore.getCurrentPosition().y);
			else if (ExitGateFore.getCurrentPosition().y > ExitGateBack.getCurrentPosition().y - 266.0f + 200.0f) AddScoreEffect(SCORE_200, ExitGateFore.getCurrentPosition().x, ExitGateFore.getCurrentPosition().y);

			LevelCompleteEffect = true;
			MusicManager::StopAllMusic();
			MusicManager::PlayMusic("LevelComplete");
			ExitGateForeEffect.setCurrentPosition(ExitGateFore.getCurrentPosition());
			ExitGateForeEffect.setPreviousPosition(ExitGateForeEffect.getCurrentPosition());
			ExitGateForeEffect.setInterpolatedPosition(ExitGateForeEffect.getCurrentPosition());
			LevelEndMarioProjectileCleanup();

			ExitGateForeEffectSpeed = RandomFloatNumberGenerator(123.75f, 146.25f) * M_PI / 180.0f;
			ExitGateForeActive = false;
			ExitGateForeRender = false;
		}
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
		ExitGateFore.setCurrentPosition(sf::Vector2f(ExitGateFore.getCurrentPosition().x, ExitGateBack.getCurrentPosition().y - 250.0f + ExitGateForeY));
	}
	else if (!ExitGateForeRender) {
		ExitGateForeEffect.move(sf::Vector2f(- sin(ExitGateForeEffectSpeed) * 5.0f * deltaTime, (cos(ExitGateForeEffectSpeed) * 5.0f + ExitGateForeEffectYSpeed) * deltaTime));
		ExitGateForeEffectYSpeed += 1.f * deltaTime * 0.2f;
		ExitGateForeEffect.rotate(sf::degrees(-25.0f * deltaTime));
	}
}
void resetExitGateClock() {
	ExitGateClock = 0.f;
}
void ExitGateClockUpdate(const float deltaTime) {
	if (!LevelCompleteEffect || EffectActive) return;
	if (ExitGateClock <= 8.f * 50) {
		ExitGateClock += deltaTime;
	}
	else {
		if (getTime() > 0) TimeCounting(deltaTime);
		else ExitGateClock += deltaTime;
	}

	if (ExitGateClock >= 9.f * 50) {
		//win behavior, but implement later
		window.close();
	}
}
void ExitGateEffectReset() {
	//reset behavior
	ExitGateFore.setCurrentPosition(sf::Vector2f(ExitGateBack.getCurrentPosition().x + 43.0f, ExitGateBack.getCurrentPosition().y - 250.0f));
	ExitGateFore.setPreviousPosition(ExitGateFore.getCurrentPosition());
	ExitGateFore.setInterpolatedPosition(ExitGateFore.getCurrentPosition());

	ExitGateForeY = 0.f;
	ExitGateState = false;
	ExitGateForeEffectYSpeed = 0.0f;
	ExitGateForeEffectSpeed = 0.0f;
	ExitGateForeRender = true;
}
void ExitGateDraw() {
	if (!isOutScreen(ExitGateIndicator.getCurrentPosition().x, ExitGateIndicator.getCurrentPosition().y, 64, 64)) {
		ExitGateIndicatorAnimation.AnimationUpdate(ExitGateIndicator.getCurrentPosition(), ExitGateIndicator.getOrigin());
		ExitGateIndicatorAnimation.AnimationDraw();
	}
	if (!isOutScreen(ExitGateBack.getCurrentPosition().x, ExitGateBack.getCurrentPosition().y, 64, 64)) {
		ExitGateBackObject.AnimationUpdate(ExitGateBack.getCurrentPosition(), ExitGateBack.getOrigin());
		ExitGateBackObject.AnimationDraw();
	}
	if (!isOutScreen(ExitGateFore.getCurrentPosition().x, ExitGateFore.getCurrentPosition().y, 64, 64) && ExitGateForeRender) {
		ExitGateForeObject.AnimationUpdate(ExitGateFore.getInterpolatedPosition(), ExitGateFore.getOrigin());
		ExitGateForeObject.AnimationDraw();
	}
}
void ExitGateEffectDraw() {
	if (!isOutScreen(ExitGateFore.getCurrentPosition().x, ExitGateFore.getCurrentPosition().y, 64, 64) && !ExitGateForeRender) {
		ExitGateForeEffectObject.setRotation(ExitGateForeEffect.getInterpolatedAngle());
		ExitGateForeEffectObject.AnimationUpdate(ExitGateForeEffect.getInterpolatedPosition(), ExitGateForeEffect.getOrigin());
		ExitGateForeEffectObject.AnimationDraw();
	}
}