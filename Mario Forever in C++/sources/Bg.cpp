#include <SFML/Graphics.hpp>

#include "../headers/Core/Background/Bg.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Core/TextureManager.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Core/Level.hpp"

#include "../resource.h"

std::vector<std::array<sf::Vertex, 4>> BgList;
std::vector<std::string> BgTextureNameList;
std::vector<float> BgParallaxList;
TextureManager BgTextureManager;
sf::VertexBuffer buffer;

void BgInit() {
	BgTextureManager.Loadingtexture(GREEN_BACKGROUND_TEXTURE, "GreenBackground", 0, 0, 512, 480, true);
	BgTextureManager.Loadingtexture(GREEN_FORE_TEXTURE, "GreenFore", 0, 0, 512, 480, true);
	BgTextureManager.Loadingtexture(GREEN_MID_TEXTURE, "GreenMid", 0, 0, 410, 480, true);
	BgTextureManager.Loadingtexture(GREEN_MID2_TEXTURE, "GreenMid2", 0, 0, 282, 480, true);
	BgTextureManager.Loadingtexture(GREEN_BACK_TEXTURE, "GreenBack", 0, 0, 1411, 480, true);

	buffer.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
	buffer.create(4);
}
void AddBg(int type, int parallax) {
	switch (type) {
	case 0:
		BgTextureNameList.push_back("GreenBackground");
		break;
	case 1:
		BgTextureNameList.push_back("GreenFore");
		break;
	case 2:
		BgTextureNameList.push_back("GreenMid");
		break;
	case 3:
		BgTextureNameList.push_back("GreenMid2");
		break;
	case 4:
		BgTextureNameList.push_back("GreenBack");
		break;
	}
	BgList.push_back(std::array<sf::Vertex, 4>());
	BgList.back()[0].texCoords = sf::Vector2f(0, 0);
	BgList.back()[1].texCoords = sf::Vector2f(LevelWidth, 0);
	BgList.back()[2].texCoords = sf::Vector2f(0, LevelHeight);
	BgList.back()[3].texCoords = sf::Vector2f(LevelWidth, LevelHeight);

	BgList.back()[0].position = sf::Vector2f(0, 0);
	BgList.back()[1].position = sf::Vector2f(0 + LevelWidth, 0);
	BgList.back()[2].position = sf::Vector2f(0, 0 + LevelHeight);
	BgList.back()[3].position = sf::Vector2f(0 + LevelWidth, 0 + LevelHeight);
	BgParallaxList.push_back((parallax > 100 ? 100 : parallax) / 100.0f);
}
void BgUpdatePos() {
	for (int i = 0; i < BgList.size(); ++i) {
		//BgList[i][0].texCoords = sf::Vector2f(ViewX * (1 - BgParallaxList[i]) - (view.getSize().x - Width), ViewY * (1 - BgParallaxList[i]) - (view.getSize().y - Height));
		//BgList[i][1].texCoords = sf::Vector2f(ViewX * (1 - BgParallaxList[i]) + view.getSize().x, ViewY * (1 - BgParallaxList[i]) - (view.getSize().y - Height));
		//BgList[i][2].texCoords = sf::Vector2f(ViewX * (1 - BgParallaxList[i]) - (view.getSize().x - Width), ViewY * (1 - BgParallaxList[i]) + view.getSize().y);
		//BgList[i][3].texCoords = sf::Vector2f(ViewX * (1 - BgParallaxList[i]) + view.getSize().x, ViewY * (1 - BgParallaxList[i]) + view.getSize().y);

		//BgList[i][0].position = sf::Vector2f(ViewX - (view.getSize().x - Width), ViewY - (view.getSize().y - Height));
		//BgList[i][1].position = sf::Vector2f(ViewX + view.getSize().x, ViewY - (view.getSize().y - Height));
		//BgList[i][2].position = sf::Vector2f(ViewX - (view.getSize().x - Width), ViewY + view.getSize().y);
		//BgList[i][3].position = sf::Vector2f(ViewX + view.getSize().x, ViewY + view.getSize().y);

		if (BgParallaxList[i] == 0.0f) continue;
		BgList[i][0].position = sf::Vector2f(ViewX * BgParallaxList[i], ViewY * BgParallaxList[i]);
		BgList[i][1].position = sf::Vector2f(ViewX * BgParallaxList[i] + LevelWidth, ViewY * BgParallaxList[i]);
		BgList[i][2].position = sf::Vector2f(ViewX * BgParallaxList[i], ViewY * BgParallaxList[i] + LevelHeight);
		BgList[i][3].position = sf::Vector2f(ViewX * BgParallaxList[i] + LevelWidth, ViewY * BgParallaxList[i] + LevelHeight);
	}
}
void DeleteAllBg() {
	BgList.clear();
	BgTextureNameList.clear();
	BgParallaxList.clear();
}
void BgDraw() {
	for (int i = 0; i < BgList.size(); ++i) {
		buffer.update(BgList[i].data());
		window.draw(buffer, BgTextureManager.GetTexture(BgTextureNameList[i]));
	}
}