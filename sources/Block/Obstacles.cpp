#include "Block/Obstacles.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/ImageManager.hpp"

//Obstacles define
sf::VertexArray ObstaclesVA;
sf::RenderTexture ObstacleRTexture;
//sf::Texture ObstaclesTexture;
//set ID for each texture obstacle
std::vector<std::array<int, 3>> ID_list{
	{0, 0, 0},
	{1, 32, 0},
	{2, 64, 0},
	{3, 0, 32},
	{4, 32, 32},
	{5, 64, 32},
	{6, 96, 0},
	{7, 128, 0},
	{8, 160, 0},
	{9, 128, 32},
	{10, 160, 32},
};
ImageManager ObstaclesTextureManager;
//texture loading

void loadObstacleRes() {
	ImageManager::AddImage("TilesetImage", "data/resources/Tileset.png");
	for (int i = 0; i < ID_list.size(); ++i) {
		ImageManager::AddTexture("TilesetImage", sf::IntRect({ID_list[i][1], ID_list[i][2]}, {32, 32}), "Tile_" + std::to_string(ID_list[i][0]));
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