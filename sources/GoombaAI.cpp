#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <set>

#include "Object/GoombaAI.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/Scroll.hpp"
#include "Block/Obstacles.hpp"
#include "Block/Brick.hpp"
#include "Block/LuckyBlock.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Animate/SingleAnimationObject.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Core/Collision/Collide.hpp"
#include "Effect/GoombaAIEffect.hpp"
#include "Object/Mario.hpp"
#include "Core/Sound.hpp"
#include "Core/ImageManager.hpp"
#include "Effect/BrickParticle.hpp"
#include "Effect/MarioEffect.hpp"
#include "Core/Interpolation.hpp"
#include "Class/CollisionObjectClass.hpp"
#include "Class/GoombaAIClass.hpp"
#include "Effect/BroAIEffect.hpp"
#include "Object/BroAI.hpp"

// alot vector
std::vector<MFCPP::GoombaAI> GoombaAIList;

static std::vector<std::string> GoombaAnimName;
static int GOOMBA_IMAGE_WIDTH = 62;
static int GOOMBA_WIDTH = 31;
static int GOOMBA_HEIGHT = 32;
static std::vector<std::string> MushroomAnimName;
static int MUSHROOM_IMAGE_WIDTH = 31;
static int MUSHROOM_WIDTH = 31;
static int MUSHROOM_HEIGHT = 32;
static std::vector<std::string> KoopaLeftAnimName;
static std::vector<std::string> KoopaRightAnimName;
static int KOOPA_IMAGE_WIDTH = 64;
static int KOOPA_WIDTH = 32;
static int KOOPA_HEIGHT = 47;
static std::vector<std::string> KoopaShellAnimName;
static int KOOPA_SHELL_IMAGE_WIDTH = 132;
static int KOOPA_SHELL_WIDTH = 33;
static int KOOPA_SHELL_HEIGHT = 28;
static std::vector<std::string> SpinyLeftAnimName;
static std::vector<std::string> SpinyRightAnimName;
static int RED_SPINY_IMAGE_WIDTH = 66;
static int RED_SPINY_WIDTH = 33;
static int RED_SPINY_HEIGHT = 32;
static std::vector<std::string> FireFlowerAnimName;
static int FIRE_FLOWER_IMAGE_WIDTH = 128;
static int FIRE_FLOWER_WIDTH = 32;
static int FIRE_FLOWER_HEIGHT = 32;


