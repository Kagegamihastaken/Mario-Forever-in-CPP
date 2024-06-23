#include "../headers/CoinEffect.hpp"
#include "../headers/AnimationManager.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/WindowFrame.hpp"
#include <SFML/Graphics.hpp>
#include <vector>

std::vector<sf::Sprite> CoinEffectList;
sf::Texture CoinEffectTexture;
AnimationManager CoinEffectAnimation;

int CoinEffectInit() {
	if (!CoinEffectTexture.loadFromFile("data/resources/CoinEffect.png")) {
		std::cout << "Cannot load data/resources/CoinEffect.png" << "\n";
	}
	CoinEffectAnimation.addAnimation("IdleEffect", CoinEffectTexture, { 21,0 }, { 37,32 }, { 0,0 }, 70);
	return 6;
}
int iniCoinEffect = CoinEffectInit();
void CoinEffectUpdate() {
	for (auto& i : CoinEffectList) {
		if (!isOutScreen(i.getPosition().x, i.getPosition().y, 32, 32)) {
			CoinEffectAnimation.update("IdleEffect", i);
			window.draw(i);
		}
	}
}
void AddCoinEffect(float x, float y) {
	sf::Sprite Init;
	Init.setTexture(CoinEffectTexture);
	Init.setPosition(x, y);
	CoinEffectList.push_back(Init);
}
void DeleteCoinEffect(float x, float y) {
	for (int i = 0; i < CoinEffectList.size(); i++) {
		if (CoinEffectList[i].getPosition().x == x && CoinEffectList[i].getPosition().y == y) {
			CoinEffectList.erase(CoinEffectList.begin() + i);
			break;
		}
	}
}