#include "Core/Background/Bg.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Scroll.hpp"
#include "Core/Level.hpp"

std::vector<std::array<sf::Vertex, 4>> BgList;
std::vector<std::string> BgTextureNameList;
std::vector<float> BgParallaxList;
ImageManager BgTextureManager;
sf::VertexBuffer buffer;

void BgInit() {
	ImageManager::AddImage("GreenBackgroundImage", "data/resources/Background/GreenBackground.png");
	ImageManager::AddImage("GreenForeImage", "data/resources/Background/GreenFore.png");
	ImageManager::AddImage("GreenMidImage", "data/resources/Background/GreenMid.png");
	ImageManager::AddImage("GreenMid2Image", "data/resources/Background/GreenMid2.png");
	ImageManager::AddImage("GreenBackImage", "data/resources/Background/GreenBack.png");

	ImageManager::AddTexture("GreenBackgroundImage", "GreenBackground", true);
	ImageManager::AddTexture("GreenForeImage", "GreenFore", true);
	ImageManager::AddTexture("GreenMidImage", "GreenMid", true);
	ImageManager::AddTexture("GreenMid2Image", "GreenMid2", true);
	ImageManager::AddTexture("GreenBackImage", "GreenBack", true);

	buffer.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
	if (!buffer.create(4)) std::cerr << "Cannot create buffer\n";
}
void AddBg(const int type, const int parallax) {
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
	default: ;
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
		window.draw(buffer, &ImageManager::GetTexture(BgTextureNameList[i]));
	}
}