#define USE_MATH_DEFINES
#include <cmath>
#include "Object/ExitGate.hpp"

#include "Core/AnimationSequenceManager.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Scroll.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Collision/Collide.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Core/MusicManager.hpp"
#include "Core/Time.hpp"
#include "Core/Utility.hpp"
#include "Core/Animate/StaticAnimationObject.hpp"
#include "Core/Enumeration/enum.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Effect/MarioEffect.hpp"
#include "Object/Mario.hpp"
#include "Object/Effect/Score10000Effect.hpp"
#include "Object/Effect/Score1000Effect.hpp"
#include "Object/Effect/Score100Effect.hpp"
#include "Object/Effect/Score2000Effect.hpp"
#include "Object/Effect/Score200Effect.hpp"
#include "Object/Effect/Score5000Effect.hpp"
#include "Object/Effect/Score500Effect.hpp"

static MFCPP::SingleAnimationObject ExitGateIndicatorAnimation;
static MFCPP::StaticAnimationObject ExitGateForeObject;
static MFCPP::StaticAnimationObject ExitGateBackObject;
static MFCPP::StaticAnimationObject ExitGateForeEffectObject;

MFCPP::ActiveObject<float> ExitGateForeEffect(sf::Vector2f(0.f, 0.f), sf::Vector2f(21.0f, 7.0f), sf::degrees(0.f));
MFCPP::ActiveObject<float> ExitGateIndicator(sf::Vector2f(0.f, 0.f), sf::Vector2f( 0.0f, 31.0f ), sf::degrees(0.f));
MFCPP::ActiveObject<float> ExitGateFore(sf::Vector2f(0.f, 0.f), sf::Vector2f(23.0f, 0.0f), sf::degrees(0.f));
MFCPP::ActiveObject<float> ExitGateBack(sf::Vector2f(0.f, 0.f), sf::Vector2f(0.0f, 287.0f), sf::degrees(0.f));
static sf::FloatRect m_hitbox_fore;
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

static constexpr int EXIT_INDICATOR_WIDTH = 31;
static constexpr int EXIT_INDICATOR_HEIGHT = 32;

