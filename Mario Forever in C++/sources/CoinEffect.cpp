#include "../headers/CoinEffect.hpp"
#include "../headers/AnimationManager.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/WindowFrame.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

std::vector<CoinEffect> CoinEffectList;
sf::Texture CoinEffectTexture;

int CoinEffectInit() {
	if (!CoinEffectTexture.loadFromFile("data/resources/CoinEffect.png")) {
		std::cout << "Cannot load data/resources/CoinEffect.png" << "\n";
	}
	return 6;
}
int iniCoinEffect = CoinEffectInit();
void AddCoinEffect(float x, float y) {
	CoinEffect Init;
	Init.coinEffectAnimation.setAnimation({ 37,32 }, { 0,0 }, { 21,0 }, 95);
	Init.property.setTexture(CoinEffectTexture);
	Init.property.setPosition(x, y);
	CoinEffectList.push_back(Init);
}
void DeleteCoinEffect(float x, float y) {
	for (int i = 0; i < CoinEffectList.size(); ++i) {
		if (CoinEffectList[i].property.getPosition().x == x && CoinEffectList[i].property.getPosition().y == y) {
			CoinEffectList.erase(CoinEffectList.begin() + i);
			break;
		}
	}
}
void CoinEffectStatusUpdate() {
	for (auto& i : CoinEffectList) {
		i.property.move(0.0f, i.velocity * deltaTime);
		if (i.velocity < 0.0f) i.velocity += 0.125f * deltaTime;
		else i.velocity = 0.0f;

		if (i.coinEffectAnimation.isAtTheEnd()) DeleteCoinEffect(i.property.getPosition().x, i.property.getPosition().y);
	}
}
void CoinEffectUpdate() {
	for (auto& i : CoinEffectList) {
		if (!isOutScreen(i.property.getPosition().x, i.property.getPosition().y, 32, 32)) {
			i.coinEffectAnimation.update(i.property, CoinEffectTexture);
			window.draw(i.property);
		}
		else i.coinEffectAnimation.silentupdate(i.property);
	}
}