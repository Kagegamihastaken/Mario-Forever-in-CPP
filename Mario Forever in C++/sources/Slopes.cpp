#include <SFML/Graphics.hpp>
#include <vector>
#include <array>

#include "../headers/Slopes.hpp"
#include "../headers/Loading.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/Obstacles.hpp"

#include "../resource.h"
std::vector<Obstacles> SlopesList;
std::vector<int> SlopesGraphIDList;
sf::Texture SlopeTexture;
//ID_list[i][4] == 0: Left & ID_list[i][4] == 1: Right
std::vector<std::array<float, 2>> GraphList{
	{1.0f, 0.0f},
	{0.5f, 0.0f},
	{0.5f, 16.0f}
};
std::vector<std::array<int, 4>> IDSlope_list{
	{0, 0, 0, 0},
	{1, 32, 0, 1},
	{2, 64, 0, 2},
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
			window.draw(i.property);
		}
	}
}