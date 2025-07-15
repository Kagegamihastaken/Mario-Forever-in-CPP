#include "Core/Loading/enum.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Core/WindowFrame.hpp"
#include "Object/Mario.hpp"
#include "Core/Sound.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Class/ScoreEffectClass.hpp"
//define
plf::colony<MFCPP::ScoreEffect> ScoreEffectList;
static bool ScoreEffectDeleteGate = false;

void ScoreEffectInit() {
	ImageManager::AddTexture("Score_100", "data/resources/Score.png", sf::IntRect({ 0, 0 }, { 26, 16 }));
	ImageManager::AddTexture("Score_200", "data/resources/Score.png", sf::IntRect({ 0, 16 }, { 28, 16 }));
	ImageManager::AddTexture("Score_500", "data/resources/Score.png", sf::IntRect({ 0, 32 }, { 28, 16 }));
	ImageManager::AddTexture("Score_1000", "data/resources/Score.png", sf::IntRect({ 0, 48 }, { 34, 16 }));
	ImageManager::AddTexture("Score_2000", "data/resources/Score.png", sf::IntRect({ 0, 64 }, { 35, 16 }));
	ImageManager::AddTexture("Score_5000", "data/resources/Score.png", sf::IntRect({ 0, 80 }, { 36, 16 }));
	ImageManager::AddTexture("Score_10000", "data/resources/Score.png", sf::IntRect({ 0, 96 }, { 42, 16 }));
	ImageManager::AddTexture("Score_1UP", "data/resources/Score.png", sf::IntRect({ 0, 112 }, { 32, 16 }));
	//ScoreEffectTexture.Loadingtexture("data/resources/Score.png", "SCORE_TEXTURE", 0, 0, 42, 128);
}
void SetPrevScoreEffectPos() {
	for (auto &i : ScoreEffectList) {
		if (i.isDestroyed()) continue;

		i.setPreviousPosition(i.getCurrentPosition());
	}
}
void InterpolateScoreEffectPos(const float alpha) {
	for (auto &i : ScoreEffectList) {
		if (i.isDestroyed()) continue;

		i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
	}
}
void AddScoreEffect(const ScoreID id, float x, float y) {
	plf::colony<MFCPP::ScoreEffect>::colony_iterator<false> it;
	switch (id) {
		case SCORE_100:
			it = ScoreEffectList.emplace(sf::Vector2f(x, y), sf::Vector2f(12, 15));
			it->setTexture("Score_100");
			Score += 100;
			break;
		case SCORE_200:
			it = ScoreEffectList.emplace(sf::Vector2f(x, y), sf::Vector2f(14, 15));
			it->setTexture("Score_200");
			Score += 200;
			break;
		case SCORE_500:
			it = ScoreEffectList.emplace(sf::Vector2f(x, y), sf::Vector2f(13, 15));
			it->setTexture("Score_500");
			Score += 500;
			break;
		case SCORE_1000:
			it = ScoreEffectList.emplace(sf::Vector2f(x, y), sf::Vector2f(16, 15));
			it->setTexture("Score_1000");
			Score += 1000;
			break;
		case SCORE_2000:
			it = ScoreEffectList.emplace(sf::Vector2f(x, y), sf::Vector2f(16, 15));
			it->setTexture("Score_2000");
			Score += 2000;
			break;
		case SCORE_5000:
			it = ScoreEffectList.emplace(sf::Vector2f(x, y), sf::Vector2f(16, 15));
			it->setTexture("Score_5000");
			Score += 5000;
			break;
		case SCORE_10000:
			it = ScoreEffectList.emplace(sf::Vector2f(x, y), sf::Vector2f(20, 15));
			it->setTexture("Score_10000");
			Score += 10000;
			break;
		case SCORE_1UP:
			it = ScoreEffectList.emplace(sf::Vector2f(x, y), sf::Vector2f(14, 15));
			it->setTexture("Score_1UP");
			++Lives;
			SoundManager::PlaySound("1UP");
			break;
		default: ;
	}
}
void DeleteScoreEffect(plf::colony<MFCPP::ScoreEffect>::colony_iterator<false>& it) {
	ScoreEffectDeleteGate = true;
	it->setDestroyed(true);
}
void DeleteAllScoreEffect() {
	ScoreEffectList.clear();
}
void ScoreEffectStatusUpdate(const float deltaTime) {
	for (auto it = ScoreEffectList.begin(); it != ScoreEffectList.end(); ++it) {
		if (it->isDestroyed()) continue;

		it->move(sf::Vector2f(0.f, it->getVelocity() * deltaTime));
		if (it->getVelocity() < 0.0f) it->setVelocity(it->getVelocity() + 0.025f * deltaTime);
		else if (it->getVelocity() >= 0.0f) {
			it->setVelocity(0.f);
			if (it->getAlpha() > 0) {
				it->setAlpha(it->getAlpha() - 7.5f * deltaTime);
				it->setColor(sf::Color(255, 255, 255, std::max(0, static_cast<int>(it->getAlpha()))));
			}
			else
				DeleteScoreEffect(it);
		}
	}
}
void ScoreEffectDraw() {
	if (ScoreEffectList.empty()) return;
	for (auto & i : ScoreEffectList) {
		if (i.isDestroyed()) continue;

		i.AnimationUpdate(i.getInterpolatedPosition(), i.getOrigin());
		i.AnimationDraw(window);
	}
}
void ScoreEffectCleanup() {
	if (!ScoreEffectDeleteGate) return;
	auto it = ScoreEffectList.begin();
	while (it != ScoreEffectList.end()) {
		if (!it->isDestroyed()) ++it;
		else it = ScoreEffectList.erase(it);
	}
	ScoreEffectDeleteGate = false;
}