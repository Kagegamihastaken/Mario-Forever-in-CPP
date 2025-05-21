#include <SFML/Graphics.hpp>
#include <vector>

#include "Core/Loading/enum.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Core/WindowFrame.hpp"
#include "Object/Mario.hpp"
#include "Core/Sound.hpp"
#include "Core/TextureManager.hpp"
#include "Core/Interpolation.hpp"

//define
std::vector<sf::Sprite> ScoreEffectList;
std::vector<ScoreID> ScoreEffectIDList;
TextureManager ScoreEffectTexture;
std::vector<float> ScoreEffectVelocity;
std::vector<float> ScoreEffectAlpha;
std::vector<sf::Vector2f> ScoreEffectCurr;
std::vector<sf::Vector2f> ScoreEffectPrev;

void ScoreEffectInit() {
	ScoreEffectTexture.Loadingtexture("data/resources/Score.png", "SCORE_TEXTURE", 0, 0, 42, 128);
}
void SetPrevScoreEffectPos() {
	for (int i = 0; i < ScoreEffectList.size(); i++) {
		ScoreEffectPrev[i] = ScoreEffectCurr[i];
	}
}
void InterpolateScoreEffectPos(const float alpha) {
	for (int i = 0; i < ScoreEffectList.size(); i++) {
		ScoreEffectList[i].setPosition(linearInterpolation(ScoreEffectPrev[i], ScoreEffectCurr[i], alpha));
	}
}
void AddScoreEffect(ScoreID id, float x, float y) {
	sf::Sprite Init(*ScoreEffectTexture.GetTexture("SCORE_TEXTURE"));
	switch (id) {
	case SCORE_100:
		Init.setTextureRect(sf::IntRect({ 0, 0 }, { 26, 16 }));
		Score += 100;
		Init.setOrigin({ 12, 15 });
		break;
	case SCORE_200:
		Init.setTextureRect(sf::IntRect({ 0, 16 }, { 28, 16 }));
		Score += 200;
		Init.setOrigin({ 14, 15 });
		break;
	case SCORE_500:
		Init.setTextureRect(sf::IntRect({ 0, 32 }, { 28, 16 }));
		Score += 500;
		Init.setOrigin({ 13, 15 });
		break;
	case SCORE_1000:
		Init.setTextureRect(sf::IntRect({ 0, 48 }, { 34, 16 }));
		Score += 1000;
		Init.setOrigin({ 16, 15 });
		break;
	case SCORE_2000:
		Init.setTextureRect(sf::IntRect({ 0, 64 }, { 35, 16 }));
		Score += 2000;
		Init.setOrigin({ 16, 15 });
		break;
	case SCORE_5000:
		Init.setTextureRect(sf::IntRect({ 0, 80 }, { 36, 16 }));
		Score += 5000;
		Init.setOrigin({ 16, 15 });
		break;
	case SCORE_10000:
		Init.setTextureRect(sf::IntRect({ 0, 96 }, { 38, 16 }));
		Score += 10000;
		Init.setOrigin({ 20, 15 });
		break;
	case SCORE_1UP:
		Init.setTextureRect(sf::IntRect({ 0, 112 }, { 32, 16 }));
		Init.setOrigin({ 14, 15 });
		++Lives;
		Sounds.PlaySound("1UP");
		break;
	}
	Init.setPosition({ x, y });
	ScoreEffectCurr.push_back(Init.getPosition());
	ScoreEffectPrev.push_back(Init.getPosition());
	ScoreEffectList.push_back(Init);
	ScoreEffectIDList.push_back(id);
	ScoreEffectVelocity.push_back(-1.5f);
	ScoreEffectAlpha.push_back(255);
}
void DeleteScoreEffect(const int i, const float deltaTime) {
	if (ScoreEffectAlpha[i] > 0) {
		ScoreEffectAlpha[i] -= 7.5f * deltaTime;
		ScoreEffectList[i].setColor(sf::Color(255, 255, 255, std::max(0, static_cast<int>(ScoreEffectAlpha[i]))));
	}
	else {
		ScoreEffectList.erase(ScoreEffectList.begin() + i);
		ScoreEffectIDList.erase(ScoreEffectIDList.begin() + i);
		ScoreEffectVelocity.erase(ScoreEffectVelocity.begin() + i);
		ScoreEffectAlpha.erase(ScoreEffectAlpha.begin() + i);
		ScoreEffectCurr.erase(ScoreEffectCurr.begin() + i);
		ScoreEffectPrev.erase(ScoreEffectPrev.begin() + i);
	}
}
void DeleteAllScoreEffect() {
	ScoreEffectList.clear();
	ScoreEffectIDList.clear();
	ScoreEffectVelocity.clear();
	ScoreEffectAlpha.clear();
	ScoreEffectCurr.clear();
	ScoreEffectPrev.clear();
}
inline void ScoreEffectStatusUpdate(float deltaTime) {
	for (int i = 0; i < ScoreEffectList.size(); ++i) {
		ScoreEffectCurr[i] = { ScoreEffectCurr[i].x, ScoreEffectCurr[i].y + ScoreEffectVelocity[i] * deltaTime };
		if (ScoreEffectVelocity[i] < 0.0f) ScoreEffectVelocity[i] += 0.025f * deltaTime;
		else DeleteScoreEffect(i, deltaTime);
	}
}
inline void ScoreEffectUpdate() {
	if (ScoreEffectList.size() == 0) return;
	for (int i = 0; i < ScoreEffectList.size(); ++i) {
		rObject.draw(ScoreEffectList[i]);
	}
}