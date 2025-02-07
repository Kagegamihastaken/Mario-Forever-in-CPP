#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

#include "../headers/Core/Loading/enum.hpp"
#include "../headers/Effect/ScoreEffect.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Object/Mario.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Sound.hpp"

#include "../resource.h"

//define
std::vector<sf::Sprite> ScoreEffectList;
std::vector<ScoreID> ScoreEffectIDList;
sf::Texture ScoreEffectTexture;
std::vector<float> ScoreEffectVelocity;
std::vector<float> ScoreEffectAlpha;

int ScoreEffectInit() {
	LoadTexture(ScoreEffectTexture, SCOREEFFECT_TEXTURE);
	return 6;
}
int IniScore = ScoreEffectInit();
void AddScoreEffect(ScoreID id, float x, float y) {
	sf::Sprite Init;
	Init.setTexture(ScoreEffectTexture);
	if (id == SCORE_100) {
		Score += 100;
		Init.setTextureRect(sf::IntRect(0, 0, 26, 16));
	}
	else if (id == SCORE_200) {
		Score += 200;
		Init.setTextureRect(sf::IntRect(0, 16, 28, 16));
	}
	else if (id == SCORE_500) {
		Score += 500;
		Init.setTextureRect(sf::IntRect(0, 32, 28, 16));
	}
	else if (id == SCORE_1000) {
		Score += 1000;
		Init.setTextureRect(sf::IntRect(0, 48, 34, 16));
	}
	else if (id == SCORE_2000) {
		Score += 2000;
		Init.setTextureRect(sf::IntRect(0, 64, 35, 16));
	}
	else if (id == SCORE_5000) {
		Score += 5000;
		Init.setTextureRect(sf::IntRect(0, 80, 34, 16));
	}
	else if (id == SCORE_10000) {
		Score += 10000;
		Init.setTextureRect(sf::IntRect(0, 96, 42, 16));
	}
	else if (id == SCORE_1UP) {
		Init.setTextureRect(sf::IntRect(0, 112, 32, 16));
		++Lives;
		Sounds.PlaySound("1UP");
	}
	Init.setPosition(static_cast<int>(round(x)), y);
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
		ScoreEffectList[i].move(0, ScoreEffectVelocity[i] * deltaTime);
		if (ScoreEffectVelocity[i] < 0.0f) ScoreEffectVelocity[i] += 0.025f * deltaTime;
		else DeleteScoreEffect(ScoreEffectList[i].getPosition().x, ScoreEffectList[i].getPosition().y);
	}
}
inline void ScoreEffectUpdate() {
	for (int i = 0; i < ScoreEffectList.size(); ++i) {
		window.draw(ScoreEffectList[i]);
	}
}