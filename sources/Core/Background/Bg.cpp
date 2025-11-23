#include "Core/Background/Bg.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Scroll.hpp"
#include "Core/Level.hpp"

std::vector<std::array<sf::Vertex, 4>> BgList;
std::vector<std::string> BgTextureNameList;
std::vector<sf::Vector2f> BgParallaxList;
sf::VertexBuffer buffer;

void BgInit() {
	ImageManager::PreloadTexture("GreenBackground", "data/resources/Background/GreenBackground.png", {}, true);
	ImageManager::PreloadTexture("GreenFore", "data/resources/Background/GreenFore.png", {}, true);
	ImageManager::PreloadTexture("GreenMid", "data/resources/Background/GreenMid.png", {}, true);
	ImageManager::PreloadTexture("GreenMid2", "data/resources/Background/GreenMid2.png", {}, true);
	ImageManager::PreloadTexture("GreenBack", "data/resources/Background/GreenBack.png", {}, true);
	ImageManager::PreloadTexture("Cave", "data/resources/Background/Cave.png", {}, true);

	buffer.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
	if (!buffer.create(4)) std::cerr << "Cannot create buffer\n";
}
void AddBg(const std::string& name, const sf::Vector2f& parallax) {
	BgTextureNameList.emplace_back(name);
	BgList.emplace_back();
	BgList.back()[0].texCoords = sf::Vector2f(0, 0);
	BgList.back()[1].texCoords = sf::Vector2f(LevelWidth, 0);
	BgList.back()[2].texCoords = sf::Vector2f(0, static_cast<float>(ImageManager::GetReturnTexture(name)->getSize().y));
	BgList.back()[3].texCoords = sf::Vector2f(LevelWidth, static_cast<float>(ImageManager::GetReturnTexture(name)->getSize().y));

	BgList.back()[0].position = sf::Vector2f(0, 0);
	BgList.back()[1].position = sf::Vector2f(LevelWidth, 0);
	BgList.back()[2].position = sf::Vector2f(0, static_cast<float>(ImageManager::GetReturnTexture(name)->getSize().y));
	BgList.back()[3].position = sf::Vector2f(LevelWidth, static_cast<float>(ImageManager::GetReturnTexture(name)->getSize().y));
	const sf::Vector2f parNormalize(parallax.x > 1.f ? 1.f : parallax.x, parallax.y > 1.f ? 1.f : parallax.y);
	BgParallaxList.push_back(parNormalize);
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
		BgList[i][0].position = sf::Vector2f(ViewX * (1.f - BgParallaxList[i].x), ViewY * (1.f - BgParallaxList[i].y));
		BgList[i][1].position = sf::Vector2f(ViewX * (1.f - BgParallaxList[i].x) + LevelWidth, ViewY * (1.f - BgParallaxList[i].y));
		BgList[i][2].position = sf::Vector2f(ViewX * (1.f - BgParallaxList[i].x), ViewY * (1.f - BgParallaxList[i].y) + static_cast<float>(ImageManager::GetReturnTexture(BgTextureNameList[i])->getSize().y));
		BgList[i][3].position = sf::Vector2f(ViewX * (1.f - BgParallaxList[i].x) + LevelWidth, ViewY * (1.f - BgParallaxList[i].y) + static_cast<float>(ImageManager::GetReturnTexture(BgTextureNameList[i])->getSize().y));
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