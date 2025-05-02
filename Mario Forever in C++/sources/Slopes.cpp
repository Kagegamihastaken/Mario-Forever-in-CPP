#include <SFML/Graphics.hpp>
#include <vector>
#include <array>

#include "../headers/Block/Slopes.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Block/Obstacles.hpp"

#include "../resource.h"
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
std::array<std::array<sf::FloatRect, 32>, 3> SlopeRectList{
	{
		sf::FloatRect({-1.0f, 31.0f}, {1.0f, 1.0f}),
		sf::FloatRect({0.0f, 30.0f}, {1.0f, 2.0f}),
		sf::FloatRect({1.0f, 29.0f}, {1.0f, 3.0f}),
		sf::FloatRect({2.0f, 28.0f}, {1.0f, 4.0f}),
		sf::FloatRect({3.0f, 27.0f}, {1.0f, 5.0f}),
		sf::FloatRect({4.0f, 26.0f}, {1.0f, 6.0f}),
		sf::FloatRect({5.0f, 25.0f}, {1.0f, 7.0f}),
		sf::FloatRect({6.0f, 24.0f}, {1.0f, 8.0f}),
		sf::FloatRect({7.0f, 23.0f}, {1.0f, 9.0f}),
		sf::FloatRect({8.0f, 22.0f}, {1.0f, 10.0f}),
		sf::FloatRect({9.0f, 21.0f}, {1.0f, 11.0f}),
		sf::FloatRect({10.0f, 20.0f}, {1.0f, 12.0f}),
		sf::FloatRect({11.0f, 19.0f}, {1.0f, 13.0f}),
		sf::FloatRect({12.0f, 18.0f}, {1.0f, 14.0f}),
		sf::FloatRect({13.0f, 17.0f}, {1.0f, 15.0f}),
		sf::FloatRect({14.0f, 16.0f}, {1.0f, 16.0f}),
		sf::FloatRect({15.0f, 15.0f}, {1.0f, 17.0f}),
		sf::FloatRect({16.0f, 14.0f}, {1.0f, 18.0f}),
		sf::FloatRect({17.0f, 13.0f}, {1.0f, 19.0f}),
		sf::FloatRect({18.0f, 12.0f}, {1.0f, 20.0f}),
		sf::FloatRect({19.0f, 11.0f}, {1.0f, 21.0f}),
		sf::FloatRect({20.0f, 10.0f}, {1.0f, 22.0f}),
		sf::FloatRect({21.0f, 9.0f}, {1.0f, 23.0f}),
		sf::FloatRect({22.0f, 8.0f}, {1.0f, 24.0f}),
		sf::FloatRect({23.0f, 7.0f}, {1.0f, 25.0f}),
		sf::FloatRect({24.0f, 6.0f}, {1.0f, 26.0f}),
		sf::FloatRect({25.0f, 5.0f}, {1.0f, 27.0f}),
		sf::FloatRect({26.0f, 4.0f}, {1.0f, 28.0f}),
		sf::FloatRect({27.0f, 3.0f}, {1.0f, 29.0f}),
		sf::FloatRect({28.0f, 2.0f}, {1.0f, 30.0f}),
		sf::FloatRect({29.0f, 1.0f}, {1.0f, 31.0f}),
		sf::FloatRect({30.0f, 0.0f}, {1.0f, 32.0f}),
		sf::FloatRect({31.0f, -1.0f}, {1.0f, 33.0f}),
		sf::FloatRect({32.0f, -2.0f}, {1.0f, 34.0f})
	}
};

int loadSlopeRes() {
	LoadTexture(SlopeTexture, SLOPE_TEXTURE);
	return 6;
}
const int ObstacleInit = loadSlopeRes();
int getSlopesIndex(float x, float y) {
	for (int i = 0; i < SlopesList.size(); ++i) {
		if (SlopesList[i].property.getPosition().x == x && SlopesList[i].property.getPosition().y == y) {
			return i;
		}
	}
	return -1;
}
void SlopeUpdate() {
	for (const auto& i : SlopesList) {
		if (!isOutScreen(i.property.getPosition().x, i.property.getPosition().y, 32, 32)) {
			rTexture.draw(i.property);
		}
	}
}