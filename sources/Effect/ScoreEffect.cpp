#include "Core/Loading/enum.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Effect/Score10000Effect.hpp"
#include "Object/Effect/Score1000Effect.hpp"
#include "Object/Effect/Score100Effect.hpp"
#include "Object/Effect/Score1UPEffect.hpp"
#include "Object/Effect/Score2000Effect.hpp"
#include "Object/Effect/Score200Effect.hpp"
#include "Object/Effect/Score5000Effect.hpp"
#include "Object/Effect/Score500Effect.hpp"

void ScoreEffectInit() {
	ImageManager::AddTexture("Score_100", "data/resources/Score.png", sf::IntRect({ 0, 0 }, { 26, 16 }));
	ImageManager::AddTexture("Score_200", "data/resources/Score.png", sf::IntRect({ 0, 16 }, { 28, 16 }));
	ImageManager::AddTexture("Score_500", "data/resources/Score.png", sf::IntRect({ 0, 32 }, { 28, 16 }));
	ImageManager::AddTexture("Score_1000", "data/resources/Score.png", sf::IntRect({ 0, 48 }, { 34, 16 }));
	ImageManager::AddTexture("Score_2000", "data/resources/Score.png", sf::IntRect({ 0, 64 }, { 35, 16 }));
	ImageManager::AddTexture("Score_5000", "data/resources/Score.png", sf::IntRect({ 0, 80 }, { 36, 16 }));
	ImageManager::AddTexture("Score_10000", "data/resources/Score.png", sf::IntRect({ 0, 96 }, { 42, 16 }));
	ImageManager::AddTexture("Score_1UP", "data/resources/Score.png", sf::IntRect({ 0, 112 }, { 32, 16 }));
}
void AddScoreEffect(const ScoreID id, float x, float y) {
	switch (id) {
		case ScoreID::SCORE_100:
			GameScene::effectManager.addEffect<Score100Effect>(sf::Vector2f(x, y));
			break;
		case ScoreID::SCORE_200:
			GameScene::effectManager.addEffect<Score200Effect>(sf::Vector2f(x, y));
			break;
		case ScoreID::SCORE_500:
			GameScene::effectManager.addEffect<Score500Effect>(sf::Vector2f(x, y));
			break;
		case ScoreID::SCORE_1000:
			GameScene::effectManager.addEffect<Score1000Effect>(sf::Vector2f(x, y));
			break;
		case ScoreID::SCORE_2000:
			GameScene::effectManager.addEffect<Score2000Effect>(sf::Vector2f(x, y));
			break;
		case ScoreID::SCORE_5000:
			GameScene::effectManager.addEffect<Score5000Effect>(sf::Vector2f(x, y));
			break;
		case ScoreID::SCORE_10000:
			GameScene::effectManager.addEffect<Score10000Effect>(sf::Vector2f(x, y));
			break;
		case ScoreID::SCORE_1UP:
			GameScene::effectManager.addEffect<Score1UPEffect>(sf::Vector2f(x, y));
			break;
		default: ;
	}
}