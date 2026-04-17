#include "Core/Loading/enum.hpp"
#include "Effect/BrickParticle.hpp"
#include "Core/ImageManager.hpp"
#include "Core/Scene/GameScene.hpp"
#include "Object/Effect/NormalBrickParticle.hpp"

void BrickParticleInit() {
	//Load with Brick
	ImageManager::PreloadTexture("NormalBrickParticle", "data/resources/BrickParticle.png", sf::IntRect({0, 0}, {16, 16}));
	ImageManager::PreloadTexture("GrayBrickParticle", "data/resources/BrickParticle.png", sf::IntRect({16, 0}, {16, 16}));
}
void AddSubBrickParticle(const BrickID id, float Xvelo, float Yvelo, const float ori_x, const float ori_y) {
	GameScene::effectManager.addEffect<NormalBrickParticle>(sf::Vector2f(Xvelo, Yvelo), sf::Vector2f(ori_x, ori_y));
}
void AddBrickParticle(const BrickID id, const float ori_x, const float ori_y) {
	AddSubBrickParticle(id, -2.f, -8.f, ori_x + 8.f, ori_y + 8.f);
	AddSubBrickParticle(id, 2.f, -8.f, ori_x + 24.f, ori_y + 8.f);
	AddSubBrickParticle(id, -4.f, -7.f, ori_x + 8.f, ori_y + 24.f);
	AddSubBrickParticle(id, 4.f, -7.f, ori_x + 24.f, ori_y + 24.f);
}