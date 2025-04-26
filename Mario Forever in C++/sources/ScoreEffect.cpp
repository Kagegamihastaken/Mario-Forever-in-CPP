#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "../headers/Core/Loading/enum.hpp"
#include "../headers/Effect/ScoreEffect.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Object/Mario.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Sound.hpp"
#include "../headers/Core/TextureManager.hpp"

#include "../resource.h"

//define
std::vector<sf::Sprite> ScoreEffectList;
std::vector<ScoreID> ScoreEffectIDList;
TextureManager ScoreEffectTexture;
std::vector<float> ScoreEffectVelocity;
std::vector<float> ScoreEffectAlpha;

void ScoreEffectInit() {
	ScoreEffectTexture.Loadingtexture(SCOREEFFECT_TEXTURE, "SCORE_100", 0, 0, 26, 16);
	ScoreEffectTexture.Loadingtexture(SCOREEFFECT_TEXTURE, "SCORE_200", 0, 16, 28, 16);
	ScoreEffectTexture.Loadingtexture(SCOREEFFECT_TEXTURE, "SCORE_500", 0, 32, 28, 16);
	ScoreEffectTexture.Loadingtexture(SCOREEFFECT_TEXTURE, "SCORE_1000", 0, 48, 34, 16);
	ScoreEffectTexture.Loadingtexture(SCOREEFFECT_TEXTURE, "SCORE_2000", 0, 64, 35, 16);
	ScoreEffectTexture.Loadingtexture(SCOREEFFECT_TEXTURE, "SCORE_5000", 0, 80, 36, 16);
	ScoreEffectTexture.Loadingtexture(SCOREEFFECT_TEXTURE, "SCORE_10000", 0, 96, 38, 16);
	ScoreEffectTexture.Loadingtexture(SCOREEFFECT_TEXTURE, "SCORE_1UP", 0, 112, 32, 16);
}
void AddScoreEffect(ScoreID id, float x, float y) {
	sf::Sprite Init(tempTex);
	switch (id) {
	case SCORE_100:
		Init.setTexture(*ScoreEffectTexture.GetTexture("SCORE_100"), true);
		Score += 100;
		Init.setOrigin({ 12, 15 });
		break;
	case SCORE_200:
		Init.setTexture(*ScoreEffectTexture.GetTexture("SCORE_200"), true);
		Score += 200;
		Init.setOrigin({ 14, 15 });
		break;
	case SCORE_500:
		Init.setTexture(*ScoreEffectTexture.GetTexture("SCORE_500"), true);
		Score += 500;
		Init.setOrigin({ 13, 15 });
		break;
	case SCORE_1000:
		Init.setTexture(*ScoreEffectTexture.GetTexture("SCORE_1000"), true);
		Score += 1000;
		Init.setOrigin({ 16, 15 });
		break;
	case SCORE_2000:
		Init.setTexture(*ScoreEffectTexture.GetTexture("SCORE_2000"), true);
		Score += 2000;
		Init.setOrigin({ 16, 15 });
		break;
	case SCORE_5000:
		Init.setTexture(*ScoreEffectTexture.GetTexture("SCORE_5000"), true);
		Score += 5000;
		Init.setOrigin({ 16, 15 });
		break;
	case SCORE_10000:
		Init.setTexture(*ScoreEffectTexture.GetTexture("SCORE_10000"), true);
		Score += 10000;
		Init.setOrigin({ 20, 15 });
		break;
	case SCORE_1UP:
		Init.setTexture(*ScoreEffectTexture.GetTexture("SCORE_1UP"), true);
		Init.setOrigin({ 14, 15 });
		++Lives;
		Sounds.PlaySound("1UP");
		break;
	}
	Init.setPosition({ x, y });
	ScoreEffectList.push_back(Init);
	ScoreEffectIDList.push_back(id);
	ScoreEffectVelocity.push_back(-1.5f);
	ScoreEffectAlpha.push_back(255);
}
void DeleteScoreEffect(float x, float y) {
	for (int i = 0; i < ScoreEffectList.size(); ++i) {
		if (ScoreEffectList[i].getPosition().x == x && ScoreEffectList[i].getPosition().y == y) {
			if (ScoreEffectAlpha[i] > 0) {
				ScoreEffectAlpha[i] -= 7.5f * deltaTime;
				ScoreEffectList[i].setColor(sf::Color(255, 255, 255, std::max(0, static_cast<int>(ScoreEffectAlpha[i]))));
			}
			else {
				ScoreEffectList.erase(ScoreEffectList.begin() + i);
				ScoreEffectIDList.erase(ScoreEffectIDList.begin() + i);
				ScoreEffectVelocity.erase(ScoreEffectVelocity.begin() + i);
				ScoreEffectAlpha.erase(ScoreEffectAlpha.begin() + i);
			}
		}
	}
}
void DeleteAllScoreEffect() {
	ScoreEffectList.clear();
	ScoreEffectIDList.clear();
	ScoreEffectVelocity.clear();
	ScoreEffectAlpha.clear();
}
inline void ScoreEffectStatusUpdate() {
	if (ScoreEffectList.size() == 0) return;
	for (int i = 0; i < ScoreEffectList.size(); ++i) {
		ScoreEffectList[i].move({ 0, ScoreEffectVelocity[i] * deltaTime });
		if (ScoreEffectVelocity[i] < 0.0f) ScoreEffectVelocity[i] += 0.025f * deltaTime;
		else DeleteScoreEffect(ScoreEffectList[i].getPosition().x, ScoreEffectList[i].getPosition().y);
	}
}
inline void ScoreEffectUpdate() {
	if (ScoreEffectList.size() == 0) return;
	for (int i = 0; i < ScoreEffectList.size(); ++i) {
		window.draw(ScoreEffectList[i]);
	}
}