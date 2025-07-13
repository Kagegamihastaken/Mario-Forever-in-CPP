#include "Object/GoombaAI.hpp"
#include "Core/Loading/enum.hpp"
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
#include "Effect/MarioEffect.hpp"
#include "Core/Interpolation.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Class/GoombaAIClass.hpp"
#include "Effect/BroAIEffect.hpp"
#include "Object/BroAI.hpp"
#include "Core/Logging.hpp"
plf::colony<MFCPP::GoombaAI> GoombaAIList;
static bool GoombaAIDeleteGate = false;

static std::vector<std::string> GoombaAnimName;
static int GOOMBA_IMAGE_WIDTH = 62;
static int GOOMBA_WIDTH = 31;
static int GOOMBA_HEIGHT = 32;
static std::vector<std::string> MushroomAnimName;
static int MUSHROOM_IMAGE_WIDTH = 31;
static int MUSHROOM_WIDTH = 31;
static int MUSHROOM_HEIGHT = 32;
static std::vector<std::string> GreenKoopaAnimName;
static std::vector<std::string> RedKoopaAnimName;
static int KOOPA_IMAGE_WIDTH = 64;
static int KOOPA_WIDTH = 32;
static int KOOPA_HEIGHT = 47;
static std::vector<std::string> GreenKoopaShellAnimName;
static std::vector<std::string> RedKoopaShellAnimName;
static int KOOPA_SHELL_IMAGE_WIDTH = 132;
static int KOOPA_SHELL_WIDTH = 33;
static int KOOPA_SHELL_HEIGHT = 28;
static std::vector<std::string> RedSpinyAnimName;
static std::vector<std::string> GreenSpinyAnimName;
static int SPINY_IMAGE_WIDTH = 66;
static int SPINY_WIDTH = 33;
static int SPINY_HEIGHT = 32;
static std::vector<std::string> FireFlowerAnimName;
static int FIRE_FLOWER_IMAGE_WIDTH = 128;
static int FIRE_FLOWER_WIDTH = 32;
static int FIRE_FLOWER_HEIGHT = 32;


