#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../headers/Coin.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/Mario.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/AnimationManager.hpp"

#include <vector>
#include <iostream>
#include <utility>
std::vector<Coin> CoinList;
sf::Texture CoinTexture;
sf::SoundBuffer CoinSoundBuffer;
sf::Sound CoinSound;
AnimationManager CoinAnimation;
int CoinCount = 0;
bool firstUpdate = true;

int CoinInit() {
	if (!CoinTexture.loadFromFile("data/resources/Coin.png")) {
		std::cout << "Cannot load data/resources/Coin.png" << "\n";
	}
	CoinAnimation.addAnimation("IdleCoin", CoinTexture, { 3,0 }, { 32,32 }, { 0,0 }, 20);

	if (!CoinSoundBuffer.loadFromFile("data/sounds/coin.wav")) {
		std::cout << "Cannot load data/sounds/coin.wav" << "\n";
	}
	CoinSound.setBuffer(CoinSoundBuffer);
	return 6;
}
int CoinIni = CoinInit();
void addCoin(float x, float y) {
	Coin operate;
	operate.setHitbox({ 6, 2, 19, 28 });
	operate.property.setTexture(CoinTexture);
	operate.property.setPosition(x, y);
	CoinList.push_back(operate);
}
void DeleteCoin(float x, float y) {
	for (int i = 0; i < CoinList.size(); i++) {
		if (CoinList[i].property.getPosition().x == x && CoinList[i].property.getPosition().y == y) {
			CoinList.erase(CoinList.begin() + i);
			break;
		}
	}
}
void CoinOnTouch() {
	auto playerHitbox = player.getGlobalHitboxMain();
	for (int i = 0; i < CoinList.size(); i++) {
		if (CoinList[i].isCollide(playerHitbox)) {
			DeleteCoin(CoinList[i].property.getPosition().x, CoinList[i].property.getPosition().y);
			CoinSound.play();
			++CoinCount;
			break;
		}
	}
}
void CoinUpdate() {
	for (auto& i : CoinList) {
		if (!isOutScreen(i.property.getPosition().x, i.property.getPosition().y, 32, 32)) {
			CoinAnimation.update("IdleCoin", i.property);
			window.draw(i.property);
		}
	}
}