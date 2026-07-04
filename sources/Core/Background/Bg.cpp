#include "Core/Background/Bg.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Scroll.hpp"
#include "Core/Level.hpp"
#include "Core/Profiler.hpp"
struct BgLayer {
	sf::VertexArray vertices;
	sf::Texture* texture;
	sf::Vector2f parallax;
};
std::vector<BgLayer> BgList;

void BgInit() {
	ImageManager::PreloadTexture("BlueBackground", "data/resources/Background/BlueBackground.png", {}, true);
	ImageManager::PreloadTexture("BlueForeground", "data/resources/Background/BlueForeground.png", {}, true);
	ImageManager::PreloadTexture("GreenBackground", "data/resources/Background/GreenBackground.png", {}, true);
	ImageManager::PreloadTexture("GreenFore", "data/resources/Background/GreenFore.png", {}, true);
	ImageManager::PreloadTexture("GreenMid", "data/resources/Background/GreenMid.png", {}, true);
	ImageManager::PreloadTexture("GreenMid2", "data/resources/Background/GreenMid2.png", {}, true);
	ImageManager::PreloadTexture("GreenBack", "data/resources/Background/GreenBack.png", {}, true);
	ImageManager::PreloadTexture("Cave", "data/resources/Background/Cave.png", {}, true);
}
void AddBg(std::string_view name, sf::Vector2f parallax) {
	BgLayer dat;
	dat.texture = ImageManager::getReturnTexture(name.data());

	dat.vertices.setPrimitiveType(sf::PrimitiveType::TriangleStrip);
	dat.vertices.resize(4);

	dat.vertices[0].texCoords = sf::Vector2f(0, 0);
	dat.vertices[1].texCoords = sf::Vector2f(LevelWidth, 0);
	dat.vertices[2].texCoords = sf::Vector2f(0, static_cast<float>(ImageManager::getReturnTexture(name)->getSize().y));
	dat.vertices[3].texCoords = sf::Vector2f(LevelWidth, static_cast<float>(ImageManager::getReturnTexture(name)->getSize().y));

	dat.vertices[0].position = sf::Vector2f(0, 0);
	dat.vertices[1].position = sf::Vector2f(LevelWidth, 0);
	dat.vertices[2].position = sf::Vector2f(0, static_cast<float>(ImageManager::getReturnTexture(name)->getSize().y));
	dat.vertices[3].position = sf::Vector2f(LevelWidth, static_cast<float>(ImageManager::getReturnTexture(name)->getSize().y));
	dat.parallax = {parallax.x > 1.f ? 1.f : parallax.x, parallax.y > 1.f ? 1.f : parallax.y};
	BgList.emplace_back(dat);
}
void BgUpdatePos() {
	for (auto &i : BgList) {
		const sf::Vector2f& ViewPos = Scroll::getViewPosition();
		sf::Vector2f viewOff = sf::Vector2f(ViewPos.x * (1.f - i.parallax.x), ViewPos.y * (1.f - i.parallax.y));

		i.vertices[0].position = viewOff;
		i.vertices[1].position = viewOff + sf::Vector2f(LevelWidth, 0.f);
		i.vertices[2].position = viewOff + sf::Vector2f(0.f, static_cast<float>(i.texture->getSize().y));
		i.vertices[3].position = viewOff + sf::Vector2f(LevelWidth, static_cast<float>(i.texture->getSize().y));
	}
}
void DeleteAllBg() {
	BgList.clear();
}
void BgDraw() {
	ZoneScopedNC("BgDraw", 0x6DB0C9);
	for (const auto &i : BgList) {
		WindowFrame::getWindow().draw(i.vertices, i.texture);
	}
}