void GoombaAILoadRes() {
	for (int i = 0; i < GOOMBA_IMAGE_WIDTH / GOOMBA_WIDTH; i++) {
		ImageManager::AddTexture(fmt::format("Goomba_{}", i), "data/resources/Goomba/Goomba.png", sf::IntRect({i * GOOMBA_WIDTH, 0}, {GOOMBA_WIDTH, GOOMBA_HEIGHT}));
		GoombaAnimName.push_back(fmt::format("Goomba_{}", i));
	}
	for (int i = 0; i < MUSHROOM_IMAGE_WIDTH / MUSHROOM_WIDTH; i++) {
		ImageManager::AddTexture(fmt::format("Mushroom_{}", i), "data/resources/Mushroom.png", sf::IntRect({i * MUSHROOM_WIDTH, 0}, {MUSHROOM_WIDTH, MUSHROOM_HEIGHT}));
		MushroomAnimName.push_back(fmt::format("Mushroom_{}", i));
	}
	for (int i = 0; i < KOOPA_IMAGE_WIDTH / KOOPA_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("GreenKoopa_{}", i), "data/resources/Koopa/GreenKoopa.png", sf::IntRect({i * KOOPA_WIDTH, 0}, {KOOPA_WIDTH, KOOPA_HEIGHT}));
		GreenKoopaAnimName.push_back(fmt::format("GreenKoopa_{}", i));
		ImageManager::AddTexture(fmt::format("RedKoopa_{}", i), "data/resources/Koopa/RedKoopa.png", sf::IntRect({i * KOOPA_WIDTH, 0}, {KOOPA_WIDTH, KOOPA_HEIGHT}));
		RedKoopaAnimName.push_back(fmt::format("RedKoopa_{}", i));
	}
	for (int i = 0; i < KOOPA_SHELL_IMAGE_WIDTH / KOOPA_SHELL_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("GreenKoopaShell_{}", i), "data/resources/Koopa/GreenKoopaShell.png", sf::IntRect({i * KOOPA_SHELL_WIDTH, 0}, {KOOPA_SHELL_WIDTH, KOOPA_SHELL_HEIGHT}));
		GreenKoopaShellAnimName.push_back(fmt::format("GreenKoopaShell_{}", i));
		ImageManager::AddTexture(fmt::format("RedKoopaShell_{}", i), "data/resources/Koopa/RedKoopaShell.png", sf::IntRect({i * KOOPA_SHELL_WIDTH, 0}, {KOOPA_SHELL_WIDTH, KOOPA_SHELL_HEIGHT}));
		RedKoopaShellAnimName.push_back(fmt::format("RedKoopaShell_{}", i));
	}
	for (int i = 0; i < SPINY_IMAGE_WIDTH / SPINY_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("RedSpiny_{}", i), "data/resources/Spiny/RedSpiny.png", sf::IntRect({i * SPINY_WIDTH, 0}, {SPINY_WIDTH, SPINY_HEIGHT}));
		RedSpinyAnimName.push_back(fmt::format("RedSpiny_{}", i));
		ImageManager::AddTexture(fmt::format("GreenSpiny_{}", i), "data/resources/Spiny/GreenSpiny.png", sf::IntRect({i * SPINY_WIDTH, 0}, {SPINY_WIDTH, SPINY_HEIGHT}));
		GreenSpinyAnimName.push_back(fmt::format("GreenSpiny_{}", i));
	}
	for (int i = 0; i < FIRE_FLOWER_IMAGE_WIDTH / FIRE_FLOWER_WIDTH; ++i) {
		ImageManager::AddTexture(fmt::format("FireFlower_{}", i), "data/resources/FireFlower.png", sf::IntRect({i * FIRE_FLOWER_WIDTH, 0}, {FIRE_FLOWER_WIDTH, FIRE_FLOWER_HEIGHT}));
		FireFlowerAnimName.push_back(fmt::format("FireFlower_{}", i));
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
	plf::colony<MFCPP::GoombaAI>::colony_iterator<false> it;
	switch (type) {
		case GOOMBA:
			it = GoombaAIList.emplace(type, Dir, GoombaAICollisionType::YES, GoombaAIBehaviour::GOOMBAAI_NORMAL, 1.0f,
				sf::FloatRect({0.0f, 0.0f}, {31.0f, 32.0f}), sf::Vector2f(x, y),
				sf::Vector2f(15, 31), false, SkinID, 0.0f);
			it->setAnimation(0, 1, 11);
			it->setAnimationSequence(GoombaAnimName);
			break;
		case KOOPA:
			switch (SkinID) {
			case 0:
				it = GoombaAIList.emplace(type, Dir, GoombaAICollisionType::YES, GoombaAIBehaviour::GOOMBAAI_NORMAL, 1.0f,
					sf::FloatRect({0.0f, 0.0f}, {32.0f, 47.0f}), sf::Vector2f(x, y),
					sf::Vector2f(16, 46), false, SkinID, 0.0f);
				it->setAnimation(0, 1, 12, true);
				it->setAnimationSequence(GreenKoopaAnimName);
				break;
			case 1:
				it = GoombaAIList.emplace(type, Dir, GoombaAICollisionType::YES, GoombaAIBehaviour::GOOMBAAI_REDKOOPA, 2.0f,
					sf::FloatRect({0.0f, 0.0f}, {32.0f, 47.0f}), sf::Vector2f(x, y),
					sf::Vector2f(16, 46), false, SkinID, 0.0f);
				it->setAnimation(0, 1, 12, true);
				it->setAnimationSequence(RedKoopaAnimName);
				break;
			default: ;
			}
			break;
		case MUSHROOM:
			it = GoombaAIList.emplace(type, Dir, GoombaAICollisionType::FULL, GoombaAIBehaviour::GOOMBAAI_NORMAL, 2.0f,
				sf::FloatRect({0.0f, 0.0f}, {31.0f, 32.0f}), sf::Vector2f(x, y + 31.f),
				sf::Vector2f(16, 31), true, SkinID, 0.0f, false);
			it->setAnimation(0, 0, 100);
			it->setAnimationSequence(MushroomAnimName);
			break;
		case SHELL:
			switch (SkinID) {
			case 0:
				it = GoombaAIList.emplace(type, Dir, GoombaAICollisionType::FULL, GoombaAIBehaviour::GOOMBAAI_NORMAL, 0.0f,
					sf::FloatRect({0.0f, 0.0f}, {32.0f, 28.0f}), sf::Vector2f(x, y),
					sf::Vector2f(16, 27), false, SkinID, 0.12f);
				it->setAnimation(3, 3, 100);
				it->setAnimationSequence(GreenKoopaShellAnimName);
				break;
			case 1:
				it = GoombaAIList.emplace(type, Dir, GoombaAICollisionType::FULL, GoombaAIBehaviour::GOOMBAAI_NORMAL, 0.0f,
					sf::FloatRect({0.0f, 0.0f}, {32.0f, 28.0f}), sf::Vector2f(x, y),
					sf::Vector2f(16, 27), false, SkinID, 0.12f);
				it->setAnimation(3, 3, 100);
				it->setAnimationSequence(RedKoopaShellAnimName);
				break;
			default: ;
			}
			break;
		case SHELL_MOVING:
			switch (SkinID) {
			case 0:
				it = GoombaAIList.emplace(type, Dir, GoombaAICollisionType::YES, GoombaAIBehaviour::GOOMBAAI_NORMAL, 5.0f,
					sf::FloatRect({0.0f, 0.0f}, {32.0f, 28.0f}), sf::Vector2f(x, y),
					sf::Vector2f(16, 27), false, SkinID, 0.6f);
				it->setAnimation(0, 3, 54);
				it->setAnimationSequence(GreenKoopaShellAnimName);
				break;
			case 1:
				it = GoombaAIList.emplace(type, Dir, GoombaAICollisionType::YES, GoombaAIBehaviour::GOOMBAAI_NORMAL, 5.0f,
					sf::FloatRect({0.0f, 0.0f}, {32.0f, 28.0f}), sf::Vector2f(x, y),
					sf::Vector2f(16, 27), false, SkinID, 0.6f);
				it->setAnimation(0, 3, 54);
				it->setAnimationSequence(RedKoopaShellAnimName);
				break;
			default: ;
			}
			break;
		case SPINY:
			switch (SkinID) {
			case 0:
				it = GoombaAIList.emplace(type, Dir, GoombaAICollisionType::NO, GoombaAIBehaviour::GOOMBAAI_NORMAL, 1.0f,
					sf::FloatRect({0.0f, 0.0f}, {32.0f, 32.0f}), sf::Vector2f(x, y),
					sf::Vector2f(16, 31), false, SkinID, 0.0f);
				it->setAnimation(0, 1, 14, true);
				it->setAnimationSequence(RedSpinyAnimName);
				break;
			case 1:
				it = GoombaAIList.emplace(type, Dir, GoombaAICollisionType::NO, GoombaAIBehaviour::GOOMBAAI_REDKOOPA, 1.0f,
					sf::FloatRect({0.0f, 0.0f}, {32.0f, 32.0f}), sf::Vector2f(x, y),
					sf::Vector2f(16, 31), false, SkinID, 0.0f);
				it->setAnimation(0, 1, 14, true);
				it->setAnimationSequence(GreenSpinyAnimName);
				break;
			default: ;
			}
			break;
		case FIRE_FLOWER:
			it = GoombaAIList.emplace(type, Dir, GoombaAICollisionType::FULL, GoombaAIBehaviour::GOOMBAAI_NORMAL, 0.0f,
				sf::FloatRect({0.0f, 0.0f}, {32.0f, 32.0f}), sf::Vector2f(x, y + 31.f),
				sf::Vector2f(16, 31), true, SkinID, 0.0f, false);
			it->setAnimation(0, 3, 27);
			it->setAnimationSequence(FireFlowerAnimName);
			break;
		default: ;
	}
}
void DeleteGoombaAIIndex(const plf::colony<MFCPP::GoombaAI>::colony_iterator<false>& it) {
	GoombaAIDeleteGate = true;
	it->willDestroy(true);
}
void DeleteGoombaAI(const GoombaAIType type, const float x, const float y) {
	for (auto it = GoombaAIList.begin(); it != GoombaAIList.end(); ++it) {
		if (it->GetType() == type && it->getCurrentPosition().x == x && it->getCurrentPosition().y == y) {
			DeleteGoombaAIIndex(it);
		}
	}
}
void DeleteAllGoombaAI() {
	GoombaAIList.clear();
}
void GoombaStatusUpdate(const float deltaTime) {
	for (auto it = GoombaAIList.begin(); it != GoombaAIList.end(); ++it) {
		if (it->willBeDestroyed()) continue;

		if (isOutScreenYBottom(it->getCurrentPosition().y, 80)) {
			DeleteGoombaAIIndex(it);
		}
		if (!isOutScreen(it->getCurrentPosition().x - it->getOrigin().x, it->getCurrentPosition().y, 32, 80)) {
			if (it->IsDisabled()) it->SetDisabled(false);
		}
		else continue;
		if (it->IsAppearing() && !it->IsDisabled()) {
			it->move(sf::Vector2f(0.0f, -0.5f * deltaTime));
			it->SetAppearY(it->GetAppearY() + 0.5f * deltaTime);
			if (it->GetAppearY() >= 32.0f) it->SetAppearing(false);
		}
	}
}
void GoombaAICheckCollide() {
	if (EffectActive) return;
	const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.curr, player.property.getOrigin());

	for (auto it = GoombaAIList.begin(); it != GoombaAIList.end(); ++it) {
		if (it->willBeDestroyed() || it->IsDisabled() || it->IsAppearing()) continue;
		if (f_abs(player.curr.x - it->getCurrentPosition().x) >= 160.0f) continue;

		if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(it->GetHitboxMain(), it->getCurrentPosition(), it->getOrigin()); isCollide(GoombaAIHitbox, hitbox_mario)) {
			if (it->GetCollisionType() == GoombaAICollisionType::YES) {
				if ((it->GetInvincibleTimer().getElapsedTime().asSeconds() >= it->GetInvincibleTimerLimit() && it->GetInvincibleTimerLimit() > 0.0f) || it->GetInvincibleTimerLimit() == 0.0f) {
					if ((it->getCurrentPosition().y - 16.f > player.curr.y) && Yvelo > 0.0f) {
						//player.curr = {player.curr.x, it->getCurrentPosition().y - it->getOrigin().y - 1.0f};
						if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) Yvelo = -8.0f;
						else Yvelo = -13.0f;
						SoundManager::PlaySound("Stomp");
						switch (it->GetType()) {
						case GoombaAIType::GOOMBA:
							AddScoreEffect(SCORE_100, it->getCurrentPosition().x, it->getCurrentPosition().y - it->getOrigin().y);
							AddGoombaAIEffect(it->GetType(), COLLIDE, it->GetSkinID(), it->getCurrentPosition().x, it->getCurrentPosition().y);
							break;
						case GoombaAIType::KOOPA:
							AddScoreEffect(SCORE_100, it->getCurrentPosition().x, it->getCurrentPosition().y - it->getOrigin().y);
							AddGoombaAI(SHELL, it->GetSkinID(), it->getCurrentPosition().x, it->getCurrentPosition().y + 2.0f);
							break;
						case GoombaAIType::SHELL_MOVING:
							it->SetShellHitCount(0);
							AddScoreEffect(SCORE_100, it->getCurrentPosition().x, it->getCurrentPosition().y - it->getOrigin().y);
							AddGoombaAI(SHELL, it->GetSkinID(), it->getCurrentPosition().x, it->getCurrentPosition().y);
							break;
						case GoombaAIType::MUSHROOM:
						case GoombaAIType::SHELL:
						case GoombaAIType::SPINY:
						case GoombaAIType::FIRE_FLOWER:
						default: ;
						}
						DeleteGoombaAIIndex(it);
						break;
					}
				}
			}
			if (it->GetCollisionType() == GoombaAICollisionType::YES || it->GetCollisionType() == GoombaAICollisionType::NO) {
				if ((it->GetInvincibleTimer().getElapsedTime().asSeconds() >= it->GetInvincibleTimerLimit() && it->GetInvincibleTimerLimit() > 0.0f) || it->GetInvincibleTimerLimit() == 0.0f) {
					if ((it->getCurrentPosition().y - 16.0f) < player.curr.y) {
						PowerDown();
						break;
					}
				}
			}
			else if (it->GetCollisionType() == GoombaAICollisionType::FULL) {
				switch (it->GetType()) {
				case GoombaAIType::MUSHROOM:
					SoundManager::PlaySound("Powerup");
					AddScoreEffect(SCORE_1000, it->getCurrentPosition().x, it->getCurrentPosition().y - it->getOrigin().y);
					if (PowerState == 0) SetPowerState(1);
					DeleteGoombaAIIndex(it);
					break;
				case GoombaAIType::SHELL:
					if ((it->GetInvincibleTimer().getElapsedTime().asSeconds() > it->GetInvincibleTimerLimit() && it->GetInvincibleTimerLimit() > 0.0f) || it->GetInvincibleTimerLimit() == 0.0f) {
						SoundManager::PlaySound("Kick2");
						if (it->getCurrentPosition().x >= player.curr.x) AddGoombaAI(SHELL_MOVING, it->GetSkinID(), it->getCurrentPosition().x, it->getCurrentPosition().y, RIGHT);
						else AddGoombaAI(SHELL_MOVING, it->GetSkinID(), it->getCurrentPosition().x, it->getCurrentPosition().y, LEFT);
						DeleteGoombaAIIndex(it);
					}
					break;
				case GoombaAIType::FIRE_FLOWER:
					SoundManager::PlaySound("Powerup");
					AddScoreEffect(SCORE_1000, it->getCurrentPosition().x, it->getCurrentPosition().y - it->getOrigin().y);
					if (PowerState > 0) SetPowerState(2);
					else if (PowerState == 0) SetPowerState(1);
					DeleteGoombaAIIndex(it);
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
			BrickCollideRemove = CheckCollisionSide(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxWall()), BrickCurrPosX, BrickCurrPosY, NoAdd, 1);
			if (BrickCollideRemove.first || BrickCollideRemove.second) {
				BrickBreak(BrickCurrPosX, BrickCurrPosY);
			}
			LuckyCollideRemove = CheckCollisionSide(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxWall()), LuckyCurrPosX, LuckyCurrPosY, NoAdd, 2);
			if (LuckyCollideRemove.first || LuckyCollideRemove.second) {
				//i.SetDirection(static_cast<GoombaAIDirection>(!i.GetDirection()));
				LuckyHit(LuckyCurrPosX, LuckyCurrPosY);
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
		if (BrickCollideRemove.first || BrickCollideRemove.second) {
			BrickCollide = BrickCollideRemove;
			CurrPosXCollide = BrickCurrPosX;
		}
		else {
			BrickCollide = CheckCollisionSide(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxWall()), CurrPosXCollide, CurrPosYCollide, NoAdd, 1);
		}
		if (LuckyCollideRemove.first || LuckyCollideRemove.second) {
			LuckyCollide = LuckyCollideRemove;
			CurrPosXCollide = LuckyCurrPosX;
		}
		else
			LuckyCollide = CheckCollisionSide(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxWall()), CurrPosXCollide, CurrPosYCollide, NoAdd, 2);

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
	for (auto & i : GoombaAIList) {
		float CurrPosYCollide, CurrPosXCollide;
		if (i.willBeDestroyed() || i.IsDisabled() || i.IsAppearing()) continue;
		// bottom update
		i.move(sf::Vector2f(0.0f, i.GetYvelo() * deltaTime));
		i.SetYvelo(i.GetYvelo() + (i.GetYvelo() >= 10.0f ? 0.0f : 1.f * deltaTime * 0.3f));
		//}
		if (QuickCheckBotCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), CurrPosXCollide, CurrPosYCollide)) {
			//For RedKoopaAI Only, This depends on Bot Collision to work
			//THis thing needs to check if That Things is on the ground to automatic change
			if (i.GetBehaviour() == GoombaAIBehaviour::GOOMBAAI_REDKOOPA) {
				if (!isAccurateCollideBotStopEdge(MFCPP::CollisionObject({i.getCurrentPosition().x, i.getCurrentPosition().y + 1.f}, i.getOrigin(), i.GetHitboxMain()), i.GetDirection())) {
					i.setCurrentPosition(sf::Vector2f(i.getPreviousPosition().x, i.getCurrentPosition().y));
					i.SetDirection(static_cast<GoombaAIDirection>(!i.GetDirection()));
				}
			}

			if (i.GetYvelo() >= 0.0f) {
				const float floorY = GetCurrFloorY(i.getCurrentPosition(), CurrPosXCollide, CurrPosYCollide);
				if (i.getCurrentPosition().y < CurrPosYCollide + floorY - std::max(i.GetXvelo() + 1.f, 3.f)) continue;
				i.SetYvelo(0.0f);
				i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, CurrPosYCollide + floorY - (i.GetSize().size.y - i.getOrigin().y)));
				continue;
			}
		}
		// top update
		if (QuickCheckTopCollision(MFCPP::CollisionObject(i.getCurrentPosition(), i.getOrigin(), i.GetHitboxMain()), CurrPosXCollide, CurrPosYCollide)) {
			i.SetYvelo(0.0f);
			i.setCurrentPosition(sf::Vector2f(i.getCurrentPosition().x, CurrPosYCollide + (32.0f + i.getOrigin().y)));
		}
	}
}
static void Kicking(const plf::colony<MFCPP::GoombaAI>::colony_iterator<false>& it, const sf::Vector2f& pos, const float originY) {
	switch (it->GetShellHitCount()) {
	case 0:
		SoundManager::PlaySound("Kick2");
		AddScoreEffect(SCORE_100, pos.x, pos.y - originY);
		it->SetShellHitCount(it->GetShellHitCount() + 1);
		break;
	case 1:
		SoundManager::PlaySound("Kick3");
		AddScoreEffect(SCORE_200, pos.x, pos.y - originY);
		it->SetShellHitCount(it->GetShellHitCount() + 1);
		break;
	case 2:
		SoundManager::PlaySound("Kick4");
		AddScoreEffect(SCORE_500, pos.x, pos.y - originY);
		it->SetShellHitCount(it->GetShellHitCount() + 1);
		break;
	case 3:
		SoundManager::PlaySound("Kick5");
		AddScoreEffect(SCORE_1000, pos.x, pos.y - originY);
		it->SetShellHitCount(it->GetShellHitCount() + 1);
		break;
	case 4:
		SoundManager::PlaySound("Kick6");
		AddScoreEffect(SCORE_2000, pos.x, pos.y - originY);
		it->SetShellHitCount(it->GetShellHitCount() + 1);
		break;
	case 5:
		SoundManager::PlaySound("Kick7");
		AddScoreEffect(SCORE_5000, pos.x, pos.y - originY);
		it->SetShellHitCount(it->GetShellHitCount() + 1);
		break;
	case 6:
		SoundManager::PlaySound("Kick8");
		AddScoreEffect(SCORE_1UP, pos.x, pos.y - originY);
		it->SetShellHitCount(0);
		break;
	default: ;
	}
}
void GoombaAICollisionUpdate() {
	std::set<plf::colony<MFCPP::GoombaAI>::colony_iterator<false>> coll_set;
	bool flag = false;
	for (auto it = GoombaAIList.begin(); it != GoombaAIList.end(); ++it) {
		if (it->willBeDestroyed() || it->IsAppearing() || it->IsDisabled() || !it->IsCanDeath()) continue;
		const sf::FloatRect hitbox_loop = getGlobalHitbox(it->GetHitboxMain(), it->getCurrentPosition() ,it->getOrigin());
		flag = false;
		for (auto jt = GoombaAIList.begin(); jt != GoombaAIList.end(); ++jt) {
			if (it == jt || !jt->IsCanDeath() || jt->willBeDestroyed() || jt->IsAppearing() ||
				f_abs(it->getCurrentPosition().x - jt->getCurrentPosition().x) >= 160.0f) continue;
			if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(jt->GetHitboxMain(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(GoombaAIHitbox, hitbox_loop)) {
				if (jt->GetType() != SHELL_MOVING && it->GetType() != SHELL_MOVING) {
					if (jt->IsDisabled()) {
						coll_set.insert(it);
						flag = true;
						break;
					}
					else if ((!it->GetCollideWith().first && !jt->GetCollideWith().first) || it->GetCollideWith().second != jt->GetUUID() && it->GetCollideWith().first) {
						if (it->GetDirection() == jt->GetDirection() && it->GetXvelo() > 0.0f && jt->GetXvelo() > 0.0f) continue;
						else if (it->GetDirection() == RIGHT && jt->GetDirection() == LEFT) continue;

						it->SetCollideWith({true, jt->GetUUID()});

						coll_set.insert(it);
						coll_set.insert(jt);
						flag = true;
						break;
					}
					else if (it->GetCollideWith().first && it->GetCollideWith().second == jt->GetUUID()) {
						flag = true;
						break;
					}
				}
				else if (it->GetType() == SHELL_MOVING && jt->GetType() != SHELL_MOVING) {
					if (isOutScreen(it->getCurrentPosition().x, it->getCurrentPosition().y, 96, 96)) break;
					Kicking(it, sf::Vector2f(jt->getCurrentPosition().x, jt->getCurrentPosition().y), jt->getOrigin().y);
					AddGoombaAIEffect(jt->GetType(), NONE, jt->GetSkinID(), jt->getCurrentPosition().x, jt->getCurrentPosition().y);
					DeleteGoombaAIIndex(jt);
					break;
				}
				else if (it->GetType() == SHELL_MOVING && jt->GetType() == SHELL_MOVING) {
					if (isOutScreen(it->getCurrentPosition().x, it->getCurrentPosition().y, 96, 96)) break;
					SoundManager::PlaySound("Kick2");
					AddScoreEffect(SCORE_100, jt->getCurrentPosition().x, jt->getCurrentPosition().y - it->getOrigin().y);
					AddScoreEffect(SCORE_100, it->getCurrentPosition().x, it->getCurrentPosition().y - it->getOrigin().y);
					AddGoombaAIEffect(it->GetType(), NONE, it->GetSkinID(), it->getCurrentPosition().x, it->getCurrentPosition().y);
					DeleteGoombaAIIndex(it);
					AddGoombaAIEffect(jt->GetType(), NONE, jt->GetSkinID(), jt->getCurrentPosition().x, jt->getCurrentPosition().y);
					DeleteGoombaAIIndex(jt);
					break;
				}
			}
		}
		if (!flag) {
			it->SetCollideWith({false, it->GetUUID()});
		}
		if (it->GetType() == SHELL_MOVING) {
			for (auto jt = BroAIList.begin(); jt != BroAIList.end(); ++jt) {
				if (isOutScreen(it->getCurrentPosition().x, it->getCurrentPosition().y, 96, 96)) break;

				if (jt->willBeDestroyed()) continue;

				if (const sf::FloatRect BroAIHitbox = getGlobalHitbox(jt->getHitbox(), jt->getCurrentPosition(), jt->getOrigin()); isCollide(BroAIHitbox, hitbox_loop)) {
					Kicking(it, sf::Vector2f(jt->getCurrentPosition().x, jt->getCurrentPosition().y), jt->getOrigin().y);
					AddBroAIEffect(jt->getType(), static_cast<bool>(jt->getAnimationDirection()), jt->getCurrentPosition().x, jt->getCurrentPosition().y);
					DeleteBroAIIndex(jt);
				}
			}
		}
	}
	for (const auto& it : coll_set) {
		it->SetDirection(static_cast<GoombaAIDirection>(!it->GetDirection()));
	}
}
void GoombaAIDraw() {
	for (auto & i : GoombaAIList) {
		if (i.willBeDestroyed()) continue;
		//if (GoombaAIDirectionList[i] == LEFT) it->property.setColor(sf::Color(255, 0, 0));
		//else it->property.setColor(sf::Color(0, 0, 255));
		if (!i.IsDisabled()) {
			if (i.GetDirection() == RIGHT) {
				i.setAnimationDirection(AnimationDirection::ANIM_RIGHT);
				//if (it->property.getOrigin() != GoombaAIOriginList[i].first) it->property.setOrigin(GoombaAIOriginList[i].first);
			}
			else if (i.GetDirection() == LEFT) {
				i.setAnimationDirection(AnimationDirection::ANIM_LEFT);
				//if (it->property.getOrigin() != GoombaAIOriginList[i].second) it->property.setOrigin(GoombaAIOriginList[i].second);
			}
		}
		if (!isOutScreen(i.getInterpolatedPosition().x - i.getOrigin().x, i.getInterpolatedPosition().y, 32, 80)) {
			i.AnimationUpdate(i.getInterpolatedPosition(), sf::Vector2f(i.getOrigin().x, i.getOrigin().y));
			i.AnimationDraw(window);
			//window.draw(it->property);
		}
	}
}
void GoombaAICleanup() {
	if (!GoombaAIDeleteGate) return;
	auto it = GoombaAIList.begin();
	while (it != GoombaAIList.end()) {
		if (!it->willBeDestroyed()) ++it;
		else it = GoombaAIList.erase(it);
	}
	GoombaAIDeleteGate = false;
}