void ExitGateInit() {
	ExitGateBackObject.setTexture("EXIT_GATE_BACK");

	ExitGateForeObject.setTexture("EXIT_GATE_FORE");
	m_hitbox_fore = sf::FloatRect({ 0.0f, 0.0f }, { 44.0f, 16.0f });

	ExitGateForeEffectObject.setTexture("EXIT_GATE_FORE_EFFECT", true);

	ExitGateIndicatorAnimation.setAnimation(0, 2, 50, true);
	ExitGateIndicatorAnimation.setAnimationSequence("EXIT_INDICATOR");
}
void SetPrevExitGatePos() {
	ExitGateFore.Update();
	ExitGateForeEffect.Update();
	ExitGateForeEffect.Update();
}
void ExitGateStatusUpdate(const float deltaTime) {
	if (ExitGateForeActive) {
		if (ExitGateIndicator.getCurrentPosition().x <= Mario::getCurrentPosition().x - 24.0f && !Mario::isPrejump() && !Mario::isFalling()) {
			GameScene::effectManager.addEffect<Score100Effect>(sf::Vector2f(Mario::getCurrentPosition().x, Mario::getCurrentPosition().y));
			LevelCompleteEffect = true;
			MusicManager::StopAllMusic();
			MusicManager::PlayMusic(MusicID::LEVEL_COMPLETE);
			ExitGateForeActive = false;
			//LevelEndMarioProjectileCleanup();
		}
		if (isCollide(getGlobalHitbox(Mario::getHitbox(), Mario::getCurrentPosition(), Mario::getOrigin()), getGlobalHitbox(m_hitbox_fore, ExitGateFore.getCurrentPosition(), ExitGateFore.getOrigin()))) {
			if (ExitGateFore.getCurrentPosition().y <= ExitGateBack.getCurrentPosition().y - 266.0f + 30.0f) GameScene::effectManager.addEffect<Score10000Effect>(sf::Vector2f(ExitGateFore.getCurrentPosition().x, ExitGateFore.getCurrentPosition().y));
			else if (ExitGateFore.getCurrentPosition().y >= ExitGateBack.getCurrentPosition().y - 266.0f + 30.0f && ExitGateFore.getCurrentPosition().y <= ExitGateBack.getCurrentPosition().y - 266.0f + 60.0f) GameScene::effectManager.addEffect<Score5000Effect>(sf::Vector2f(ExitGateFore.getCurrentPosition().x, ExitGateFore.getCurrentPosition().y));
			else if (ExitGateFore.getCurrentPosition().y >= ExitGateBack.getCurrentPosition().y - 266.0f + 60.0f && ExitGateFore.getCurrentPosition().y <= ExitGateBack.getCurrentPosition().y - 266.0f + 100.0f) GameScene::effectManager.addEffect<Score2000Effect>(sf::Vector2f(ExitGateFore.getCurrentPosition().x, ExitGateFore.getCurrentPosition().y));
			else if (ExitGateFore.getCurrentPosition().y >= ExitGateBack.getCurrentPosition().y - 266.0f + 100.0f && ExitGateFore.getCurrentPosition().y <= ExitGateBack.getCurrentPosition().y - 266.0f + 150.0f) GameScene::effectManager.addEffect<Score1000Effect>(sf::Vector2f(ExitGateFore.getCurrentPosition().x, ExitGateFore.getCurrentPosition().y));
			else if (ExitGateFore.getCurrentPosition().y >= ExitGateBack.getCurrentPosition().y - 266.0f + 150.0f && ExitGateFore.getCurrentPosition().y <= ExitGateBack.getCurrentPosition().y - 266.0f + 200.0f) GameScene::effectManager.addEffect<Score500Effect>(sf::Vector2f(ExitGateFore.getCurrentPosition().x, ExitGateFore.getCurrentPosition().y));
			else if (ExitGateFore.getCurrentPosition().y > ExitGateBack.getCurrentPosition().y - 266.0f + 200.0f) GameScene::effectManager.addEffect<Score200Effect>(sf::Vector2f(ExitGateFore.getCurrentPosition().x, ExitGateFore.getCurrentPosition().y));

			LevelCompleteEffect = true;
			MusicManager::StopAllMusic();
			MusicManager::PlayMusic(MusicID::LEVEL_COMPLETE);
			ExitGateForeEffect.teleport(ExitGateFore.getCurrentPosition());
			//LevelEndMarioProjectileCleanup();

			ExitGateForeEffectSpeed = Utility::RandomFloatNumberGenerator(123.75f, 146.25f) * M_PI / 180.0f;
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
		WindowFrame::running = false;
		WindowFrame::getWindow().close();

	}
}
void ExitGateEffectReset() {
	//reset behavior
	ExitGateFore.teleport(sf::Vector2f(ExitGateBack.getCurrentPosition().x + 43.0f, ExitGateBack.getCurrentPosition().y - 250.0f));

	ExitGateForeY = 0.f;
	ExitGateState = false;
	ExitGateForeEffectYSpeed = 0.0f;
	ExitGateForeEffectSpeed = 0.0f;
	ExitGateForeRender = true;
}
void ExitGateDraw(float alpha) {
	if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(ExitGateIndicator.getCurrentPosition(), ExitGateIndicator.getOrigin(), sf::FloatRect({0.f, 0.f}, {EXIT_INDICATOR_WIDTH, EXIT_INDICATOR_HEIGHT})), 0)) {
		ExitGateIndicatorAnimation.animationUpdate(ExitGateIndicator.getCurrentPosition(), ExitGateIndicator.getOrigin());
		ExitGateIndicatorAnimation.animationDraw();
	}
	if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(ExitGateBack.getCurrentPosition(), ExitGateBack.getOrigin(), sf::FloatRect({0.f, 0.f}, {95.f, 288.f})), 0)) {
		ExitGateBackObject.animationUpdate(ExitGateBack.getCurrentPosition(), ExitGateBack.getOrigin());
		ExitGateBackObject.animationDraw();
	}
	if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(ExitGateFore.getInterpolatedPosition(alpha), ExitGateFore.getOrigin(), m_hitbox_fore), 0) && ExitGateForeRender) {
		ExitGateForeObject.animationUpdate(ExitGateFore.getInterpolatedPosition(alpha), ExitGateFore.getOrigin());
		ExitGateForeObject.animationDraw();
	}
}
void ExitGateAnimationUpdate(float deltaTime) {
	ExitGateIndicatorAnimation.frameTimeAccumulate(deltaTime);
}
void ExitGateEffectDraw(float alpha) {
	if (!Scroll::isOutOfScreen(MFCPP::CollisionObject(ExitGateForeEffect.getInterpolatedPosition(alpha), ExitGateForeEffect.getOrigin(), sf::FloatRect({0.f, 0.f}, {44.f, 16.f})), 48) && !ExitGateForeRender) {
		ExitGateForeEffectObject.setRotation(ExitGateForeEffect.getInterpolatedAngle(alpha));
		ExitGateForeEffectObject.animationUpdate(ExitGateForeEffect.getInterpolatedPosition(alpha), ExitGateForeEffect.getOrigin());
		ExitGateForeEffectObject.animationDraw();
	}
}