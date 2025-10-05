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
	//Slope
	{11, 0, 96, 3, 0, 32},
	{12, 32, 96, 3, 16, 32},
	{13, 64, 96, 3, 0, 16},
	{14, 0, 64, 3, 32, 0},
	{15, 32, 64, 3, 16, 0},
	{16, 64, 64, 3, 32, 16},
	//Normal
	{17, 128, 64, 0, 0, 0},
	{18, 160, 64, 0, 0, 0},
	{19, 192, 0, 0, 0, 0},
	{20, 192, 32, 0, 0, 0},
	{21, 224, 0, 0, 0, 0},
	{22, 224, 32, 0, 0, 0},
	{23, 256, 0, 0, 0, 0},
	{24, 256, 32, 0, 0, 0},
	{25, 0, 128, 0, 0, 0},
	{26, 32, 128, 0, 0, 0},
	{27, 64, 128, 0, 0, 0},
	{28, 0, 160, 0, 0, 0},
	{29, 32, 160, 0, 0, 0},
	{30, 64, 160, 0, 0, 0},
	{31, 0, 192, 0, 0, 0},
	{32, 32, 192, 0, 0, 0},
	{33, 64, 192, 0, 0, 0},
	{34, 96, 32, 0, 0, 0},
	{35, 128, 96, 0, 0, 0},
	{36, 160, 96, 0, 0, 0},
	{37, 128, 128, 0, 0, 0},
	{38, 160, 128, 0, 0, 0},
	{39, 128, 160, 0, 0, 0},
	{40, 160, 160, 0, 0, 0},
	{41, 192, 128, 0, 0, 0},
	{42, 192, 160, 0, 0, 0},
	{43, 224, 128, 0, 0, 0},
	{44, 224, 160, 0, 0, 0},
	{45, 256, 128, 0, 0, 0},
	{46, 256, 160, 0, 0, 0},
};
//texture loading

void loadObstacleRes() {
	for (int i = 0; i < ID_list.size(); ++i)
		ImageManager::PreloadTexture(fmt::format("Tile_{}", ID_list[i][0]), "data/resources/Tileset.png", sf::IntRect({ID_list[i][1], ID_list[i][2]}, {32, 32}));
}
void loadObstacleTemp() {
	for (int i = 0; i < ID_list.size(); ++i)
		ImageManager::AddTexture(fmt::format("Tile_{}", ID_list[i][0]), "data/resources/Tileset.png", sf::IntRect({ID_list[i][1], ID_list[i][2]}, {32, 32}));
}
void ObstaclesDraw() {
	window.draw(ObstaclesVA, &ObstacleRTexture.getTexture());
}