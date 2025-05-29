#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <array>
#include <set>

#include "Object/GoombaAI.hpp"
#include "Core/Loading/enum.hpp"
#include "Core/Loading/Loading.hpp"
#include "Core/Scroll.hpp"
#include "Block/Obstacles.hpp"
#include "Block/Brick.hpp"
#include "Block/LuckyBlock.hpp"
#include "Core/WindowFrame.hpp"
#include "Core/Animate/LocalAnimationManager.hpp"
#include "Effect/ScoreEffect.hpp"
#include "Core/Collision/Collide.hpp"
#include "Effect/GoombaAIEffect.hpp"
#include "Object/Mario.hpp"
#include "Core/Sound.hpp"
#include "Core/TextureManager.hpp"
#include "Effect/BrickParticle.hpp"
#include "Effect/MarioEffect.hpp"
#include "Core/Interpolation.hpp"

// alot vector
std::vector<MovableObject> GoombaAIList;
std::vector<GoombaAIType> GoombaAITypeList;
std::vector<std::pair<float, float>> GoombaAIHitboxList; //
std::vector<GoombaAIDirection> GoombaAIDirectionList;
std::vector<float> GoombaAIYveloList;
std::vector<float> GoombaAIXveloList;
std::vector<bool> GoombaAIDisabledList;
std::vector<GoombaAIHittable> GoombaAIHittableList;
std::vector<std::array<float, 4>> GoombaAIDefinationList;
std::vector<std::pair<LocalAnimationManager, LocalAnimationManager>> GoombaAIAnimationList;
std::vector<bool> GoombaAIAppearingList;
std::vector<float> GoombaAIAppearingYList;
std::vector<int> GoombaAISkinIDList;
std::vector<std::pair<sf::Vector2f, sf::Vector2f>> GoombaAIOriginList;
std::vector<sf::Clock> GoombaAIInvincibleTimerList;
std::vector<float> GoombaAIInvincibleSecondLimitList;
std::vector<int> GoombaAIShellHitCount;
std::vector<std::pair<bool, int>> GoombaAICollisionList;
TextureManager GoombaAITextureManager;