void GoombaAILoadRes() {

	ImageManager::AddImage("GoombaImage", "data/resources/Goomba/Goomba.png");
	for (int i = 0; i < GOOMBA_IMAGE_WIDTH / GOOMBA_WIDTH; i++) {
		ImageManager::AddTexture("GoombaImage", sf::IntRect({i * GOOMBA_WIDTH, 0}, {GOOMBA_WIDTH, GOOMBA_HEIGHT}), "Goomba_" + std::to_string(i));
		GoombaAnimName.push_back("Goomba_" + std::to_string(i));
	}
	ImageManager::AddImage("MushroomImage", "data/resources/Mushroom.png");
	for (int i = 0; i < MUSHROOM_IMAGE_WIDTH / MUSHROOM_WIDTH; i++) {
		ImageManager::AddTexture("MushroomImage", sf::IntRect({i * MUSHROOM_WIDTH, 0}, {MUSHROOM_WIDTH, MUSHROOM_HEIGHT}), "Mushroom_" + std::to_string(i));
		MushroomAnimName.push_back("Mushroom_" + std::to_string(i));
	}
	ImageManager::AddImage("GreenKoopaImage", "data/resources/Koopa/GreenKoopa.png");
	for (int i = 0; i < KOOPA_IMAGE_WIDTH / KOOPA_WIDTH; ++i) {
		ImageManager::AddTexture("GreenKoopaImage", sf::IntRect({i * KOOPA_WIDTH, 0}, {KOOPA_WIDTH, KOOPA_HEIGHT}), "GreenKoopaRight_" + std::to_string(i));
		ImageManager::AddTexture("GreenKoopaImage", sf::IntRect({i * KOOPA_WIDTH, 0}, {KOOPA_WIDTH, KOOPA_HEIGHT}), "GreenKoopaLeft_" + std::to_string(i), false, true);
		KoopaLeftAnimName.push_back("GreenKoopaLeft_" + std::to_string(i));
		KoopaRightAnimName.push_back("GreenKoopaRight_" + std::to_string(i));
	}
	ImageManager::AddImage("GreenKoopaShellImage", "data/resources/Koopa/GreenKoopaShell.png");
	for (int i = 0; i < KOOPA_SHELL_IMAGE_WIDTH / KOOPA_SHELL_WIDTH; ++i) {
		ImageManager::AddTexture("GreenKoopaShellImage", sf::IntRect({i * KOOPA_SHELL_WIDTH, 0}, {KOOPA_SHELL_WIDTH, KOOPA_SHELL_HEIGHT}), "GreenKoopaShell_" + std::to_string(i));
		KoopaShellAnimName.push_back("GreenKoopaShell_" + std::to_string(i));
	}
	ImageManager::AddImage("RedSpinyImage", "data/resources/Spiny/RedSpiny.png");
	for (int i = 0; i < RED_SPINY_IMAGE_WIDTH / RED_SPINY_WIDTH; ++i) {
		ImageManager::AddTexture("RedSpinyImage", sf::IntRect({i * RED_SPINY_WIDTH, 0}, {RED_SPINY_WIDTH, RED_SPINY_HEIGHT}), "RedSpinyRight_" + std::to_string(i));
		ImageManager::AddTexture("RedSpinyImage", sf::IntRect({i * RED_SPINY_WIDTH, 0}, {RED_SPINY_WIDTH, RED_SPINY_HEIGHT}), "RedSpinyLeft_" + std::to_string(i), false, true);
		SpinyLeftAnimName.push_back("RedSpinyLeft_" + std::to_string(i));
		SpinyRightAnimName.push_back("RedSpinyRight_" + std::to_string(i));
	}
	ImageManager::AddImage("FireFlowerImage", "data/resources/FireFlower.png");
	for (int i = 0; i < FIRE_FLOWER_IMAGE_WIDTH / FIRE_FLOWER_WIDTH; ++i) {
		ImageManager::AddTexture("FireFlowerImage", sf::IntRect({i * FIRE_FLOWER_WIDTH, 0}, {FIRE_FLOWER_WIDTH, FIRE_FLOWER_HEIGHT}), "FireFlower_" + std::to_string(i));
		FireFlowerAnimName.push_back("FireFlower_" + std::to_string(i));
	}
	//GoombaAITextureManager.LoadingAnimatedTexture(GOOMBA_TEXTURE, "Goomba", 0, 1, 0, 31, 32);
	//GoombaAITextureManager.LoadingAnimatedTexture(MUSHROOM_TEXTURE, "Mushroom", 0, 0, 0, 31, 32);
	//GoombaAITextureManager.LoadingAnimatedTexture(GREEN_KOOPA_TEXTURE, "Koopa_right_green", 0, 1, 0, 32, 47);
	//GoombaAITextureManager.LoadingAnimatedTexture(GREEN_KOOPA_TEXTURE, "Koopa_left_green", 0, 1, 1, 32, 47);
	//GoombaAITextureManager.LoadingAnimatedTexture(GREEN_KOOPA_SHELL_TEXTURE, "Koopa_Shell_green_idle", 3, 3, 0, 33, 28);
	//GoombaAITextureManager.LoadingAnimatedTexture(GREEN_KOOPA_SHELL_TEXTURE, "Koopa_Shell_green_moving", 0, 3, 0, 33, 28);
	//GoombaAITextureManager.LoadingAnimatedTexture(RED_SPINY_TEXTURE, "Spiny_right_red", 0, 1, 0, 33, 32);
	//GoombaAITextureManager.LoadingAnimatedTexture(RED_SPINY_TEXTURE, "Spiny_left_red", 0, 1, 1, 33, 32);
}
void SetPrevGoombaAIPos() {
	for (auto & i : GoombaAIList) {
		i.setPreviousPosition(i.getCurrentPosition());
	}
}
void InterpolateGoombaAIPos(const float alpha) {
	for (auto & i : GoombaAIList) {
		i.setInterpolatedPosition(linearInterpolation(i.getPreviousPosition(), i.getCurrentPosition(), alpha));
	}
}
void AddGoombaAI(GoombaAIType type, int SkinID, const float x, const float y, const GoombaAIDirection Dir = LEFT) {
	switch (type) {
		case GOOMBA:
		GoombaAIList.emplace_back(type, Dir, GoombaAICollisionType::YES, 1.0f,
			sf::FloatRect({0.0f, 0.0f}, {31.0f, 32.0f}), sf::Vector2f(x, y),
			sf::Vector2f(15, 31), false, SkinID, 0.0f);
		GoombaAIList.back().setAnimation(0, 1, 11);
		GoombaAIList.back().setAnimationSequence(GoombaAnimName, GoombaAnimName);
		break;
		case KOOPA:
		GoombaAIList.emplace_back(type, Dir, GoombaAICollisionType::YES, 1.0f,
			sf::FloatRect({0.0f, 0.0f}, {32.0f, 47.0f}), sf::Vector2f(x, y),
			sf::Vector2f(16, 44), false, SkinID, 0.0f);
		GoombaAIList.back().setAnimation(0, 1, 12);
		GoombaAIList.back().setAnimationSequence(KoopaLeftAnimName, KoopaRightAnimName);
		break;
		case MUSHROOM:
		GoombaAIList.emplace_back(type, Dir, GoombaAICollisionType::FULL, 2.0f,
			sf::FloatRect({0.0f, 0.0f}, {31.0f, 32.0f}), sf::Vector2f(x, y + 31.f),
			sf::Vector2f(16, 31), true, SkinID, 0.0f, false);
		GoombaAIList.back().setAnimation(0, 0, 100);
		GoombaAIList.back().setAnimationSequence(MushroomAnimName, MushroomAnimName);
		break;
		case SHELL:
		GoombaAIList.emplace_back(type, Dir, GoombaAICollisionType::FULL, 0.0f,
			sf::FloatRect({0.0f, 0.0f}, {33.0f, 28.0f}), sf::Vector2f(x, y),
			sf::Vector2f(16, 27), false, SkinID, 0.12f);
		GoombaAIList.back().setAnimation(3, 3, 100);
		GoombaAIList.back().setAnimationSequence(KoopaShellAnimName, KoopaShellAnimName);
		break;
		case SHELL_MOVING:
		GoombaAIList.emplace_back(type, Dir, GoombaAICollisionType::YES, 5.0f,
			sf::FloatRect({0.0f, 0.0f}, {33.0f, 28.0f}), sf::Vector2f(x, y),
			sf::Vector2f(16, 27), false, SkinID, 0.6f);
		GoombaAIList.back().setAnimation(0, 3, 54);
		GoombaAIList.back().setAnimationSequence(KoopaShellAnimName, KoopaShellAnimName);
		break;
		case SPINY:
		GoombaAIList.emplace_back(type, Dir, GoombaAICollisionType::NO, 1.0f,
			sf::FloatRect({0.0f, 0.0f}, {33.0f, 32.0f}), sf::Vector2f(x, y),
			sf::Vector2f(16, 29), false, SkinID, 0.6f);
		GoombaAIList.back().setAnimation(0, 1, 14);
		GoombaAIList.back().setAnimationSequence(SpinyLeftAnimName, SpinyRightAnimName);
		break;
		case FIRE_FLOWER:
		GoombaAIList.emplace_back(type, Dir, GoombaAICollisionType::FULL, 0.0f,
			sf::FloatRect({0.0f, 0.0f}, {32.0f, 32.0f}), sf::Vector2f(x, y + 31.f),
			sf::Vector2f(16, 31), true, SkinID, 0.0f, false);
		GoombaAIList.back().setAnimation(0, 3, 27);
		GoombaAIList.back().setAnimationSequence(FireFlowerAnimName);
		break;
		default: ;
	}
}
void DeleteGoombaAIIndex(const int i) {
	GoombaAIList.erase(GoombaAIList.begin() + i);
}
void DeleteGoombaAI(const GoombaAIType type, const float x, const float y) {
	//std::erase_if(GoombaAIList, [&](const MFCPP::GoombaAI &i) ->bool {
	//	return i.getCurrentPosition().x == x && i.getCurrentPosition().y == y && i.GetType() == type;
	//});
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIList[i].getCurrentPosition().x == x && GoombaAIList[i].getCurrentPosition().y == y && GoombaAIList[i].GetType() == type) {
			DeleteGoombaAIIndex(i);
			break;
		}
	}
}
void DeleteAllGoombaAI() {
	GoombaAIList.clear();
}
void GoombaStatusUpdate(const float deltaTime) {
	std::vector<sf::Vector2f> GoombaAIDeletionPositionList;
	std::vector<GoombaAIType> GoombaAIDeletionTypeList;
	for (auto & i : GoombaAIList) {
		if (isOutScreenYBottom(i.getCurrentPosition().y, 80)) {
			GoombaAIDeletionPositionList.push_back(i.getCurrentPosition());
			GoombaAIDeletionTypeList.push_back(i.GetType());
		}
		if (!isOutScreen(i.getCurrentPosition().x - i.getOrigin().x, i.getCurrentPosition().y, 32, 80)) {
			if (i.IsDisabled()) i.SetDisabled(false);
		}
		else continue;
		if (i.IsAppearing() && !i.IsDisabled()) {
			i.move(sf::Vector2f(0.0f, -0.5f * deltaTime));
			i.SetAppearY(i.GetAppearY() + 0.5f * deltaTime);
			if (i.GetAppearY() >= 32.0f) i.SetAppearing(false);
		}
	}
	if (GoombaAIDeletionPositionList.empty()) return;
	for (int i = 0; i < GoombaAIDeletionPositionList.size(); ++i) DeleteGoombaAI(GoombaAIDeletionTypeList[i], GoombaAIDeletionPositionList[i].x, GoombaAIDeletionPositionList[i].y);
}
void GoombaAICheckCollide() {
	if (EffectActive) return;
	const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());

	for (auto & i : GoombaAIList) {
		if (i.IsDisabled() || i.IsAppearing()) continue;
		if (f_abs(player.curr.x - i.getCurrentPosition().x) >= 160.0f) continue;
		if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(i.GetHitboxMain(), i.getCurrentPosition(), i.getOrigin()); isCollide(GoombaAIHitbox, hitbox_mario)) {
			if (i.GetCollisionType() == GoombaAICollisionType::YES) {
				if ((i.GetInvincibleTimer().getElapsedTime().asSeconds() >= i.GetInvincibleTimerLimit() && i.GetInvincibleTimerLimit() > 0.0f) || i.GetInvincibleTimerLimit() == 0.0f) {
					if ((i.getCurrentPosition().y - 16.f > player.curr.y) && Yvelo > 0.0f) {
						player.curr = {player.curr.x, i.getCurrentPosition().y - i.getOrigin().y - 1.0f};
						DeleteGoombaAI(i.GetType(), i.getCurrentPosition().x, i.getCurrentPosition().y);
						if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) Yvelo = -8.0f;
						else Yvelo = -13.0f;
						SoundManager::PlaySound("Stomp");
						switch (i.GetType()) {
						case GoombaAIType::GOOMBA:
							AddScoreEffect(SCORE_100, i.getCurrentPosition().x, i.getCurrentPosition().y - i.getOrigin().y);
							AddGoombaAIEffect(i.GetType(), COLLIDE, i.GetSkinID(), i.getCurrentPosition().x, i.getCurrentPosition().y);
							break;
						case GoombaAIType::KOOPA:
							AddScoreEffect(SCORE_100, i.getCurrentPosition().x, i.getCurrentPosition().y - i.getOrigin().y);
							AddGoombaAI(SHELL, i.GetSkinID(), i.getCurrentPosition().x, i.getCurrentPosition().y + 2.0f);
							break;
						case GoombaAIType::SHELL_MOVING:
							i.SetShellHitCount(0);
							AddScoreEffect(SCORE_100, i.getCurrentPosition().x, i.getCurrentPosition().y - i.getOrigin().y);
							AddGoombaAI(SHELL, i.GetSkinID(), i.getCurrentPosition().x, i.getCurrentPosition().y);
							break;
						case GoombaAIType::MUSHROOM:
						case GoombaAIType::SHELL:
						case GoombaAIType::SPINY:
						case GoombaAIType::FIRE_FLOWER:
						default: ;
						}
						break;
					}
				}
			}
			if (i.GetCollisionType() == GoombaAICollisionType::YES || i.GetCollisionType() == GoombaAICollisionType::NO) {
				if ((i.GetInvincibleTimer().getElapsedTime().asSeconds() >= i.GetInvincibleTimerLimit() && i.GetInvincibleTimerLimit() > 0.0f) || i.GetInvincibleTimerLimit() == 0.0f) {
					if ((i.getCurrentPosition().y - 16.0f) < player.curr.y) {
						PowerDown();
						break;
					}
				}
			}
			else if (i.GetCollisionType() == GoombaAICollisionType::FULL) {
				switch (i.GetType()) {
				case GoombaAIType::MUSHROOM:
					SoundManager::PlaySound("Powerup");
					AddScoreEffect(SCORE_1000, i.getCurrentPosition().x, i.getCurrentPosition().y - i.getOrigin().y);
					if (PowerState == 0) SetPowerState(1);
					DeleteGoombaAI(i.GetType(), i.getCurrentPosition().x, i.getCurrentPosition().y);
					break;
				case GoombaAIType::SHELL:
					if ((i.GetInvincibleTimer().getElapsedTime().asSeconds() > i.GetInvincibleTimerLimit() && i.GetInvincibleTimerLimit() > 0.0f) || i.GetInvincibleTimerLimit() == 0.0f) {
						SoundManager::PlaySound("Kick2");
						DeleteGoombaAI(i.GetType(), i.getCurrentPosition().x, i.getCurrentPosition().y);
						if (i.getCurrentPosition().x >= player.curr.x) AddGoombaAI(SHELL_MOVING, i.GetSkinID(), i.getCurrentPosition().x, i.getCurrentPosition().y, RIGHT);
						else AddGoombaAI(SHELL_MOVING, i.GetSkinID(), i.getCurrentPosition().x, i.getCurrentPosition().y, LEFT);
					}
					break;
				case GoombaAIType::FIRE_FLOWER:
					SoundManager::PlaySound("Powerup");
					AddScoreEffect(SCORE_1000, i.getCurrentPosition().x, i.getCurrentPosition().y - i.getOrigin().y);
					if (PowerState > 0) SetPowerState(2);
					else if (PowerState == 0) SetPowerState(1);
					DeleteGoombaAI(i.GetType(), i.getCurrentPosition().x, i.getCurrentPosition().y);
					break;
				case GoombaAIType::SHELL_MOVING:
				case GoombaAIType::GOOMBA:
				case GoombaAIType::KOOPA:
				case GoombaAIType::SPINY:
				default: ;
				}
				break;
			}
		}
	}
}
void GoombaAIVertXUpdate(const float deltaTime) {
	bool NoAdd;
	float CurrPosXCollide, CurrPosYCollide;
	std::pair<bool, bool> BrickCollideRemove, LuckyCollideRemove;
	std::pair<bool, bool> ObstacleCollide, BrickCollide, LuckyCollide;
	int be, nd;
	// Check if a GoombaAI collide with left or right
	for (auto & i : GoombaAIList) {
		if (i.IsDisabled() || i.IsAppearing()) continue;
		//move
		if (i.GetDirection() == LEFT) i.move(sf::Vector2f(- i.GetXvelo() * deltaTime, 0.0f));
		else i.move(sf::Vector2f(i.GetXvelo() * deltaTime, 0.0f));
		//shell moving break block
		if (i.GetType() == SHELL_MOVING) {
			float CusCurrPosX = 0, CusCurrPosY = 0;
			NoAdd = false;
			if (i.GetDirection() == RIGHT) {
				be = find_min_inx(i.getCurrentPosition(), BricksHorzPosList);
				nd = find_max_inx_dist(i.getCurrentPosition(), BricksHorzPosList, 64.0f + (i.GetXvelo()) * 16.0f);
				BrickCollideRemove = isAccurateCollideSide(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), BricksHorzPosList, CusCurrPosX, CusCurrPosY, NoAdd, be, nd, 80.0f);
			}
			else if (i.GetDirection() == LEFT) {
				be = find_max_inx(i.getCurrentPosition(), BricksHorzPosList);
				nd = find_min_inx_dist(i.getCurrentPosition(), BricksHorzPosList, 64.0f + (i.GetXvelo()) * 16.0f);
				BrickCollideRemove = isAccurateCollideSide(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), BricksHorzPosList, CusCurrPosX, CusCurrPosY, NoAdd, nd, be, 80.0f);
			}
			if (BrickCollideRemove.first || BrickCollideRemove.second) {
				i.SetDirection(static_cast<GoombaAIDirection>(!i.GetDirection()));
				const BrickID CurrBrickID = GetIDBrick(CusCurrPosX, CusCurrPosY);
				switch (GetBrickAtt(CusCurrPosX, CusCurrPosY)) {
				case NORMAL:
					SoundManager::PlaySound("Break");
					AddBrickParticle(CurrBrickID, CusCurrPosX, CusCurrPosY);
					DeleteBrick(CusCurrPosX, CusCurrPosY);
					Score += 50;
					break;
				case MULTICOIN:
					MultiBrickCoin(CusCurrPosX, CusCurrPosY - 32.0f, getBrickIndex(CusCurrPosX, CusCurrPosY));
				}
			}
			CusCurrPosX = 0, CusCurrPosY = 0;
			if (i.GetDirection() == RIGHT) {
				be = find_min_inx(i.getCurrentPosition(), LuckyHorzPosList);
				nd = find_max_inx_dist(i.getCurrentPosition(), LuckyHorzPosList, 64.0f + (i.GetXvelo()) * 16.0f);
				LuckyCollideRemove = isAccurateCollideSide(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), LuckyHorzPosList, CusCurrPosX, CusCurrPosY, NoAdd, be, nd, 80.0f);
			}
			else if (i.GetDirection() == LEFT) {
				be = find_max_inx(i.getCurrentPosition(), LuckyHorzPosList);
				nd = find_min_inx_dist(i.getCurrentPosition(), LuckyHorzPosList, 64.0f + (i.GetXvelo()) * 16.0f);
				LuckyCollideRemove = isAccurateCollideSide(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), LuckyHorzPosList, CusCurrPosX, CusCurrPosY, NoAdd, nd, be, 80.0f);
			}
			if (LuckyCollideRemove.first || LuckyCollideRemove.second) {
				i.SetDirection(static_cast<GoombaAIDirection>(!i.GetDirection()));
				LuckyHit(CusCurrPosX, CusCurrPosY - 32.0f, getLuckyIndex(CusCurrPosX, CusCurrPosY));
			}
		}
		// Count if size AllCollideList equal to CollideAddCounter
		CurrPosXCollide = 0, CurrPosYCollide = 0;
		// 0 for right direction; 1 for left direction
		NoAdd = false;
		// Loop through obstacles
		if (i.GetDirection() == RIGHT) {
			be = find_min_inx(i.getCurrentPosition(), ObstaclesHorzPosList);
			nd = find_max_inx_dist(i.getCurrentPosition(), ObstaclesHorzPosList, 64.0f + (i.GetXvelo()) * 16.0f);
			ObstacleCollide = isAccurateCollideSide(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), ObstaclesHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
			be = find_min_inx(i.getCurrentPosition(), BricksHorzPosList);
			nd = find_max_inx_dist(i.getCurrentPosition(), BricksHorzPosList, 64.0f + (i.GetXvelo()) * 16.0f);
			BrickCollide = isAccurateCollideSide(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), BricksHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
			be = find_min_inx(i.getCurrentPosition(), LuckyHorzPosList);
			nd = find_max_inx_dist(i.getCurrentPosition(), LuckyHorzPosList, 64.0f + (i.GetXvelo()) * 16.0f);
			LuckyCollide = isAccurateCollideSide(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), LuckyHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
		}
		else if (i.GetDirection() == LEFT) {
			be = find_max_inx(i.getCurrentPosition(), ObstaclesHorzPosList);
			nd = find_min_inx_dist(i.getCurrentPosition(), ObstaclesHorzPosList, 64.0f + (i.GetXvelo()) * 16.0f);
			ObstacleCollide = isAccurateCollideSide(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), ObstaclesHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
			be = find_max_inx(i.getCurrentPosition(), BricksHorzPosList);
			nd = find_min_inx_dist(i.getCurrentPosition(), BricksHorzPosList, 64.0f + (i.GetXvelo()) * 16.0f);
			BrickCollide = isAccurateCollideSide(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), BricksHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
			be = find_max_inx(i.getCurrentPosition(), LuckyHorzPosList);
			nd = find_min_inx_dist(i.getCurrentPosition(), LuckyHorzPosList, 64.0f + (i.GetXvelo()) * 16.0f);
			LuckyCollide = isAccurateCollideSide(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), LuckyHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		}

		// Stop on wall
		// Adjust Position if collide
		if (ObstacleCollide.first || BrickCollide.first || LuckyCollide.first) {
			i.SetDirection(static_cast<GoombaAIDirection>(!i.GetDirection()));
			i.setCurrentPosition(sf::Vector2f(CurrPosXCollide + 32.0f + i.getOrigin().x, i.getCurrentPosition().y));
		}
		if (ObstacleCollide.second || BrickCollide.second || LuckyCollide.second) {
			i.SetDirection(static_cast<GoombaAIDirection>(!i.GetDirection()));
			i.setCurrentPosition(sf::Vector2f(CurrPosXCollide - (i.GetSize().size.x - i.getOrigin().x), i.getCurrentPosition().y));
		}
	}
}
void GoombaAIVertYUpdate(const float deltaTime) {
	bool isLanding;
	float CurrPosYCollide;
	for (auto & i : GoombaAIList) {
		if (i.IsDisabled() || i.IsAppearing()) continue;
		// bottom update

		i.SetYvelo(i.GetYvelo() + (i.GetYvelo() >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f));
		i.move(sf::Vector2f(0.0f, i.GetYvelo() * deltaTime));
		i.SetYvelo(i.GetYvelo() + (i.GetYvelo() >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f));
		//}
		bool NoAdd = false;
		int be = find_min_iny(i.getCurrentPosition(), ObstaclesVertPosList);
		int nd = find_max_iny_dist(i.getCurrentPosition(), ObstaclesVertPosList, 64.0f + (i.GetYvelo()) * 16.0f);
		bool ObstacleCollide = isAccurateCollideBot(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), ObstaclesVertPosList,
		                                             CurrPosYCollide, NoAdd, be, nd, 80.0f);
		be = find_min_iny(i.getCurrentPosition(), BricksVertPosList);
		nd = find_max_iny_dist(i.getCurrentPosition(), BricksVertPosList, 64.0f + (i.GetYvelo()) * 16.0f);
		bool BrickCollide = isAccurateCollideBot(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), BricksVertPosList, CurrPosYCollide,
		                                          NoAdd, be, nd, 80.0f);
		be = find_min_iny(i.getCurrentPosition(), LuckyVertPosList);
		nd = find_max_iny_dist(i.getCurrentPosition(), LuckyVertPosList, 64.0f + (i.GetYvelo()) * 16.0f);
		bool LuckyCollide = isAccurateCollideBot(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), LuckyVertPosList, CurrPosYCollide,
		                                          NoAdd, be, nd, 80.0f);
		if (ObstacleCollide || BrickCollide || LuckyCollide) {
			if (i.GetYvelo() >= 0.0f) {
				i.SetYvelo(0.0f);
				isLanding = true;
			}
			else {
				i.SetYvelo(0.0f);
				isLanding = false;
			}
			//recolide
			if (isLanding)
				i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, CurrPosYCollide - (i.GetSize().size.y - i.getOrigin().y)));
		}
		// top update
		NoAdd = false;
		be = find_max_iny(i.getCurrentPosition(), ObstaclesVertPosList);
		nd = find_min_iny_dist(i.getCurrentPosition(), ObstaclesVertPosList, 64.0f - (i.GetYvelo()) * 16.0f);
		ObstacleCollide = isAccurateCollideTop(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), ObstaclesVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		be = find_max_iny(i.getCurrentPosition(), BricksVertPosList);
		nd = find_min_iny_dist(i.getCurrentPosition(), BricksVertPosList, 64.0f - (i.GetYvelo()) * 16.0f);
		BrickCollide = isAccurateCollideTop(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), BricksVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		be = find_max_iny(i.getCurrentPosition(), LuckyVertPosList);
		nd = find_min_iny_dist(i.getCurrentPosition(), LuckyVertPosList, 64.0f - (i.GetYvelo()) * 16.0f);
		LuckyCollide = isAccurateCollideTop(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), LuckyVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		if (ObstacleCollide || BrickCollide || LuckyCollide) {
			i.SetYvelo(0.0f);
			i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, CurrPosYCollide + (32.0f + i.getOrigin().y)));
		}
	}
}
static void Kicking(const int i, const sf::Vector2f& pos, const float originY) {
	switch (GoombaAIList[i].GetShellHitCount()) {
	case 0:
		SoundManager::PlaySound("Kick2");
		AddScoreEffect(SCORE_100, pos.x, pos.y - originY);
		GoombaAIList[i].SetShellHitCount(GoombaAIList[i].GetShellHitCount() + 1);
		break;
	case 1:
		SoundManager::PlaySound("Kick3");
		AddScoreEffect(SCORE_200, pos.x, pos.y - originY);
		GoombaAIList[i].SetShellHitCount(GoombaAIList[i].GetShellHitCount() + 1);
		break;
	case 2:
		SoundManager::PlaySound("Kick4");
		AddScoreEffect(SCORE_500, pos.x, pos.y - originY);
		GoombaAIList[i].SetShellHitCount(GoombaAIList[i].GetShellHitCount() + 1);
		break;
	case 3:
		SoundManager::PlaySound("Kick5");
		AddScoreEffect(SCORE_1000, pos.x, pos.y - originY);
		GoombaAIList[i].SetShellHitCount(GoombaAIList[i].GetShellHitCount() + 1);
		break;
	case 4:
		SoundManager::PlaySound("Kick6");
		AddScoreEffect(SCORE_2000, pos.x, pos.y - originY);
		GoombaAIList[i].SetShellHitCount(GoombaAIList[i].GetShellHitCount() + 1);
		break;
	case 5:
		SoundManager::PlaySound("Kick7");
		AddScoreEffect(SCORE_5000, pos.x, pos.y - originY);
		GoombaAIList[i].SetShellHitCount(GoombaAIList[i].GetShellHitCount() + 1);
		break;
	case 6:
		SoundManager::PlaySound("Kick8");
		AddScoreEffect(SCORE_1UP, pos.x, pos.y - originY);
		GoombaAIList[i].SetShellHitCount(0);
		break;
	default: ;
	}
}
void GoombaAICollisionUpdate() {
	std::set<std::pair<GoombaAIType, std::pair<float, float>>> GoombaAIDeleteSet;
	std::set<std::pair<float, float>> BroAIDeleteSet;
	std::set<int> coll_set;
	bool flag = false;
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIList[i].IsAppearing() || GoombaAIList[i].IsDisabled() || GoombaAIList[i].GetType() == GoombaAIType::MUSHROOM) continue;
		const sf::FloatRect hitbox_loop = getGlobalHitbox(GoombaAIList[i].GetHitboxMain(), GoombaAIList[i].getCurrentPosition() ,GoombaAIList[i].getOrigin());
		flag = false;
		for (int j = 0; j < GoombaAIList.size(); ++j) {
			if (f_abs(GoombaAIList[i].getCurrentPosition().x - GoombaAIList[j].getCurrentPosition().x) >= 160.0f) continue;
			else if (GoombaAIList[j].IsAppearing() || i == j || GoombaAIList[j].GetType() == GoombaAIType::MUSHROOM) continue;
			if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(GoombaAIList[j].GetHitboxMain(), GoombaAIList[j].getCurrentPosition(), GoombaAIList[j].getOrigin()); isCollide(GoombaAIHitbox, hitbox_loop)) {
				if (GoombaAIList[j].GetType() != SHELL_MOVING && GoombaAIList[i].GetType() != SHELL_MOVING) {
					if (GoombaAIList[j].IsDisabled()) {
						coll_set.insert(i);
						flag = true;
						break;
					}
					else if ((!GoombaAIList[i].GetCollideWith().first && !GoombaAIList[j].GetCollideWith().first) || (GoombaAIList[i].GetCollideWith().second != j && GoombaAIList[i].GetCollideWith().first)) {
						if (GoombaAIList[i].GetDirection() == GoombaAIList[j].GetDirection() && GoombaAIList[i].GetXvelo() > 0.0f && GoombaAIList[j].GetXvelo() > 0.0f) continue;
						else if (GoombaAIList[i].GetDirection() == RIGHT && GoombaAIList[j].GetDirection() == LEFT) continue;

						GoombaAIList[i].SetCollideWith({true, j});

						coll_set.insert(i);
						coll_set.insert(j);
						flag = true;
						break;
					}
					else if (GoombaAIList[i].GetCollideWith().first && GoombaAIList[i].GetCollideWith().second == j) {
						flag = true;
						break;
					}
					else continue;
				}
				else if (GoombaAIList[i].GetType() == SHELL_MOVING && GoombaAIList[j].GetType() != SHELL_MOVING) {
					Kicking(i, sf::Vector2f(GoombaAIList[j].getCurrentPosition().x, GoombaAIList[j].getCurrentPosition().y), GoombaAIList[j].getOrigin().y);
					AddGoombaAIEffect(GoombaAIList[j].GetType(), NONE, GoombaAIList[j].GetSkinID(), GoombaAIList[j].getCurrentPosition().x, GoombaAIList[j].getCurrentPosition().y);
					GoombaAIDeleteSet.insert({ GoombaAIList[j].GetType(), { GoombaAIList[j].getCurrentPosition().x , GoombaAIList[j].getCurrentPosition().y } });
					break;
				}
				else if (GoombaAIList[i].GetType() == SHELL_MOVING && GoombaAIList[j].GetType() == SHELL_MOVING) {
					SoundManager::PlaySound("Kick2");
					AddScoreEffect(SCORE_100, GoombaAIList[j].getCurrentPosition().x, GoombaAIList[j].getCurrentPosition().y - GoombaAIList[i].getOrigin().y);
					AddScoreEffect(SCORE_100, GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y - GoombaAIList[i].getOrigin().y);
					AddGoombaAIEffect(GoombaAIList[i].GetType(), NONE, GoombaAIList[i].GetSkinID(), GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y);
					GoombaAIDeleteSet.insert({ GoombaAIList[i].GetType(),  { GoombaAIList[i].getCurrentPosition().x , GoombaAIList[i].getCurrentPosition().y } });
					AddGoombaAIEffect(GoombaAIList[j].GetType(), NONE, GoombaAIList[j].GetSkinID(), GoombaAIList[j].getCurrentPosition().x, GoombaAIList[j].getCurrentPosition().y);
					GoombaAIDeleteSet.insert({ GoombaAIList[j].GetType(), { GoombaAIList[j].getCurrentPosition().x , GoombaAIList[j].getCurrentPosition().y } });
					break;
				}
			}
		}
		if (!flag) {
			GoombaAIList[i].SetCollideWith({false, -1});
		}
		if (GoombaAIList[i].GetType() == SHELL_MOVING) {
			for (const auto & j : BroAIList) {
				if (const sf::FloatRect BroAIHitbox = getGlobalHitbox(j.getHitbox(), j.getCurrentPosition(), j.getOrigin()); isCollide(BroAIHitbox, hitbox_loop)) {
					Kicking(i, sf::Vector2f(j.getCurrentPosition().x, j.getCurrentPosition().y), j.getOrigin().y);
					AddBroAIEffect(j.getType(), static_cast<bool>(j.getAnimationDirection()), j.getCurrentPosition().x, j.getCurrentPosition().y);
					BroAIDeleteSet.insert({j.getCurrentPosition().x, j.getCurrentPosition().y});
				}
			}
		}
	}
	for (const auto& i : coll_set) {
		GoombaAIList[i].SetDirection(static_cast<GoombaAIDirection>(!GoombaAIList[i].GetDirection()));
	}
	if (!GoombaAIDeleteSet.empty())
		for (const auto&[fst, snd] : GoombaAIDeleteSet)
			DeleteGoombaAI(fst, snd.first, snd.second);
	if (!BroAIDeleteSet.empty()) {
		for (const auto&[fst, snd] : BroAIDeleteSet)
			DeleteBroAI(fst, snd);
	}
}
void GoombaAIDraw() {
	for (auto & i : GoombaAIList) {
		//if (GoombaAIDirectionList[i] == LEFT) GoombaAIList[i].property.setColor(sf::Color(255, 0, 0));
		//else GoombaAIList[i].property.setColor(sf::Color(0, 0, 255));
		if (!i.IsDisabled()) {
			if (i.GetDirection() == RIGHT) {
				i.setAnimationDirection(AnimationDirection::ANIM_RIGHT);
				//if (GoombaAIList[i].property.getOrigin() != GoombaAIOriginList[i].first) GoombaAIList[i].property.setOrigin(GoombaAIOriginList[i].first);
			}
			else if (i.GetDirection() == LEFT) {
				i.setAnimationDirection(AnimationDirection::ANIM_LEFT);
				//if (GoombaAIList[i].property.getOrigin() != GoombaAIOriginList[i].second) GoombaAIList[i].property.setOrigin(GoombaAIOriginList[i].second);
			}
		}
		if (!isOutScreen(i.getInterpolatedPosition().x - i.getOrigin().x, i.getInterpolatedPosition().y, 32, 80)) {
			i.AnimationUpdate(i.getInterpolatedPosition(), sf::Vector2f(i.getOrigin().x, i.getOrigin().y));
			i.AnimationDraw(window);
			//window.draw(GoombaAIList[i].property);
		}
	}
}