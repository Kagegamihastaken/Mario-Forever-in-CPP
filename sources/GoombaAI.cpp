#include "Object/GoombaAI.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/Scroll.hpp"
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
#include "Core/Tilemap.hpp"
#include "Effect/BroAIEffect.hpp"
#include "Object/BroAI.hpp"

std::vector<MFCPP::GoombaAI> GoombaAIList;
static bool GoombaAIDeleteGate = false;

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
		if (i.willBeDestroyed()) continue;

		i.setPreviousPosition(i.getCurrentPosition());
	}
}
void InterpolateGoombaAIPos(const float alpha) {
	for (auto & i : GoombaAIList) {
		if (i.willBeDestroyed()) continue;

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
			sf::FloatRect({0.0f, 0.0f}, {32.0f, 28.0f}), sf::Vector2f(x, y),
			sf::Vector2f(16, 27), false, SkinID, 0.12f);
		GoombaAIList.back().setAnimation(3, 3, 100);
		GoombaAIList.back().setAnimationSequence(KoopaShellAnimName, KoopaShellAnimName);
		break;
		case SHELL_MOVING:
		GoombaAIList.emplace_back(type, Dir, GoombaAICollisionType::YES, 5.0f,
			sf::FloatRect({0.0f, 0.0f}, {32.0f, 28.0f}), sf::Vector2f(x, y),
			sf::Vector2f(16, 27), false, SkinID, 0.6f);
		GoombaAIList.back().setAnimation(0, 3, 54);
		GoombaAIList.back().setAnimationSequence(KoopaShellAnimName, KoopaShellAnimName);
		break;
		case SPINY:
		GoombaAIList.emplace_back(type, Dir, GoombaAICollisionType::NO, 1.0f,
			sf::FloatRect({0.0f, 0.0f}, {32.0f, 32.0f}), sf::Vector2f(x, y),
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
	GoombaAIDeleteGate = true;
	GoombaAIList[i].willDestroy(true);
}
void DeleteGoombaAI(const GoombaAIType type, const float x, const float y) {
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
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIList[i].willBeDestroyed()) continue;

		if (isOutScreenYBottom(GoombaAIList[i].getCurrentPosition().y, 80)) {
			DeleteGoombaAIIndex(i);
		}
		if (!isOutScreen(GoombaAIList[i].getCurrentPosition().x - GoombaAIList[i].getOrigin().x, GoombaAIList[i].getCurrentPosition().y, 32, 80)) {
			if (GoombaAIList[i].IsDisabled()) GoombaAIList[i].SetDisabled(false);
		}
		else continue;
		if (GoombaAIList[i].IsAppearing() && !GoombaAIList[i].IsDisabled()) {
			GoombaAIList[i].move(sf::Vector2f(0.0f, -0.5f * deltaTime));
			GoombaAIList[i].SetAppearY(GoombaAIList[i].GetAppearY() + 0.5f * deltaTime);
			if (GoombaAIList[i].GetAppearY() >= 32.0f) GoombaAIList[i].SetAppearing(false);
		}
	}
}
void GoombaAICheckCollide() {
	if (EffectActive) return;
	const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());

	for (int i = 0; i < GoombaAIList.size(); ++i){
		if (GoombaAIList[i].willBeDestroyed() || GoombaAIList[i].IsDisabled() || GoombaAIList[i].IsAppearing()) continue;
		if (f_abs(player.curr.x - GoombaAIList[i].getCurrentPosition().x) >= 160.0f) continue;

		if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(GoombaAIList[i].GetHitboxMain(), GoombaAIList[i].getCurrentPosition(), GoombaAIList[i].getOrigin()); isCollide(GoombaAIHitbox, hitbox_mario)) {
			if (GoombaAIList[i].GetCollisionType() == GoombaAICollisionType::YES) {
				if ((GoombaAIList[i].GetInvincibleTimer().getElapsedTime().asSeconds() >= GoombaAIList[i].GetInvincibleTimerLimit() && GoombaAIList[i].GetInvincibleTimerLimit() > 0.0f) || GoombaAIList[i].GetInvincibleTimerLimit() == 0.0f) {
					if ((GoombaAIList[i].getCurrentPosition().y - 16.f > player.curr.y) && Yvelo > 0.0f) {
						player.curr = {player.curr.x, GoombaAIList[i].getCurrentPosition().y - GoombaAIList[i].getOrigin().y - 1.0f};
						if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) Yvelo = -8.0f;
						else Yvelo = -13.0f;
						SoundManager::PlaySound("Stomp");
						switch (GoombaAIList[i].GetType()) {
						case GoombaAIType::GOOMBA:
							AddScoreEffect(SCORE_100, GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y - GoombaAIList[i].getOrigin().y);
							AddGoombaAIEffect(GoombaAIList[i].GetType(), COLLIDE, GoombaAIList[i].GetSkinID(), GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y);
							break;
						case GoombaAIType::KOOPA:
							AddScoreEffect(SCORE_100, GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y - GoombaAIList[i].getOrigin().y);
							AddGoombaAI(SHELL, GoombaAIList[i].GetSkinID(), GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y + 2.0f);
							break;
						case GoombaAIType::SHELL_MOVING:
							GoombaAIList[i].SetShellHitCount(0);
							AddScoreEffect(SCORE_100, GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y - GoombaAIList[i].getOrigin().y);
							AddGoombaAI(SHELL, GoombaAIList[i].GetSkinID(), GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y);
							break;
						case GoombaAIType::MUSHROOM:
						case GoombaAIType::SHELL:
						case GoombaAIType::SPINY:
						case GoombaAIType::FIRE_FLOWER:
						default: ;
						}
						DeleteGoombaAIIndex(i);
						break;
					}
				}
			}
			if (GoombaAIList[i].GetCollisionType() == GoombaAICollisionType::YES || GoombaAIList[i].GetCollisionType() == GoombaAICollisionType::NO) {
				if ((GoombaAIList[i].GetInvincibleTimer().getElapsedTime().asSeconds() >= GoombaAIList[i].GetInvincibleTimerLimit() && GoombaAIList[i].GetInvincibleTimerLimit() > 0.0f) || GoombaAIList[i].GetInvincibleTimerLimit() == 0.0f) {
					if ((GoombaAIList[i].getCurrentPosition().y - 16.0f) < player.curr.y) {
						PowerDown();
						break;
					}
				}
			}
			else if (GoombaAIList[i].GetCollisionType() == GoombaAICollisionType::FULL) {
				switch (GoombaAIList[i].GetType()) {
				case GoombaAIType::MUSHROOM:
					SoundManager::PlaySound("Powerup");
					AddScoreEffect(SCORE_1000, GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y - GoombaAIList[i].getOrigin().y);
					if (PowerState == 0) SetPowerState(1);
					DeleteGoombaAIIndex(i);
					break;
				case GoombaAIType::SHELL:
					if ((GoombaAIList[i].GetInvincibleTimer().getElapsedTime().asSeconds() > GoombaAIList[i].GetInvincibleTimerLimit() && GoombaAIList[i].GetInvincibleTimerLimit() > 0.0f) || GoombaAIList[i].GetInvincibleTimerLimit() == 0.0f) {
						SoundManager::PlaySound("Kick2");
						if (GoombaAIList[i].getCurrentPosition().x >= player.curr.x) AddGoombaAI(SHELL_MOVING, GoombaAIList[i].GetSkinID(), GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y, RIGHT);
						else AddGoombaAI(SHELL_MOVING, GoombaAIList[i].GetSkinID(), GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y, LEFT);
						DeleteGoombaAIIndex(i);
					}
					break;
				case GoombaAIType::FIRE_FLOWER:
					SoundManager::PlaySound("Powerup");
					AddScoreEffect(SCORE_1000, GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y - GoombaAIList[i].getOrigin().y);
					if (PowerState > 0) SetPowerState(2);
					else if (PowerState == 0) SetPowerState(1);
					DeleteGoombaAIIndex(i);
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
	std::pair<bool, bool> BrickCollide, LuckyCollide;
	float BrickCurrPosX = 0, BrickCurrPosY = 0, LuckyCurrPosX = 0, LuckyCurrPosY = 0;
	// Check if a GoombaAI collide with left or right
	for (auto & i : GoombaAIList) {
		std::pair<bool, bool> BrickCollideRemove = {false, false};
		std::pair<bool, bool> LuckyCollideRemove = {false, false};
		if (i.willBeDestroyed() || i.IsDisabled() || i.IsAppearing()) continue;
		//move
		if (i.GetDirection() == LEFT) i.move(sf::Vector2f(- i.GetXvelo() * deltaTime, 0.0f));
		else i.move(sf::Vector2f(i.GetXvelo() * deltaTime, 0.0f));
		//shell moving break block
		if (i.GetType() == SHELL_MOVING) {
			NoAdd = false;
			if (i.GetDirection() == RIGHT) {
				BrickCollideRemove = CheckCollisionRight(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxWall()), BrickCurrPosX, BrickCurrPosY, NoAdd, 1);
			}
			else if (i.GetDirection() == LEFT) {
				BrickCollideRemove = CheckCollisionLeft(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxWall()), BrickCurrPosX, BrickCurrPosY, NoAdd, 1);
			}
			if (BrickCollideRemove.first || BrickCollideRemove.second) {
				const BrickID CurrBrickID = GetIDBrick(BrickCurrPosX, BrickCurrPosY);
				switch (GetBrickAtt(BrickCurrPosX, BrickCurrPosY)) {
				case NORMAL:
					//i.SetDirection(static_cast<GoombaAIDirection>(!i.GetDirection()));
					SoundManager::PlaySound("Break");
					AddBrickParticle(CurrBrickID, BrickCurrPosX, BrickCurrPosY);
					DeleteBrick(BrickCurrPosX, BrickCurrPosY);
					MFCPP::setIndexTilemapCollision(BrickCurrPosX, BrickCurrPosY, false);
					Score += 50;
					break;
				case MULTICOIN:
					MultiBrickCoin(BrickCurrPosX, BrickCurrPosY, getBrickIndex(BrickCurrPosX, BrickCurrPosY));
				}
			}
			if (i.GetDirection() == RIGHT) {
				LuckyCollideRemove = CheckCollisionRight(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxWall()), LuckyCurrPosX, LuckyCurrPosY, NoAdd, 2);
			}
			else if (i.GetDirection() == LEFT) {
				LuckyCollideRemove = CheckCollisionLeft(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxWall()), LuckyCurrPosX, LuckyCurrPosY, NoAdd, 2);
			}
			if (LuckyCollideRemove.first || LuckyCollideRemove.second) {
				//i.SetDirection(static_cast<GoombaAIDirection>(!i.GetDirection()));
				LuckyHit(LuckyCurrPosX, LuckyCurrPosY, getLuckyIndex(LuckyCurrPosX, LuckyCurrPosY));
			}
		}
		// Count if size AllCollideList equal to CollideAddCounter
		CurrPosXCollide = 0, CurrPosYCollide = 0;
		// 0 for right direction; 1 for left direction
		NoAdd = false;
		// Loop through obstacles
		auto [fst, snd] = QuickCheckOnlyObstaclesSideCollision(
			MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxWall()),
			static_cast<bool>(i.GetDirection()), CurrPosXCollide, CurrPosYCollide, NoAdd);
		if (i.GetDirection() == RIGHT) {
			if (BrickCollideRemove.first || BrickCollideRemove.second) {
				BrickCollide = BrickCollideRemove;
				CurrPosXCollide = BrickCurrPosX;
			}
			else {
				BrickCollide = CheckCollisionRight(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxWall()), CurrPosXCollide, CurrPosYCollide, NoAdd, 1);
			}
			if (LuckyCollideRemove.first || LuckyCollideRemove.second) {
				LuckyCollide = LuckyCollideRemove;
				CurrPosXCollide = LuckyCurrPosX;
			}
			else
				LuckyCollide = CheckCollisionRight(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxWall()), CurrPosXCollide, CurrPosYCollide, NoAdd, 2);
		}
		else if (i.GetDirection() == LEFT) {
			if (BrickCollideRemove.first || BrickCollideRemove.second) {
				BrickCollide = BrickCollideRemove;
				CurrPosXCollide = BrickCurrPosX;
			}
			else {
				BrickCollide = CheckCollisionLeft(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxWall()), CurrPosXCollide, CurrPosYCollide, NoAdd, 1);
			}
			if (LuckyCollideRemove.first || LuckyCollideRemove.second) {
				LuckyCollide = LuckyCollideRemove;
				CurrPosXCollide = LuckyCurrPosX;
			}
			else
				LuckyCollide = CheckCollisionLeft(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxWall()), CurrPosXCollide, CurrPosYCollide, NoAdd, 2);
		}

		// Stop on wall
		// Adjust Position if collide
		if (fst || BrickCollide.first || LuckyCollide.first) {
			i.SetDirection(static_cast<GoombaAIDirection>(!i.GetDirection()));
			i.setCurrentPosition(sf::Vector2f(CurrPosXCollide + 32.0f + i.getOrigin().x, i.getCurrentPosition().y));
		}
		if (snd || BrickCollide.second || LuckyCollide.second) {
			i.SetDirection(static_cast<GoombaAIDirection>(!i.GetDirection()));
			i.setCurrentPosition(sf::Vector2f(CurrPosXCollide - (i.GetSize().size.x - i.getOrigin().x), i.getCurrentPosition().y));
		}
	}
}
void GoombaAIVertYUpdate(const float deltaTime) {
	float CurrPosYCollide;
	for (auto & i : GoombaAIList) {
		if (i.willBeDestroyed() || i.IsDisabled() || i.IsAppearing()) continue;
		// bottom update
		i.move(sf::Vector2f(0.0f, i.GetYvelo() * deltaTime));
		i.SetYvelo(i.GetYvelo() + (i.GetYvelo() >= 10.0f ? 0.0f : 1.f * deltaTime * 0.3f));
		//}
		if (QuickCheckBotCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), CurrPosYCollide)) {
			if (i.GetYvelo() >= 0.0f) {
				i.SetYvelo(0.0f);
				i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, CurrPosYCollide - (i.GetSize().size.y - i.getOrigin().y)));
			}
		}
		// top update
		if (QuickCheckTopCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), CurrPosYCollide)) {
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
	std::set<int> coll_set;
	bool flag = false;
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIList[i].willBeDestroyed() || GoombaAIList[i].IsAppearing() || GoombaAIList[i].IsDisabled() || !GoombaAIList[i].IsCanDeath()) continue;
		const sf::FloatRect hitbox_loop = getGlobalHitbox(GoombaAIList[i].GetHitboxMain(), GoombaAIList[i].getCurrentPosition() ,GoombaAIList[i].getOrigin());
		flag = false;
		for (int j = 0; j < GoombaAIList.size(); ++j) {
			if (i == j || !GoombaAIList[j].IsCanDeath() || GoombaAIList[j].willBeDestroyed() || GoombaAIList[j].IsAppearing() ||
				f_abs(GoombaAIList[i].getCurrentPosition().x - GoombaAIList[j].getCurrentPosition().x) >= 160.0f) continue;
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
				}
				else if (GoombaAIList[i].GetType() == SHELL_MOVING && GoombaAIList[j].GetType() != SHELL_MOVING) {
					if (isOutScreen(GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y, 96, 96)) break;
					Kicking(i, sf::Vector2f(GoombaAIList[j].getCurrentPosition().x, GoombaAIList[j].getCurrentPosition().y), GoombaAIList[j].getOrigin().y);
					AddGoombaAIEffect(GoombaAIList[j].GetType(), NONE, GoombaAIList[j].GetSkinID(), GoombaAIList[j].getCurrentPosition().x, GoombaAIList[j].getCurrentPosition().y);
					DeleteGoombaAIIndex(j);
					break;
				}
				else if (GoombaAIList[i].GetType() == SHELL_MOVING && GoombaAIList[j].GetType() == SHELL_MOVING) {
					if (isOutScreen(GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y, 96, 96)) break;
					SoundManager::PlaySound("Kick2");
					AddScoreEffect(SCORE_100, GoombaAIList[j].getCurrentPosition().x, GoombaAIList[j].getCurrentPosition().y - GoombaAIList[i].getOrigin().y);
					AddScoreEffect(SCORE_100, GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y - GoombaAIList[i].getOrigin().y);
					AddGoombaAIEffect(GoombaAIList[i].GetType(), NONE, GoombaAIList[i].GetSkinID(), GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y);
					DeleteGoombaAIIndex(i);
					AddGoombaAIEffect(GoombaAIList[j].GetType(), NONE, GoombaAIList[j].GetSkinID(), GoombaAIList[j].getCurrentPosition().x, GoombaAIList[j].getCurrentPosition().y);
					DeleteGoombaAIIndex(j);
					break;
				}
			}
		}
		if (!flag) {
			GoombaAIList[i].SetCollideWith({false, -1});
		}
		if (GoombaAIList[i].GetType() == SHELL_MOVING) {
			for (int j = 0; j < BroAIList.size(); ++j)  {
				if (isOutScreen(GoombaAIList[i].getCurrentPosition().x, GoombaAIList[i].getCurrentPosition().y, 96, 96)) break;
				if (const sf::FloatRect BroAIHitbox = getGlobalHitbox(BroAIList[j].getHitbox(), BroAIList[j].getCurrentPosition(), BroAIList[j].getOrigin()); isCollide(BroAIHitbox, hitbox_loop)) {
					Kicking(i, sf::Vector2f(BroAIList[j].getCurrentPosition().x, BroAIList[j].getCurrentPosition().y), BroAIList[j].getOrigin().y);
					AddBroAIEffect(BroAIList[j].getType(), static_cast<bool>(BroAIList[j].getAnimationDirection()), BroAIList[j].getCurrentPosition().x, BroAIList[j].getCurrentPosition().y);
					DeleteBroAIIndex(j);
				}
			}
		}
	}
	for (const auto& i : coll_set) {
		GoombaAIList[i].SetDirection(static_cast<GoombaAIDirection>(!GoombaAIList[i].GetDirection()));
	}
}
void GoombaAIDraw() {
	for (auto & i : GoombaAIList) {
		if (i.willBeDestroyed()) continue;
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
void GoombaAICleanup() {
	if (!GoombaAIDeleteGate) return;
	int i = 0;
	while (i < GoombaAIList.size()) {
		if (!GoombaAIList[i].willBeDestroyed()) ++i;
		else GoombaAIList.erase(GoombaAIList.begin() + i);
	}
	GoombaAIDeleteGate = false;
}