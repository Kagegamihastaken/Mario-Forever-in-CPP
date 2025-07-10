#include "Block/Slopes.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/Scroll.hpp"
#include "Core/WindowFrame.hpp"
#include "Block/Obstacles.hpp"
#include "Core/Tilemap.hpp"
std::vector<Obstacles> SlopesList;
std::vector<int> SlopesIDList;
sf::Texture SlopeTexture;
//ID_list[i][4] == 0: Left & ID_list[i][4] == 1: Right
std::vector<std::array<float, 2>> GraphList{
	{1.0f, 0.0f},
	{0.5f, 0.0f},
	{0.5f, 16.0f}
};
std::vector<std::array<int, 3>> IDSlope_list{
	{0, 0, 0},
	{1, 32, 0},
	{2, 64, 0},
};

void loadSlopeRes() {
	LoadTexture(SlopeTexture, "data/resources/Slope.png");
}
void AddSlope(const float x,const float y, const int id) {
	switch (id) {
		case 0:
			SlopesList.emplace_back(0, sf::Sprite(SlopeTexture, sf::IntRect({0, 0}, {32, 32})));
			SlopesList.back().property.setPosition(sf::Vector2f(x, y));
			MFCPP::setIndexTilemapCollision(x, y, true);
			MFCPP::setIndexTilemapID(x, y, 3);
			MFCPP::setIndexTilemapFloorY(x, y, {0.f, 32.f});
			break;
		case 1:
			SlopesList.emplace_back(0, sf::Sprite(SlopeTexture, sf::IntRect({64, 0}, {32, 32})));
			SlopesList.back().property.setPosition(sf::Vector2f(x, y));
			MFCPP::setIndexTilemapCollision(x, y, true);
			MFCPP::setIndexTilemapID(x, y, 3);
			MFCPP::setIndexTilemapFloorY(x, y, {0.f, 16.f});
			break;
		case 2:
			SlopesList.emplace_back(0, sf::Sprite(SlopeTexture, sf::IntRect({32, 0}, {32, 32})));
			SlopesList.back().property.setPosition(sf::Vector2f(x, y));
			MFCPP::setIndexTilemapCollision(x, y, true);
			MFCPP::setIndexTilemapID(x, y, 3);
			MFCPP::setIndexTilemapFloorY(x, y, {16.f, 32.f});
			break;
		case 3:
			SlopesList.emplace_back(0, sf::Sprite(SlopeTexture, sf::IntRect({96, 0}, {32, 32})));
			SlopesList.back().property.setPosition(sf::Vector2f(x, y));
			MFCPP::setIndexTilemapCollision(x, y, true);
			MFCPP::setIndexTilemapID(x, y, 3);
			MFCPP::setIndexTilemapFloorY(x, y, {32.f, 0.f});
			break;
		default: ;
	}
}
int getSlopesIndex(const float x, const float y) {
	for (int i = 0; i < SlopesList.size(); ++i) {
		if (SlopesList[i].property.getPosition().x == x && SlopesList[i].property.getPosition().y == y) {
			return i;
		}
	}
	return -1;
}
void SlopeDraw() {
	for (const auto& i : SlopesList) {
		if (!isOutScreen(i.property.getPosition().x, i.property.getPosition().y, 32, 32)) {
			window.draw(i.property);
		}
	}
}