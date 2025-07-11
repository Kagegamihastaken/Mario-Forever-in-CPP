#include "Block/Obstacles.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/ImageManager.hpp"

//Obstacles define
sf::VertexArray ObstaclesVA;
sf::RenderTexture ObstacleRTexture;
//sf::Texture ObstaclesTexture;
//set ID for each texture obstacle
std::vector<std::array<int, 6>> ID_list{
	{0, 0, 0, 0, 0, 0},
	{1, 32, 0, 0, 0, 0},
	{2, 64, 0, 0, 0, 0},
	{3, 0, 32, 0, 0, 0},
	{4, 32, 32, 0, 0, 0},
	{5, 64, 32, 0, 0, 0},
	{6, 96, 0, 0, 0, 0},
	{7, 128, 0, 0, 0, 0},
	{8, 160, 0, 0, 0, 0},
	{9, 128, 32, 0, 0, 0},
	{10, 160, 32, 0, 0, 0},
	{11, 0, 96, 3, 0, 32},
	{12, 32, 96, 3, 16, 32},
	{13, 64, 96, 3, 0, 16},
	{14, 0, 64, 3, 32, 0},
	{15, 32, 64, 3, 32, 16},
	{16, 64, 64, 3, 16, 0},
};
//texture loading

void loadObstacleRes() {
	for (int i = 0; i < ID_list.size(); ++i) {
		ImageManager::AddTexture(fmt::format("Tile_{}", ID_list[i][0]), "data/resources/Tileset.png", sf::IntRect({ID_list[i][1], ID_list[i][2]}, {32, 32}));
	}
	//ObstaclesTexture = ImageManager::GetTexture("Tileset");
}
void ObstaclesDraw() {
	//sf::RenderStates states;
	//sf::Transformable trans;
	//states.transform *= trans.getTransform();
	//states.texture = ObstaclesTextureManager.GetTexture("Tileset");
	window.draw(ObstaclesVA, &ObstacleRTexture.getTexture());
	//for (const auto & i : ObstaclesList) {
	//	if (!isOutScreen(i.property.getPosition().x, i.property.getPosition().y, 32, 32)) {
	//		window.draw(i.property);
	//	}
	//}
}