void GoombaAILoadRes() {
	GoombaAITextureManager.Loadingtexture("data/resources/Goomba/Goomba.png", "Goomba", 0, 0, 62, 32);
	GoombaAITextureManager.Loadingtexture("data/resources/Mushroom.png", "Mushroom", 0, 0, 31, 32);
	GoombaAITextureManager.Loadingtexture("data/resources/Koopa/GreenKoopa.png", "Koopa_green", 0, 0, 64, 94);
	GoombaAITextureManager.Loadingtexture("data/resources/Koopa/GreenKoopaShell.png", "Koopa_Shell_green", 0, 0, 132, 28);
	GoombaAITextureManager.Loadingtexture("data/resources/Spiny/RedSpiny.png", "Spiny_red", 0, 0, 66, 64);

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
	for (int i = 0; i < GoombaAIList.size(); i++) {
		GoombaAIList[i].prev = GoombaAIList[i].curr;
	}
}
void InterpolateGoombaAIPos(const float alpha) {
	for (int i = 0; i < GoombaAIList.size(); i++) {
		GoombaAIList[i].property.setPosition(linearInterpolation(GoombaAIList[i].prev, GoombaAIList[i].curr, alpha));
	}
}
void AddGoombaAI(GoombaAIType type, int SkinID, float x, float y, GoombaAIDirection Dir = LEFT) {
	MovableObject Init;
	LocalAnimationManager in, inri;
	switch (type) {
	case GOOMBA:
		Init.property.setOrigin({ 15, 31 });
		GoombaAIDefinationList.push_back({ 31.0f, 32.0f, 0.0f, 0.0f });
		GoombaAIHitboxList.push_back({ 31.0f, 32.0f });
		in.setAnimation(0, 1, 31, 32, 0, 11);
		LocalAnimationManager::setTexture(Init.property, GoombaAITextureManager.GetTexture("Goomba"));
		inri.setAnimation(0, 1, 31, 32, 0, 11);
		GoombaAIXveloList.push_back(1.0f);
		GoombaAIHittableList.push_back(YES);
		GoombaAIAppearingList.push_back(false);
		GoombaAIOriginList.emplace_back( sf::Vector2f(15, 31), sf::Vector2f(15, 31) );
		GoombaAIInvincibleSecondLimitList.push_back(0.0f);
		break;
	case KOOPA:
		Init.property.setOrigin({ 13, 44 });
		GoombaAIDefinationList.push_back({ 32.0f, 47.0f, 0.0f, 0.0f });
		GoombaAIHitboxList.emplace_back( 32.0f, 47.0f );
		in.setAnimation(0, 1, 32, 47, 0, 12);
		LocalAnimationManager::setTexture(Init.property, GoombaAITextureManager.GetTexture("Koopa_green"));
		inri.setAnimation(0, 1, 32, 47, 1, 12);
		GoombaAIXveloList.push_back(1.0f);
		GoombaAIHittableList.push_back(YES);
		GoombaAIAppearingList.push_back(false);
		GoombaAIOriginList.push_back({ sf::Vector2f(13, 44), sf::Vector2f(18, 44) });
		GoombaAIInvincibleSecondLimitList.push_back(0.0f);
		break;
	case MUSHROOM:
		Init.property.setOrigin({ 16, 31 });
		GoombaAIDefinationList.push_back({ 31.0f, 32.0f, 0.0f, 0.0f });
		GoombaAIHitboxList.push_back({ 31.0f, 32.0f });
		in.setAnimation(0, 0, 31, 32, 0, 100);
		LocalAnimationManager::setTexture(Init.property, GoombaAITextureManager.GetTexture("Mushroom"));
		inri.setAnimation(0, 0, 31, 32, 0, 100);
		GoombaAIXveloList.push_back(2.0f);
		GoombaAIHittableList.push_back(FULL);
		GoombaAIAppearingList.push_back(true);
		GoombaAIOriginList.push_back({ sf::Vector2f(15, 31), sf::Vector2f(15, 31) });
		GoombaAIInvincibleSecondLimitList.push_back(0.0f);
		break;
	case SHELL:
		Init.property.setOrigin({ 16, 27 });
		GoombaAIDefinationList.push_back({ 33.0f, 28.0f, 0.0f, 0.0f });
		GoombaAIHitboxList.push_back({ 33.0f, 28.0f });
		in.setAnimation(3, 3, 33, 28, 0, 100);
		LocalAnimationManager::setTexture(Init.property, GoombaAITextureManager.GetTexture("Koopa_Shell_green"));
		inri.setAnimation(3, 3, 33, 28, 0, 100);
		GoombaAIXveloList.push_back(0.0f);
		GoombaAIHittableList.push_back(FULL);
		GoombaAIAppearingList.push_back(false);
		GoombaAIOriginList.push_back({ sf::Vector2f(16, 27), sf::Vector2f(16, 27) });
		GoombaAIInvincibleSecondLimitList.push_back(0.12f);
		break;
	case SHELL_MOVING:
		Init.property.setOrigin({ 16, 27 });
		GoombaAIDefinationList.push_back({ 33.0f, 28.0f, 0.0f, 0.0f });
		GoombaAIHitboxList.push_back({ 33.0f, 28.0f });
		in.setAnimation(0, 3, 33, 28, 0, 54);
		LocalAnimationManager::setTexture(Init.property, GoombaAITextureManager.GetTexture("Koopa_Shell_green"));
		inri.setAnimation(0, 3, 33, 28, 0, 54);
		GoombaAIXveloList.push_back(5.0f);
		GoombaAIHittableList.push_back(YES);
		GoombaAIAppearingList.push_back(false);
		GoombaAIOriginList.push_back({ sf::Vector2f(16, 27), sf::Vector2f(16, 27) });
		GoombaAIInvincibleSecondLimitList.push_back(0.6f);
		break;
	case SPINY:
		Init.property.setOrigin({ 16, 29 });
		GoombaAIDefinationList.push_back({ 33.0f, 32.0f, 0.0f, 0.0f });
		GoombaAIHitboxList.push_back({ 33.0f, 32.0f });
		in.setAnimation(0, 1, 33, 32, 0, 14);
		LocalAnimationManager::setTexture(Init.property, GoombaAITextureManager.GetTexture("Spiny_red"));
		inri.setAnimation(0, 1, 33, 32, 1, 14);
		GoombaAIXveloList.push_back(1.0f);
		GoombaAIHittableList.push_back(NO);
		GoombaAIAppearingList.push_back(false);
		GoombaAIOriginList.push_back({ sf::Vector2f(16, 31), sf::Vector2f(17, 31) });
		GoombaAIInvincibleSecondLimitList.push_back(0.0f);
		break;
	}
	GoombaAIInvincibleTimerList.push_back(sf::Clock());
	GoombaAIInvincibleTimerList[GoombaAIInvincibleTimerList.size() - 1].restart();
	GoombaAIShellHitCount.push_back(0);
	setHitbox(Init.hitboxMain, sf::FloatRect({ 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3] }, { GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second }));
	setHitbox(Init.hitboxTop, sf::FloatRect({ 1.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3] }, { GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first - 2.0f, 2.0f }));
	setHitbox(Init.hitboxBot, sf::FloatRect({ 1.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3] }, { GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first - 2.0f, 2.0f }));
	setHitbox(Init.hitboxRight, sf::FloatRect({ GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first - 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3] }, { 2.0f, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 9.0f }));
	setHitbox(Init.hitboxLeft, sf::FloatRect({ 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3] }, { 2.0f, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 9.0f }));
	Init.property.setPosition({ x, y });
	Init.curr = Init.property.getPosition();
	Init.prev = Init.property.getPosition();
	GoombaAIAnimationList.push_back({ in, inri });
	GoombaAIList.emplace_back(Init);
	GoombaAIDirectionList.push_back(Dir);
	GoombaAITypeList.push_back(type);
	GoombaAIYveloList.push_back(0.0f);
	GoombaAIDisabledList.push_back(true);
	GoombaAIAppearingYList.push_back(0.0f);
	GoombaAISkinIDList.push_back(SkinID);
	GoombaAICollisionList.push_back({ false, -1 });
}
void DeleteGoombaAIIndex(const int i) {
	GoombaAIList.erase(GoombaAIList.begin() + i);
	GoombaAIDirectionList.erase(GoombaAIDirectionList.begin() + i);
	GoombaAITypeList.erase(GoombaAITypeList.begin() + i);
	GoombaAIYveloList.erase(GoombaAIYveloList.begin() + i);
	GoombaAIXveloList.erase(GoombaAIXveloList.begin() + i);
	GoombaAIDisabledList.erase(GoombaAIDisabledList.begin() + i);
	GoombaAIAnimationList.erase(GoombaAIAnimationList.begin() + i);
	GoombaAIHittableList.erase(GoombaAIHittableList.begin() + i);
	GoombaAIDefinationList.erase(GoombaAIDefinationList.begin() + i);
	GoombaAIHitboxList.erase(GoombaAIHitboxList.begin() + i);
	GoombaAIAppearingList.erase(GoombaAIAppearingList.begin() + i);
	GoombaAIAppearingYList.erase(GoombaAIAppearingYList.begin() + i);
	GoombaAISkinIDList.erase(GoombaAISkinIDList.begin() + i);
	GoombaAIOriginList.erase(GoombaAIOriginList.begin() + i);
	GoombaAIInvincibleTimerList.erase(GoombaAIInvincibleTimerList.begin() + i);
	GoombaAIInvincibleSecondLimitList.erase(GoombaAIInvincibleSecondLimitList.begin() + i);
	GoombaAIShellHitCount.erase(GoombaAIShellHitCount.begin() + i);
	GoombaAICollisionList.erase(GoombaAICollisionList.begin() + i);
	//GoombaAICollisionIDList.erase(GoombaAIAnimationList.begin() + i);
}
void DeleteGoombaAI(const GoombaAIType type, const float x, const float y) {
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIList[i].curr.x == x && GoombaAIList[i].curr.y == y && GoombaAITypeList[i] == type) {
			DeleteGoombaAIIndex(i);
			//GoombaAICollisionIDList.erase(GoombaAIAnimationList.begin() + i);
			break;
		}
	}
}
void DeleteAllGoombaAI() {
	GoombaAIList.clear();
	GoombaAIDirectionList.clear();
	GoombaAITypeList.clear();
	GoombaAIYveloList.clear();
	GoombaAIXveloList.clear();
	GoombaAIDisabledList.clear();
	GoombaAIAnimationList.clear();
	GoombaAIHittableList.clear();
	GoombaAIDefinationList.clear();
	GoombaAIHitboxList.clear();
	GoombaAIAppearingList.clear();
	GoombaAIAppearingYList.clear();
	GoombaAISkinIDList.clear();
	GoombaAIOriginList.clear();
	GoombaAIInvincibleTimerList.clear();
	GoombaAIInvincibleSecondLimitList.clear();
	GoombaAIShellHitCount.clear();
	GoombaAICollisionList.clear();
}
void GoombaStatusUpdate(const float deltaTime) {
	std::vector<sf::Vector2f> GoombaAIDeletionPositionList;
	std::vector<GoombaAIType> GoombaAIDeletionTypeList;
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (isOutScreenYBottom(GoombaAIList[i].curr.y, 80)) {
			GoombaAIDeletionPositionList.push_back(GoombaAIList[i].curr);
			GoombaAIDeletionTypeList.push_back(GoombaAITypeList[i]);
		}
		if (!isOutScreen(GoombaAIList[i].curr.x - GoombaAIList[i].property.getOrigin().x, GoombaAIList[i].curr.y, 32, 80)) {
			if (GoombaAIDisabledList[i]) GoombaAIDisabledList[i] = false;
		}
		else continue;
		if (GoombaAIAppearingList[i] && !GoombaAIDisabledList[i]) {
			GoombaAIList[i].curr = { GoombaAIList[i].curr.x,GoombaAIList[i].curr.y - 0.5f * deltaTime };
			GoombaAIAppearingYList[i] += 0.5f * deltaTime;
			if (GoombaAIAppearingYList[i] >= 32.0f) GoombaAIAppearingList[i] = false;
		}
	}
	if (GoombaAIDeletionPositionList.size() == 0) return;
	for (int i = 0; i < GoombaAIDeletionPositionList.size(); ++i) DeleteGoombaAI(GoombaAIDeletionTypeList[i], GoombaAIDeletionPositionList[i].x, GoombaAIDeletionPositionList[i].y);
}
void GoombaAICheckCollide() {
	if (EffectActive) return;
	const sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.property);
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIDisabledList[i] || GoombaAIAppearingList[i]) continue;
		if (f_abs(player.curr.x - GoombaAIList[i].curr.x) >= 160.0f) continue;
		if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(GoombaAIList[i].hitboxMain, GoombaAIList[i].curr, GoombaAIList[i].property.getOrigin()); isCollide(GoombaAIHitbox, hitbox_mario)) {
			if (GoombaAIHittableList[i] == YES) {
				if ((GoombaAIInvincibleTimerList[i].getElapsedTime().asSeconds() >= GoombaAIInvincibleSecondLimitList[i] && GoombaAIInvincibleSecondLimitList[i] > 0.0f) || GoombaAIInvincibleSecondLimitList[i] == 0.0f) {
					if (((GoombaAIList[i].curr.y - 8.0f) > player.curr.y) && Yvelo > 0.0f && !GoombaAIDisabledList[i]) {
						player.curr = {player.curr.x, GoombaAIList[i].curr.y - GoombaAIList[i].property.getOrigin().y - 1.0f};
						if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) Yvelo = -8.0f;
						else Yvelo = -13.0f;
						Sounds.PlaySound("Stomp");
						switch (GoombaAITypeList[i]) {
						case GoombaAIType::GOOMBA:
							AddScoreEffect(SCORE_100, GoombaAIList[i].curr.x, GoombaAIList[i].curr.y - GoombaAIHitboxList[0].second);
							AddGoombaAIEffect(GoombaAITypeList[i], COLLIDE, GoombaAISkinIDList[i], GoombaAIList[i].curr.x, GoombaAIList[i].curr.y);
							break;
						case GoombaAIType::KOOPA:
							AddScoreEffect(SCORE_100, GoombaAIList[i].curr.x, GoombaAIList[i].curr.y - GoombaAIHitboxList[0].second);
							AddGoombaAI(SHELL, GoombaAISkinIDList[i], GoombaAIList[i].curr.x, GoombaAIList[i].curr.y + 2.0f);
							break;
						case GoombaAIType::SHELL_MOVING:
							GoombaAIShellHitCount[i] = 0;
							AddScoreEffect(SCORE_100, GoombaAIList[i].curr.x, GoombaAIList[i].curr.y - GoombaAIHitboxList[0].second);
							AddGoombaAI(SHELL, GoombaAISkinIDList[i], GoombaAIList[i].curr.x, GoombaAIList[i].curr.y);
							break;
						case GoombaAIType::MUSHROOM:
						case GoombaAIType::SHELL:
						case GoombaAIType::SPINY:
						default:
							break;
						}
						DeleteGoombaAI(GoombaAITypeList[i], GoombaAIList[i].curr.x, GoombaAIList[i].curr.y);
						break;
					}
				}
			}
			if (GoombaAIHittableList[i] == YES || GoombaAIHittableList[i] == NO) {
				if ((GoombaAIInvincibleTimerList[i].getElapsedTime().asSeconds() > GoombaAIInvincibleSecondLimitList[i] && GoombaAIInvincibleSecondLimitList[i] > 0.0f) || GoombaAIInvincibleSecondLimitList[i] == 0.0f) {
					if ((GoombaAIList[i].curr.y - 16.0f) < player.curr.y) {
						PowerDown();
						break;
					}
				}
			}
			else if (GoombaAIHittableList[i] == FULL) {
				switch (GoombaAITypeList[i]) {
				case GoombaAIType::MUSHROOM:
					Sounds.PlaySound("Powerup");
					if (GoombaAITypeList[i] == MUSHROOM) AddScoreEffect(SCORE_1000, GoombaAIList[i].curr.x, GoombaAIList[i].curr.y - GoombaAIHitboxList[0].second);
					if (PowerState == 0) PowerState = 1;
					DeleteGoombaAI(GoombaAITypeList[i], GoombaAIList[i].curr.x, GoombaAIList[i].curr.y);
					break;
				case GoombaAIType::SHELL:
					if ((GoombaAIInvincibleTimerList[i].getElapsedTime().asSeconds() > GoombaAIInvincibleSecondLimitList[i] && GoombaAIInvincibleSecondLimitList[i] > 0.0f) || GoombaAIInvincibleSecondLimitList[i] == 0.0f) {
						Sounds.PlaySound("Kick2");
						if (GoombaAIList[i].curr.x >= player.curr.x) AddGoombaAI(SHELL_MOVING, GoombaAISkinIDList[i], GoombaAIList[i].curr.x, GoombaAIList[i].curr.y, RIGHT);
						else AddGoombaAI(SHELL_MOVING, GoombaAISkinIDList[i], GoombaAIList[i].curr.x, GoombaAIList[i].curr.y, LEFT);
						DeleteGoombaAI(GoombaAITypeList[i], GoombaAIList[i].curr.x, GoombaAIList[i].curr.y);
					}
					break;
				case GoombaAIType::SHELL_MOVING:
				case GoombaAIType::GOOMBA:
				case GoombaAIType::KOOPA:
				case GoombaAIType::SPINY:
				default:
					break;
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
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIDisabledList[i] || GoombaAIAppearingList[i]) continue;
		//move
		if (GoombaAIDirectionList[i] == LEFT) GoombaAIList[i].curr = { GoombaAIList[i].curr.x - GoombaAIXveloList[i] * deltaTime, GoombaAIList[i].curr.y };
		else GoombaAIList[i].curr = { GoombaAIList[i].curr.x + GoombaAIXveloList[i] * deltaTime, GoombaAIList[i].curr.y };
		//shell moving break block
		if (GoombaAITypeList[i] == SHELL_MOVING) {
			float CusCurrPosX = 0, CusCurrPosY = 0;
			NoAdd = false;
			if (GoombaAIDirectionList[i] == RIGHT) {
				be = find_min_inx(GoombaAIList[i], BricksHorzPosList);
				nd = find_max_inx_dist(GoombaAIList[i], BricksHorzPosList, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
				BrickCollideRemove = isAccurateCollideSidet(GoombaAIList[i], GoombaAIList[i].curr, BricksHorzPosList, CusCurrPosX, CusCurrPosY, NoAdd, be, nd, 80.0f);
			}
			else if (GoombaAIDirectionList[i] == LEFT) {
				be = find_max_inx(GoombaAIList[i], BricksHorzPosList);
				nd = find_min_inx_dist(GoombaAIList[i], BricksHorzPosList, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
				BrickCollideRemove = isAccurateCollideSidet(GoombaAIList[i], GoombaAIList[i].curr, BricksHorzPosList, CusCurrPosX, CusCurrPosY, NoAdd, nd, be, 80.0f);
			}
			if (BrickCollideRemove.first || BrickCollideRemove.second) {
				GoombaAIDirectionList[i] = static_cast<GoombaAIDirection>(!GoombaAIDirectionList[i]);
				const BrickID CurrBrickID = GetIDBrick(CusCurrPosX, CusCurrPosY);
				switch (GetBrickAtt(CusCurrPosX, CusCurrPosY)) {
				case NORMAL:
					Sounds.PlaySound("Break");
					AddBrickParticle(CurrBrickID, CusCurrPosX, CusCurrPosY);
					DeleteBrick(CusCurrPosX, CusCurrPosY);
					Score += 50;
					break;
				case MULTICOIN:
					MultiBrickCoin(CusCurrPosX, CusCurrPosY - 32.0f, getBrickIndex(CusCurrPosX, CusCurrPosY));
				}
			}
			CusCurrPosX = 0, CusCurrPosY = 0;
			if (GoombaAIDirectionList[i] == RIGHT) {
				be = find_min_inx(GoombaAIList[i], LuckyHorzPosList);
				nd = find_max_inx_dist(GoombaAIList[i], LuckyHorzPosList, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
				LuckyCollideRemove = isAccurateCollideSidet(GoombaAIList[i], GoombaAIList[i].curr, LuckyHorzPosList, CusCurrPosX, CusCurrPosY, NoAdd, be, nd, 80.0f);
			}
			else if (GoombaAIDirectionList[i] == LEFT) {
				be = find_max_inx(GoombaAIList[i], LuckyHorzPosList);
				nd = find_min_inx_dist(GoombaAIList[i], LuckyHorzPosList, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
				LuckyCollideRemove = isAccurateCollideSidet(GoombaAIList[i], GoombaAIList[i].curr, LuckyHorzPosList, CusCurrPosX, CusCurrPosY, NoAdd, nd, be, 80.0f);
			}
			if (LuckyCollideRemove.first || LuckyCollideRemove.second) {
				GoombaAIDirectionList[i] = static_cast<GoombaAIDirection>(!GoombaAIDirectionList[i]);
				LuckyHit(CusCurrPosX, CusCurrPosY - 32.0f, getLuckyIndex(CusCurrPosX, CusCurrPosY));
			}
		}
		// Count if size AllCollideList equal to CollideAddCounter
		CurrPosXCollide = 0, CurrPosYCollide = 0;
		// 0 for right direction; 1 for left direction
		NoAdd = false;
		// Loop through obstacles
		if (GoombaAIDirectionList[i] == RIGHT) {
			be = find_min_inx(GoombaAIList[i], ObstaclesHorzPosList);
			nd = find_max_inx_dist(GoombaAIList[i], ObstaclesHorzPosList, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
			ObstacleCollide = isAccurateCollideSidet(GoombaAIList[i], GoombaAIList[i].curr, ObstaclesHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
			be = find_min_inx(GoombaAIList[i], BricksHorzPosList);
			nd = find_max_inx_dist(GoombaAIList[i], BricksHorzPosList, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
			BrickCollide = isAccurateCollideSidet(GoombaAIList[i], GoombaAIList[i].curr, BricksHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
			be = find_min_inx(GoombaAIList[i], LuckyHorzPosList);
			nd = find_max_inx_dist(GoombaAIList[i], LuckyHorzPosList, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
			LuckyCollide = isAccurateCollideSidet(GoombaAIList[i], GoombaAIList[i].curr, LuckyHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f);
		}
		else if (GoombaAIDirectionList[i] == LEFT) {
			be = find_max_inx(GoombaAIList[i], ObstaclesHorzPosList);
			nd = find_min_inx_dist(GoombaAIList[i], ObstaclesHorzPosList, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
			ObstacleCollide = isAccurateCollideSidet(GoombaAIList[i], GoombaAIList[i].curr, ObstaclesHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
			be = find_max_inx(GoombaAIList[i], BricksHorzPosList);
			nd = find_min_inx_dist(GoombaAIList[i], BricksHorzPosList, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
			BrickCollide = isAccurateCollideSidet(GoombaAIList[i], GoombaAIList[i].curr, BricksHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
			be = find_max_inx(GoombaAIList[i], LuckyHorzPosList);
			nd = find_min_inx_dist(GoombaAIList[i], LuckyHorzPosList, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
			LuckyCollide = isAccurateCollideSidet(GoombaAIList[i], GoombaAIList[i].curr, LuckyHorzPosList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		}

		// Stop on wall
		// Adjust Position if collide
		if (ObstacleCollide.first || BrickCollide.first || LuckyCollide.first) {
			GoombaAIDirectionList[i] = static_cast<GoombaAIDirection>(!GoombaAIDirectionList[i]);
			GoombaAIList[i].curr = { CurrPosXCollide + 32.0f - GoombaAIDefinationList[i][2] + GoombaAIList[i].property.getOrigin().x, GoombaAIList[i].curr.y };
		}
		if (ObstacleCollide.second || BrickCollide.second || LuckyCollide.second) {
			GoombaAIDirectionList[i] = static_cast<GoombaAIDirection>(!GoombaAIDirectionList[i]);
			GoombaAIList[i].curr = { CurrPosXCollide - (1.0f + GoombaAIDefinationList[i][2] + (GoombaAIDefinationList[i][0] - GoombaAIList[i].property.getOrigin().x)), GoombaAIList[i].curr.y };
		}
	}
}
void GoombaAIVertYUpdate(const float deltaTime) {
	bool isLanding;
	float CurrPosYCollide;
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIDisabledList[i] || GoombaAIAppearingList[i]) continue;
		// bottom update

		GoombaAIYveloList[i] += (GoombaAIYveloList[i] >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f);
		GoombaAIList[i].curr = { GoombaAIList[i].curr.x,GoombaAIList[i].curr.y + GoombaAIYveloList[i] * deltaTime };
		GoombaAIYveloList[i] += (GoombaAIYveloList[i] >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f);
		//}

		bool NoAdd = false;
		int be = find_min_iny(GoombaAIList[i], ObstaclesVertPosList);
		int nd = find_max_iny_dist(GoombaAIList[i], ObstaclesVertPosList, 64.0f + (GoombaAIYveloList[i]) * 16.0f);
		bool ObstacleCollide = isAccurateCollideBott(GoombaAIList[i], GoombaAIList[i].curr, ObstaclesVertPosList,
		                                             CurrPosYCollide, NoAdd, be, nd, 80.0f);
		be = find_min_iny(GoombaAIList[i], BricksVertPosList);
		nd = find_max_iny_dist(GoombaAIList[i], BricksVertPosList, 64.0f + (GoombaAIYveloList[i]) * 16.0f);
		bool BrickCollide = isAccurateCollideBott(GoombaAIList[i], GoombaAIList[i].curr, BricksVertPosList, CurrPosYCollide,
		                                          NoAdd, be, nd, 80.0f);
		be = find_min_iny(GoombaAIList[i], LuckyVertPosList);
		nd = find_max_iny_dist(GoombaAIList[i], LuckyVertPosList, 64.0f + (GoombaAIYveloList[i]) * 16.0f);
		bool LuckyCollide = isAccurateCollideBott(GoombaAIList[i], GoombaAIList[i].curr, LuckyVertPosList, CurrPosYCollide,
		                                          NoAdd, be, nd, 80.0f);
		if (ObstacleCollide || BrickCollide || LuckyCollide) {
			if (GoombaAIYveloList[i] >= 0.0f) {
				GoombaAIYveloList[i] = 0.0f;
				isLanding = true;
			}
			else {
				GoombaAIYveloList[i] = 0.0f;
				isLanding = false;
			}
			//recolide
			if (isLanding) {
				GoombaAIList[i].curr = { GoombaAIList[i].curr.x, CurrPosYCollide - (GoombaAIDefinationList[i][1] - GoombaAIList[i].property.getOrigin().y + GoombaAIDefinationList[i][3]) };
			}
		}
		// top update
		NoAdd = false;
		be = find_max_iny(GoombaAIList[i], ObstaclesVertPosList);
		nd = find_min_iny_dist(GoombaAIList[i], ObstaclesVertPosList, 64.0f - (GoombaAIYveloList[i]) * 16.0f);
		ObstacleCollide = isAccurateCollideTopt(GoombaAIList[i], GoombaAIList[i].curr, ObstaclesVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		be = find_max_iny(GoombaAIList[i], BricksVertPosList);
		nd = find_min_iny_dist(GoombaAIList[i], BricksVertPosList, 64.0f - (GoombaAIYveloList[i]) * 16.0f);
		BrickCollide = isAccurateCollideTopt(GoombaAIList[i], GoombaAIList[i].curr, BricksVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		be = find_max_iny(GoombaAIList[i], LuckyVertPosList);
		nd = find_min_iny_dist(GoombaAIList[i], LuckyVertPosList, 64.0f - (GoombaAIYveloList[i]) * 16.0f);
		LuckyCollide = isAccurateCollideTopt(GoombaAIList[i], GoombaAIList[i].curr, LuckyVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		if ((ObstacleCollide || BrickCollide || LuckyCollide) && GoombaAIYveloList[i] < 0.0f) {
			GoombaAIYveloList[i] = 0.0f;
			GoombaAIList[i].curr = { GoombaAIList[i].curr.x, CurrPosYCollide + (31.0f + GoombaAIList[i].property.getOrigin().y - GoombaAIDefinationList[i][3]) };
		}
	}
}
static void Kicking(const int i, const int j) {
	switch (GoombaAIShellHitCount[i]) {
	case 0:
		Sounds.PlaySound("Kick2");
		AddScoreEffect(SCORE_100, GoombaAIList[j].curr.x, GoombaAIList[j].curr.y - GoombaAIHitboxList[j].second);
		++GoombaAIShellHitCount[i];
		break;
	case 1:
		Sounds.PlaySound("Kick3");
		AddScoreEffect(SCORE_200, GoombaAIList[j].curr.x, GoombaAIList[j].curr.y - GoombaAIHitboxList[j].second);
		++GoombaAIShellHitCount[i];
		break;
	case 2:
		Sounds.PlaySound("Kick4");
		AddScoreEffect(SCORE_500, GoombaAIList[j].curr.x, GoombaAIList[j].curr.y - GoombaAIHitboxList[j].second);
		++GoombaAIShellHitCount[i];
		break;
	case 3:
		Sounds.PlaySound("Kick5");
		AddScoreEffect(SCORE_1000, GoombaAIList[j].curr.x, GoombaAIList[j].curr.y - GoombaAIHitboxList[j].second);
		++GoombaAIShellHitCount[i];
		break;
	case 4:
		Sounds.PlaySound("Kick6");
		AddScoreEffect(SCORE_2000, GoombaAIList[j].curr.x, GoombaAIList[j].curr.y - GoombaAIHitboxList[j].second);
		++GoombaAIShellHitCount[i];
		break;
	case 5:
		Sounds.PlaySound("Kick7");
		AddScoreEffect(SCORE_5000, GoombaAIList[j].curr.x, GoombaAIList[j].curr.y - GoombaAIHitboxList[j].second);
		++GoombaAIShellHitCount[i];
		break;
	case 6:
		Sounds.PlaySound("Kick8");
		AddScoreEffect(SCORE_1UP, GoombaAIList[j].curr.x, GoombaAIList[j].curr.y - GoombaAIHitboxList[j].second);
		GoombaAIShellHitCount[i] = 0;
		break;
	default: ;
	}
}
void GoombaAICollisionUpdate() {
	std::set<std::pair<GoombaAIType, std::pair<float, float>>> GoombaAIDeleteSet;
	std::set<int> coll_set;
	bool flag = false;
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIAppearingList[i]) continue;
		else if (GoombaAITypeList[i] == GoombaAIType::MUSHROOM) continue;
		const sf::FloatRect hitbox_loop = getGlobalHitbox(GoombaAIList[i].hitboxMain, GoombaAIList[i].curr,
		                                            GoombaAIList[i].property.getOrigin());
		flag = false;
		for (int j = 0; j < GoombaAIList.size(); ++j) {
			if (f_abs(GoombaAIList[i].curr.x - GoombaAIList[j].curr.x) >= 160.0f) continue;
			if (GoombaAIAppearingList[j] || i == j) continue;
			else if (GoombaAITypeList[j] == GoombaAIType::MUSHROOM) continue;
			if (const sf::FloatRect GoombaAIHitbox = getGlobalHitbox(GoombaAIList[j].hitboxMain, GoombaAIList[j].curr, GoombaAIList[j].property.getOrigin()); isCollide(GoombaAIHitbox, hitbox_loop)) {
				if (GoombaAITypeList[j] != SHELL_MOVING && GoombaAITypeList[i] != SHELL_MOVING) {
					if ((!GoombaAICollisionList[i].first && !GoombaAICollisionList[j].first) || (GoombaAICollisionList[i].second != j && GoombaAICollisionList[i].first)) {
						if (GoombaAIDirectionList[i] == GoombaAIDirectionList[j] && (GoombaAIXveloList[i] > 0.0f && GoombaAIXveloList[j] > 0.0f)) continue;
						if (GoombaAIDirectionList[i] == RIGHT && GoombaAIDirectionList[j] == LEFT) continue;

						if (!GoombaAIDisabledList[j]) {
							GoombaAICollisionList[i].first = true;
							GoombaAICollisionList[i].second = j;
							flag = true;

							coll_set.insert(i);
							coll_set.insert(j);
						}
						else {
							coll_set.insert(i);
						}
						break;
					}
					else if (GoombaAICollisionList[i].first && GoombaAICollisionList[i].second == j) {
						flag = true;
						break;
					}
					else continue;
				}
				else if (GoombaAITypeList[i] == SHELL_MOVING && GoombaAITypeList[j] != SHELL_MOVING) {
					Kicking(i, j);
					AddGoombaAIEffect(GoombaAITypeList[j], NONE, GoombaAISkinIDList[j], GoombaAIList[j].curr.x, GoombaAIList[j].curr.y);
					GoombaAIDeleteSet.insert({ GoombaAITypeList[j], { GoombaAIList[j].curr.x , GoombaAIList[j].curr.y } });
					break;
				}
				else if (GoombaAITypeList[i] == SHELL_MOVING && GoombaAITypeList[j] == SHELL_MOVING) {
					Sounds.PlaySound("Kick2");
					AddScoreEffect(SCORE_100, GoombaAIList[j].curr.x, GoombaAIList[j].curr.y - GoombaAIHitboxList[j].second);
					AddScoreEffect(SCORE_100, GoombaAIList[i].curr.x, GoombaAIList[i].curr.y - GoombaAIHitboxList[i].second);
					AddGoombaAIEffect(GoombaAITypeList[i], NONE, GoombaAISkinIDList[i], GoombaAIList[i].curr.x, GoombaAIList[i].curr.y);
					GoombaAIDeleteSet.insert({ GoombaAITypeList[i],  { GoombaAIList[i].curr.x , GoombaAIList[i].curr.y } });
					AddGoombaAIEffect(GoombaAITypeList[j], NONE, GoombaAISkinIDList[j], GoombaAIList[j].curr.x, GoombaAIList[j].curr.y);
					GoombaAIDeleteSet.insert({ GoombaAITypeList[j], { GoombaAIList[j].curr.x , GoombaAIList[j].curr.y } });
					break;
				}
			}
		}
		if (!flag) {
			GoombaAICollisionList[i].first = false;
			if (GoombaAICollisionList[i].second != -1) GoombaAICollisionList[i].second = -1;
		}
	}
	for (const auto& i : coll_set) {
		if (GoombaAIDirectionList[i] == LEFT) GoombaAIDirectionList[i] = RIGHT;
		else GoombaAIDirectionList[i] = LEFT;
	}
	for (const auto&[fst, snd] : GoombaAIDeleteSet) {
		DeleteGoombaAI(fst, snd.first, snd.second);
	}
}
void GoombaAIUpdate() {
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		//if (GoombaAIDirectionList[i] == LEFT) GoombaAIList[i].property.setColor(sf::Color(255, 0, 0));
		//else GoombaAIList[i].property.setColor(sf::Color(0, 0, 255));
		if (!isOutScreen(GoombaAIList[i].property.getPosition().x - GoombaAIList[i].property.getOrigin().x, GoombaAIList[i].property.getPosition().y, 32, 80) && !GoombaAIDisabledList[i]) {
			if (GoombaAIDirectionList[i] == RIGHT) {
				GoombaAIAnimationList[i].first.update(GoombaAIList[i].property);

				GoombaAIAnimationList[i].second.silentupdate();

				if (GoombaAIList[i].property.getOrigin() != GoombaAIOriginList[i].first) GoombaAIList[i].property.setOrigin(GoombaAIOriginList[i].first);
			}
			else if (GoombaAIDirectionList[i] == LEFT) {
				GoombaAIAnimationList[i].second.update(GoombaAIList[i].property);

				GoombaAIAnimationList[i].first.silentupdate();

				if (GoombaAIList[i].property.getOrigin() != GoombaAIOriginList[i].second) GoombaAIList[i].property.setOrigin(GoombaAIOriginList[i].second);
			}
			rObject.draw(GoombaAIList[i].property);
		}
		else if (isOutScreen(GoombaAIList[i].property.getPosition().x - GoombaAIList[i].property.getOrigin().x, GoombaAIList[i].property.getPosition().y, 32, 80) && !GoombaAIDisabledList[i]) {
			GoombaAIAnimationList[i].second.silentupdate();
			GoombaAIAnimationList[i].first.silentupdate();
		}
	}
}