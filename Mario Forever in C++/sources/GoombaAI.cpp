#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <array>
#include <set>

#include "../headers/Object/GoombaAI.hpp"
#include "../headers/Core/Loading/enum.hpp"
#include "../headers/Core/Loading/Loading.hpp"
#include "../headers/Core/Scroll.hpp"
#include "../headers/Block/Obstacles.hpp"
#include "../headers/Block/Brick.hpp"
#include "../headers/Block/LuckyBlock.hpp"
#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Core/Animate/LocalAnimationManager.hpp"
#include "../headers/Effect/ScoreEffect.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Effect/GoombaAIEffect.hpp"
#include "../headers/Object/Mario.hpp"
#include "../headers/Core/Sound.hpp"
#include "../headers/Core/Level.hpp"
#include "../headers/Core/TextureManager.hpp"
#include "../headers/Effect/BrickParticle.hpp"
#include "../headers/Effect/MarioEffect.hpp"

#include "../resource.h"

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
std::vector<std::pair<std::string, std::string>> GoombaAITextureNameList;
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
	GoombaAITextureManager.LoadingAnimatedTexture(GOOMBA_TEXTURE, "Goomba", 0, 1, 0, 31, 32);
	GoombaAITextureManager.LoadingAnimatedTexture(MUSHROOM_TEXTURE, "Mushroom", 0, 0, 0, 31, 32);
	GoombaAITextureManager.LoadingAnimatedTexture(GREEN_KOOPA_TEXTURE, "Koopa_right_green", 0, 1, 0, 32, 47);
	GoombaAITextureManager.LoadingAnimatedTexture(GREEN_KOOPA_TEXTURE, "Koopa_left_green", 0, 1, 1, 32, 47);
	GoombaAITextureManager.LoadingAnimatedTexture(GREEN_KOOPA_SHELL_TEXTURE, "Koopa_Shell_green_idle", 3, 3, 0, 33, 28);
	GoombaAITextureManager.LoadingAnimatedTexture(GREEN_KOOPA_SHELL_TEXTURE, "Koopa_Shell_green_moving", 0, 3, 0, 33, 28);
	GoombaAITextureManager.LoadingAnimatedTexture(RED_SPINY_TEXTURE, "Spiny_right_red", 0, 1, 0, 33, 32);
	GoombaAITextureManager.LoadingAnimatedTexture(RED_SPINY_TEXTURE, "Spiny_left_red", 0, 1, 1, 33, 32);
}
void AddGoombaAI(GoombaAIType type, int SkinID, float x, float y, GoombaAIDirection Dir = LEFT) {
	MovableObject Init;
	LocalAnimationManager in, inri;
	switch (type) {
	case GOOMBA:
		Init.property.setOrigin({ 15, 31 });
		GoombaAIDefinationList.push_back({ 31.0f, 32.0f, 0.0f, 0.0f });
		GoombaAIHitboxList.push_back({ 31, 32 });
		in.setAnimation(0, 1, 11);
		inri.setAnimation(0, 1, 11);
		GoombaAITextureNameList.push_back({ "Goomba", "Goomba" });
		GoombaAIXveloList.push_back(1.0f);
		GoombaAIHittableList.push_back(YES);
		GoombaAIAppearingList.push_back(false);
		GoombaAIOriginList.push_back({ sf::Vector2f(15, 31), sf::Vector2f(15, 31) });
		GoombaAIInvincibleSecondLimitList.push_back(0.0f);
		break;
	case KOOPA:
		Init.property.setOrigin({ 13, 44 });
		GoombaAIDefinationList.push_back({ 32.0f, 47.0f, 0.0f, 0.0f });
		GoombaAIHitboxList.push_back({ 32, 47 });
		in.setAnimation(0, 1, 12);
		inri.setAnimation(0, 1, 12);
		GoombaAITextureNameList.push_back({ "Koopa_right_green", "Koopa_left_green" });
		GoombaAIXveloList.push_back(1.0f);
		GoombaAIHittableList.push_back(YES);
		GoombaAIAppearingList.push_back(false);
		GoombaAIOriginList.push_back({ sf::Vector2f(13, 44), sf::Vector2f(18, 44) });
		GoombaAIInvincibleSecondLimitList.push_back(0.0f);
		break;
	case MUSHROOM:
		Init.property.setOrigin({ 16, 31 });
		GoombaAIDefinationList.push_back({ 31.0f, 32.0f, 0.0f, 0.0f });
		GoombaAIHitboxList.push_back({ 31, 32 });
		in.setAnimation(0, 0, 100);
		inri.setAnimation(0, 0, 100);
		GoombaAITextureNameList.push_back({ "Mushroom", "Mushroom" });
		GoombaAIXveloList.push_back(2.0f);
		GoombaAIHittableList.push_back(FULL);
		GoombaAIAppearingList.push_back(true);
		GoombaAIOriginList.push_back({ sf::Vector2f(15, 31), sf::Vector2f(15, 31) });
		GoombaAIInvincibleSecondLimitList.push_back(0.0f);
		break;
	case SHELL:
		Init.property.setOrigin({ 16, 27 });
		GoombaAIDefinationList.push_back({ 33.0f, 28.0f, 0.0f, 0.0f });
		GoombaAIHitboxList.push_back({ 33, 28 });
		in.setAnimation(0, 0, 100);
		inri.setAnimation(0, 0, 100);
		GoombaAITextureNameList.push_back({ "Koopa_Shell_green_idle", "Koopa_Shell_green_idle" });
		GoombaAIXveloList.push_back(0.0f);
		GoombaAIHittableList.push_back(FULL);
		GoombaAIAppearingList.push_back(false);
		GoombaAIOriginList.push_back({ sf::Vector2f(16, 27), sf::Vector2f(16, 27) });
		GoombaAIInvincibleSecondLimitList.push_back(0.12f);
		break;
	case SHELL_MOVING:
		Init.property.setOrigin({ 16, 27 });
		GoombaAIDefinationList.push_back({ 33.0f, 28.0f, 0.0f, 0.0f });
		GoombaAIHitboxList.push_back({ 33, 28 });
		in.setAnimation(0, 3, 54);
		inri.setAnimation(0, 3, 54);
		GoombaAITextureNameList.push_back({ "Koopa_Shell_green_moving", "Koopa_Shell_green_moving" });
		GoombaAIXveloList.push_back(5.0f);
		GoombaAIHittableList.push_back(YES);
		GoombaAIAppearingList.push_back(false);
		GoombaAIOriginList.push_back({ sf::Vector2f(16, 27), sf::Vector2f(16, 27) });
		GoombaAIInvincibleSecondLimitList.push_back(0.6f);
		break;
	case SPINY:
		Init.property.setOrigin({ 16, 29 });
		GoombaAIDefinationList.push_back({ 33.0f, 32.0f, 0.0f, 0.0f });
		GoombaAIHitboxList.push_back({ 33, 32 });
		in.setAnimation(0, 1, 14);
		inri.setAnimation(0, 1, 14);
		GoombaAITextureNameList.push_back({ "Spiny_right_red", "Spiny_left_red" });
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
	setHitbox(Init.hitboxRight2, sf::FloatRect({ GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first - 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3] }, { 3.0f, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 9.0f }));
	setHitbox(Init.hitboxLeft2, sf::FloatRect({ -1.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3] }, { 3.0f, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 9.0f }));
	setHitbox(Init.hitboxRight, sf::FloatRect({ GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first - 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3] }, { 2.0f, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 9.0f }));
	setHitbox(Init.hitboxLeft, sf::FloatRect({ 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3] }, { 2.0f, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 9.0f }));
	Init.property.setPosition({ x, y });
	GoombaAIAnimationList.push_back({ in, inri });
	GoombaAIList.push_back(Init);
	GoombaAIDirectionList.push_back(Dir);
	GoombaAITypeList.push_back(type);
	GoombaAIYveloList.push_back(0.0f);
	GoombaAIDisabledList.push_back(true);
	GoombaAIAppearingYList.push_back(0.0f);
	GoombaAISkinIDList.push_back(SkinID);
	GoombaAICollisionList.push_back({ false, -1 });
}
void DeleteGoombaAI(GoombaAIType type, float x, float y) {
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIList[i].property.getPosition().x == x && GoombaAIList[i].property.getPosition().y == y && GoombaAITypeList[i] == type) {
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
			GoombaAITextureNameList.erase(GoombaAITextureNameList.begin() + i);
			GoombaAIOriginList.erase(GoombaAIOriginList.begin() + i);
			GoombaAIInvincibleTimerList.erase(GoombaAIInvincibleTimerList.begin() + i);
			GoombaAIInvincibleSecondLimitList.erase(GoombaAIInvincibleSecondLimitList.begin() + i);
			GoombaAIShellHitCount.erase(GoombaAIShellHitCount.begin() + i);
			GoombaAICollisionList.erase(GoombaAICollisionList.begin() + i);
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
	GoombaAITextureNameList.clear();
	GoombaAIOriginList.clear();
	GoombaAIInvincibleTimerList.clear();
	GoombaAIInvincibleSecondLimitList.clear();
	GoombaAIShellHitCount.clear();
	GoombaAICollisionList.clear();
}
void GoombaStatusUpdate() {
	std::vector<sf::Vector2f> GoombaAIDeletionPositionList;
	std::vector<GoombaAIType> GoombaAIDeletionTypeList;
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (isOutScreenYBottom(GoombaAIList[i].property.getPosition().y, 80)) {
			GoombaAIDeletionPositionList.push_back(GoombaAIList[i].property.getPosition());
			GoombaAIDeletionTypeList.push_back(GoombaAITypeList[i]);
		}
		if (!isOutScreen(GoombaAIList[i].property.getPosition().x - GoombaAIList[i].property.getOrigin().x, GoombaAIList[i].property.getPosition().y, 32, 80)) {
			if (GoombaAIDisabledList[i]) GoombaAIDisabledList[i] = false;
		}
		else continue;
		if (GoombaAIAppearingList[i] && !GoombaAIDisabledList[i]) {
			GoombaAIList[i].property.move({ 0.0f, -0.5f * deltaTime });
			GoombaAIAppearingYList[i] += 0.5f * deltaTime;
			if (GoombaAIAppearingYList[i] >= 32.0f) GoombaAIAppearingList[i] = false;
		}
	}
	if (GoombaAIDeletionPositionList.size() == 0) return;
	for (int i = 0; i < GoombaAIDeletionPositionList.size(); ++i) DeleteGoombaAI(GoombaAIDeletionTypeList[i], GoombaAIDeletionPositionList[i].x, GoombaAIDeletionPositionList[i].y);
}
void GoombaAICheckCollide() {
	if (EffectActive) return;
	sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.property);
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIDisabledList[i] || GoombaAIAppearingList[i]) continue;
		if (f_abs(player.property.getPosition().x - GoombaAIList[i].property.getPosition().x) >= 160.0f) continue;
		if (isCollide(GoombaAIList[i].hitboxMain, GoombaAIList[i].property, hitbox_mario)) {
			if (GoombaAIHittableList[i] == YES) {
				if ((GoombaAIInvincibleTimerList[i].getElapsedTime().asSeconds() >= GoombaAIInvincibleSecondLimitList[i] && GoombaAIInvincibleSecondLimitList[i] > 0.0f) || GoombaAIInvincibleSecondLimitList[i] == 0.0f) {
					if (((GoombaAIList[i].property.getPosition().y - 16.0f) > player.property.getPosition().y) && Yvelo > 0.0f && !GoombaAIDisabledList[i]) {
						if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z)) Yvelo = -8.0f;
						else Yvelo = -13.0f;
						Sounds.PlaySound("Stomp");
						switch (GoombaAITypeList[i]) {
						case GOOMBA:
							AddScoreEffect(SCORE_100, GoombaAIList[i].property.getPosition().x - 15.0f, GoombaAIList[i].property.getPosition().y - GoombaAIHitboxList[0].second);
							AddGoombaAIEffect(GoombaAITypeList[i], COLLIDE, GoombaAISkinIDList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y);
							break;
						case KOOPA:
							AddScoreEffect(SCORE_100, GoombaAIList[i].property.getPosition().x - 13.0f, GoombaAIList[i].property.getPosition().y - GoombaAIHitboxList[0].second);
							AddGoombaAI(SHELL, GoombaAISkinIDList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y + 3.0f);
							break;
						case SHELL_MOVING:
							GoombaAIShellHitCount[i] = 0;
							AddScoreEffect(SCORE_100, GoombaAIList[i].property.getPosition().x - 13.0f, GoombaAIList[i].property.getPosition().y - GoombaAIHitboxList[0].second);
							AddGoombaAI(SHELL, GoombaAISkinIDList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y);
							break;
						}
						DeleteGoombaAI(GoombaAITypeList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y);
						break;
					}
				}
			}
			if (GoombaAIHittableList[i] == YES || GoombaAIHittableList[i] == NO) {
				if ((GoombaAIInvincibleTimerList[i].getElapsedTime().asSeconds() > GoombaAIInvincibleSecondLimitList[i] && GoombaAIInvincibleSecondLimitList[i] > 0.0f) || GoombaAIInvincibleSecondLimitList[i] == 0.0f) {
					if ((GoombaAIList[i].property.getPosition().y - 16.0f) < player.property.getPosition().y) {
						PowerDown();
						break;
					}
				}
			}
			else if (GoombaAIHittableList[i] == FULL) {
				switch (GoombaAITypeList[i]) {
				case MUSHROOM:
					Sounds.PlaySound("Powerup");
					if (GoombaAITypeList[i] == MUSHROOM) AddScoreEffect(SCORE_1000, GoombaAIList[i].property.getPosition().x - 15.0f, GoombaAIList[i].property.getPosition().y - GoombaAIHitboxList[0].second);
					if (PowerState == 0) PowerState = 1;
					DeleteGoombaAI(GoombaAITypeList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y);
					break;
				case SHELL:
					if ((GoombaAIInvincibleTimerList[i].getElapsedTime().asSeconds() > GoombaAIInvincibleSecondLimitList[i] && GoombaAIInvincibleSecondLimitList[i] > 0.0f) || GoombaAIInvincibleSecondLimitList[i] == 0.0f) {
						Sounds.PlaySound("Kick2");
						if (GoombaAIList[i].property.getPosition().x >= player.property.getPosition().x) AddGoombaAI(SHELL_MOVING, GoombaAISkinIDList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y, RIGHT);
						else AddGoombaAI(SHELL_MOVING, GoombaAISkinIDList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y, LEFT);
						DeleteGoombaAI(GoombaAITypeList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y);
					}
					break;
				}
				break;
			}
		}
	}
}
void GoombaAIVertXUpdate() {
	sf::FloatRect hitbox_loop;
	bool isCollideLeftBool, isCollideRightBool;
	bool NoAdd, isCollideSide;
	float CurrPosXCollide, CurrPosYCollide;
	std::pair<bool, bool> ObstacleCheck, BrickCheck, LuckyCheck, BrickCollideRemove, LuckyCollideRemove;
	std::pair<bool, bool> ObstacleCollide, BrickCollide, LuckyCollide;
	int be, nd;
	// Check if a GoombaAI collide with left or right
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIDisabledList[i] || GoombaAIAppearingList[i]) continue;
		//move
		if (GoombaAIDirectionList[i] == LEFT) GoombaAIList[i].property.move({ -GoombaAIXveloList[i] * deltaTime, 0.0f });
		else GoombaAIList[i].property.move({ GoombaAIXveloList[i] * deltaTime, 0.0f });
		//check collide
		//ObstacleCheck = isOrCollideSide(GoombaAIList[i], ObstaclesList, {});
		//BrickCheck = isOrCollideSide(GoombaAIList[i], Bricks, BrickSaveList);
		//LuckyCheck = isOrCollideSide(GoombaAIList[i], LuckyBlock, LuckyBlockSaveList);
		bool isTrueCollide = false;
		//if (ObstacleCheck.first || ObstacleCheck.second || BrickCheck.first || BrickCheck.second || LuckyCheck.first || LuckyCheck.second) {
			//shell moving break block
		if (GoombaAITypeList[i] == SHELL_MOVING) {
			float BrickCurrPosX = 0, BrickCurrPosY = 0;
			if (GoombaAIDirectionList[i] == RIGHT) {
				be = find_min_inx(GoombaAIList[i], Bricks);
				nd = find_max_inx_dist(GoombaAIList[i], Bricks, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
				BrickCollideRemove = isAccurateCollideSide2(GoombaAIList[i], Bricks, BrickCurrPosX, BrickCurrPosY, be, nd, BrickSaveList);
			}
			else if (GoombaAIDirectionList[i] == LEFT) {
				be = find_max_inx(GoombaAIList[i], Bricks);
				nd = find_min_inx_dist(GoombaAIList[i], Bricks, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
				BrickCollideRemove = isAccurateCollideSide2(GoombaAIList[i], Bricks, BrickCurrPosX, BrickCurrPosY, nd, be, BrickSaveList);
			}
			if (BrickCollideRemove.first || BrickCollideRemove.second) {
				if (GoombaAIDirectionList[i] == RIGHT) GoombaAIDirectionList[i] = LEFT;
				else GoombaAIDirectionList[i] = RIGHT;
				BrickID CurrBrickID = GetIDBrick(BrickCurrPosX, BrickCurrPosY);
				BrickAtt CurrBrickAtt = GetBrickAtt(BrickCurrPosX, BrickCurrPosY);
				switch (CurrBrickAtt) {
				case NORMAL:
					Sounds.PlaySound("Break");
					AddBrickParticle(CurrBrickID, BrickCurrPosX, BrickCurrPosY);
					DeleteBrick(BrickCurrPosX, BrickCurrPosY);
					Score += 50;
					break;
				case MULTICOIN:
					MultiBrickCoin(BrickCurrPosX, BrickCurrPosY - 32.0f, getBrickIndex(BrickCurrPosX, BrickCurrPosY));
				}
			}
			float LuckyCurrPosX = 0, LuckyCurrPosY = 0;
			if (GoombaAIDirectionList[i] == RIGHT) {
				be = find_min_inx(GoombaAIList[i], LuckyBlock);
				nd = find_max_inx_dist(GoombaAIList[i], LuckyBlock, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
				LuckyCollideRemove = isAccurateCollideSide2(GoombaAIList[i], LuckyBlock, LuckyCurrPosX, LuckyCurrPosY, be, nd, LuckyBlockSaveList);
			}
			else if (GoombaAIDirectionList[i] == LEFT) {
				be = find_max_inx(GoombaAIList[i], LuckyBlock);
				nd = find_min_inx_dist(GoombaAIList[i], LuckyBlock, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
				LuckyCollideRemove = isAccurateCollideSide2(GoombaAIList[i], LuckyBlock, LuckyCurrPosX, LuckyCurrPosY, nd, be, LuckyBlockSaveList);
			}
			if (LuckyCollideRemove.first || LuckyCollideRemove.second) {
				if (GoombaAIDirectionList[i] == RIGHT) GoombaAIDirectionList[i] = LEFT;
				else GoombaAIDirectionList[i] = RIGHT;
				LuckyHit(LuckyCurrPosX, LuckyCurrPosY - 32.0f, getLuckyIndex(LuckyCurrPosX, LuckyCurrPosY));
			}
		}
		// Count if size AllCollideList equal to CollideAddCounter
		CurrPosXCollide = 0, CurrPosYCollide = 0;
		isCollideSide = false;
		// 0 for right; 1 for left
		NoAdd = false;
		isCollideLeftBool = false;
		isCollideRightBool = false;
		// Loop through obstacles
		if (GoombaAIDirectionList[i] == RIGHT) {
			be = find_min_inx(GoombaAIList[i], ObstaclesList);
			nd = find_max_inx_dist(GoombaAIList[i], ObstaclesList, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
			ObstacleCollide = isAccurateCollideSidet(GoombaAIList[i], ObstaclesList, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f, {});
			be = find_min_inx(GoombaAIList[i], Bricks);
			nd = find_max_inx_dist(GoombaAIList[i], Bricks, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
			BrickCollide = isAccurateCollideSidet(GoombaAIList[i], Bricks, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f, BrickSaveList);
			be = find_min_inx(GoombaAIList[i], LuckyBlock);
			nd = find_max_inx_dist(GoombaAIList[i], LuckyBlock, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
			LuckyCollide = isAccurateCollideSidet(GoombaAIList[i], LuckyBlock, CurrPosXCollide, CurrPosYCollide, NoAdd, be, nd, 80.0f, LuckyBlockSaveList);
		}
		else if (GoombaAIDirectionList[i] == LEFT) {
			be = find_max_inx(GoombaAIList[i], ObstaclesList);
			nd = find_min_inx_dist(GoombaAIList[i], ObstaclesList, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
			ObstacleCollide = isAccurateCollideSidet(GoombaAIList[i], ObstaclesList, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f, {});
			be = find_max_inx(GoombaAIList[i], Bricks);
			nd = find_min_inx_dist(GoombaAIList[i], Bricks, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
			BrickCollide = isAccurateCollideSidet(GoombaAIList[i], Bricks, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f, BrickSaveList);
			be = find_max_inx(GoombaAIList[i], LuckyBlock);
			nd = find_min_inx_dist(GoombaAIList[i], LuckyBlock, 64.0f + (GoombaAIXveloList[i]) * 16.0f);
			LuckyCollide = isAccurateCollideSidet(GoombaAIList[i], LuckyBlock, CurrPosXCollide, CurrPosYCollide, NoAdd, nd, be, 80.0f, LuckyBlockSaveList);
		}

		// Stop on wall
		if ((ObstacleCollide.second && GoombaAIDirectionList[i] == RIGHT) || (ObstacleCollide.first && GoombaAIDirectionList[i] == LEFT)) {
			if (GoombaAIDirectionList[i] == RIGHT) GoombaAIDirectionList[i] = LEFT;
			else GoombaAIDirectionList[i] = RIGHT;
		}
		if ((BrickCollide.second && GoombaAIDirectionList[i] == RIGHT) || (BrickCollide.first && GoombaAIDirectionList[i] == LEFT)) {
			if (GoombaAIDirectionList[i] == RIGHT) GoombaAIDirectionList[i] = LEFT;
			else GoombaAIDirectionList[i] = RIGHT;
		}
		if ((LuckyCollide.second && GoombaAIDirectionList[i] == RIGHT) || (LuckyCollide.first && GoombaAIDirectionList[i] == LEFT)) {
			if (GoombaAIDirectionList[i] == RIGHT) GoombaAIDirectionList[i] = LEFT;
			else GoombaAIDirectionList[i] = RIGHT;
		}

		// Break if size AllCollideList is not equal to CollideAddCounter
		//if (AllCollideList.size() != CollideAddCounter) break;
		// Adjust Position if collide
		if (ObstacleCollide.first) GoombaAIList[i].property.setPosition({ CurrPosXCollide + 32.0f - GoombaAIDefinationList[i][2] + GoombaAIList[i].property.getOrigin().x, GoombaAIList[i].property.getPosition().y });
		if (ObstacleCollide.second) GoombaAIList[i].property.setPosition({ CurrPosXCollide - (1.0f + GoombaAIDefinationList[i][2] + (GoombaAIDefinationList[i][0] - GoombaAIList[i].property.getOrigin().x)), GoombaAIList[i].property.getPosition().y });
		if (BrickCollide.first) GoombaAIList[i].property.setPosition({ CurrPosXCollide + 32.0f - GoombaAIDefinationList[i][2] + GoombaAIList[i].property.getOrigin().x, GoombaAIList[i].property.getPosition().y });
		if (BrickCollide.second) GoombaAIList[i].property.setPosition({ CurrPosXCollide - (1.0f + GoombaAIDefinationList[i][2] + (GoombaAIDefinationList[i][0] - GoombaAIList[i].property.getOrigin().x)), GoombaAIList[i].property.getPosition().y });
		if (LuckyCollide.first) GoombaAIList[i].property.setPosition({ CurrPosXCollide + 32.0f - GoombaAIDefinationList[i][2] + GoombaAIList[i].property.getOrigin().x, GoombaAIList[i].property.getPosition().y });
		if (LuckyCollide.second) GoombaAIList[i].property.setPosition({ CurrPosXCollide - (1.0f + GoombaAIDefinationList[i][2] + (GoombaAIDefinationList[i][0] - GoombaAIList[i].property.getOrigin().x)), GoombaAIList[i].property.getPosition().y });

		//if (!ObstacleCollide.first && !ObstacleCollide.second && !BrickCollide.first && !BrickCollide.second && !LuckyCollide.first && !LuckyCollide.second) break;
		// Break if no collide
	//}
	}
}
void GoombaAIVertYUpdate() {
	std::pair<bool, bool> ObstacleCollide, BrickCollide, LuckyCollide;
	bool ObstacleCheck, BrickCheck, LuckyCheck, isLanding;
	float CurrPosYCollide;
	bool NoAdd;
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIDisabledList[i] || GoombaAIAppearingList[i]) continue;
		// bottom update

		//ObstacleCheck = isCollideBot(GoombaAIList[i], ObstaclesList);
		//BrickCheck = isCollideBot(GoombaAIList[i], Bricks);
		//LuckyCheck = isCollideBot(GoombaAIList[i], LuckyBlock);
		//if (!ObstacleCheck && !BrickCheck && !LuckyCheck) {
		GoombaAIYveloList[i] += (GoombaAIYveloList[i] >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f);
		GoombaAIList[i].property.move({ 0.0f, GoombaAIYveloList[i] * deltaTime });
		GoombaAIYveloList[i] += (GoombaAIYveloList[i] >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f);
		//}

		NoAdd = false;
		int be, nd;
		be = find_min_iny(GoombaAIList[i], ObstaclesVertPosList);
		nd = find_max_iny_dist(GoombaAIList[i], ObstaclesVertPosList, 64.0f + (GoombaAIYveloList[i]) * 16.0f);
		ObstacleCollide = isAccurateCollideBott(GoombaAIList[i], ObstaclesVertPosList, CurrPosYCollide, NoAdd, be, nd, 80.0f);
		be = find_min_iny(GoombaAIList[i], BricksVertPosList);
		nd = find_max_iny_dist(GoombaAIList[i], BricksVertPosList, 64.0f + (GoombaAIYveloList[i]) * 16.0f);
		BrickCollide = isAccurateCollideBott(GoombaAIList[i], BricksVertPosList, CurrPosYCollide, NoAdd, be, nd, 80.0f);
		be = find_min_iny(GoombaAIList[i], LuckyVertPosList);
		nd = find_max_iny_dist(GoombaAIList[i], LuckyVertPosList, 64.0f + (GoombaAIYveloList[i]) * 16.0f);
		LuckyCollide = isAccurateCollideBott(GoombaAIList[i], LuckyVertPosList, CurrPosYCollide, NoAdd, be, nd, 80.0f);
		//ObstacleCollide = isAccurateCollideBot(GoombaAIList[i], ObstaclesList, CurrPosYCollide, NoAdd);
		//BrickCollide = isAccurateCollideBot(GoombaAIList[i], Bricks, CurrPosYCollide, NoAdd);
		//LuckyCollide = isAccurateCollideBot(GoombaAIList[i], LuckyBlock, CurrPosYCollide, NoAdd);
		if (ObstacleCollide.first || BrickCollide.first || LuckyCollide.first) {
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
				if (ObstacleCollide.second || BrickCollide.second || LuckyCollide.second) {
					GoombaAIList[i].property.setPosition({ GoombaAIList[i].property.getPosition().x, CurrPosYCollide - (GoombaAIDefinationList[i][1] - GoombaAIList[i].property.getOrigin().y + GoombaAIDefinationList[i][3]) });
				}
				if (!ObstacleCollide.second && !BrickCollide.second && !LuckyCollide.second) {
					GoombaAIList[i].property.setPosition({ GoombaAIList[i].property.getPosition().x, CurrPosYCollide - (GoombaAIDefinationList[i][1] - GoombaAIList[i].property.getOrigin().y + GoombaAIDefinationList[i][3]) });
				}
			}
		}
		// top update
		NoAdd = false;
		be = find_max_iny(GoombaAIList[i], ObstaclesVertPosList);
		nd = find_min_iny_dist(GoombaAIList[i], ObstaclesVertPosList, 64.0f - (GoombaAIYveloList[i]) * 16.0f);
		ObstacleCollide = isAccurateCollideTopt(GoombaAIList[i], ObstaclesVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		be = find_max_iny(GoombaAIList[i], BricksVertPosList);
		nd = find_min_iny_dist(GoombaAIList[i], BricksVertPosList, 64.0f - (GoombaAIYveloList[i]) * 16.0f);
		BrickCollide = isAccurateCollideTopt(GoombaAIList[i], BricksVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		be = find_max_iny(GoombaAIList[i], LuckyVertPosList);
		nd = find_min_iny_dist(GoombaAIList[i], LuckyVertPosList, 64.0f - (GoombaAIYveloList[i]) * 16.0f);
		LuckyCollide = isAccurateCollideTopt(GoombaAIList[i], LuckyVertPosList, CurrPosYCollide, NoAdd, nd, be, 80.0f);
		//ObstacleCollide = isAccurateCollideTop(GoombaAIList[i], ObstaclesList, CurrPosYCollide, NoAdd, {});
		//BrickCollide = isAccurateCollideTop(GoombaAIList[i], Bricks, CurrPosYCollide, NoAdd, BrickSaveList);
		//LuckyCollide = isAccurateCollideTop(GoombaAIList[i], LuckyBlock, CurrPosYCollide, NoAdd, LuckyBlockSaveList);
		if ((ObstacleCollide.first || BrickCollide.first || LuckyCollide.first) && GoombaAIYveloList[i] < 0.0f) {
			GoombaAIYveloList[i] = 0.0f;
			if (ObstacleCollide.second || BrickCollide.second || LuckyCollide.second) {
				GoombaAIList[i].property.setPosition({ GoombaAIList[i].property.getPosition().x, CurrPosYCollide + (31.0f + GoombaAIList[i].property.getOrigin().y - GoombaAIDefinationList[i][3]) });
			}
			if (!ObstacleCollide.second && !BrickCollide.second && !LuckyCollide.second) {
				GoombaAIList[i].property.setPosition({ GoombaAIList[i].property.getPosition().x, CurrPosYCollide + (31.0f + GoombaAIList[i].property.getOrigin().y - GoombaAIDefinationList[i][3]) });
			}
		}
	}
}
static void Kicking(int i, int j) {
	switch (GoombaAIShellHitCount[i]) {
	case 0:
		Sounds.PlaySound("Kick2");
		AddScoreEffect(SCORE_100, GoombaAIList[j].property.getPosition().x - 15.0f, GoombaAIList[j].property.getPosition().y - GoombaAIHitboxList[j].second);
		++GoombaAIShellHitCount[i];
		break;
	case 1:
		Sounds.PlaySound("Kick3");
		AddScoreEffect(SCORE_200, GoombaAIList[j].property.getPosition().x - 15.0f, GoombaAIList[j].property.getPosition().y - GoombaAIHitboxList[j].second);
		++GoombaAIShellHitCount[i];
		break;
	case 2:
		Sounds.PlaySound("Kick4");
		AddScoreEffect(SCORE_500, GoombaAIList[j].property.getPosition().x - 15.0f, GoombaAIList[j].property.getPosition().y - GoombaAIHitboxList[j].second);
		++GoombaAIShellHitCount[i];
		break;
	case 3:
		Sounds.PlaySound("Kick5");
		AddScoreEffect(SCORE_1000, GoombaAIList[j].property.getPosition().x - 12.0f, GoombaAIList[j].property.getPosition().y - GoombaAIHitboxList[j].second);
		++GoombaAIShellHitCount[i];
		break;
	case 4:
		Sounds.PlaySound("Kick6");
		AddScoreEffect(SCORE_2000, GoombaAIList[j].property.getPosition().x - 12.0f, GoombaAIList[j].property.getPosition().y - GoombaAIHitboxList[j].second);
		++GoombaAIShellHitCount[i];
		break;
	case 5:
		Sounds.PlaySound("Kick7");
		AddScoreEffect(SCORE_5000, GoombaAIList[j].property.getPosition().x - 12.0f, GoombaAIList[j].property.getPosition().y - GoombaAIHitboxList[j].second);
		++GoombaAIShellHitCount[i];
		break;
	case 6:
		Sounds.PlaySound("Kick8");
		AddScoreEffect(SCORE_1UP, GoombaAIList[j].property.getPosition().x - 15.0f, GoombaAIList[j].property.getPosition().y - GoombaAIHitboxList[j].second);
		GoombaAIShellHitCount[i] = 0;
		break;
	}
}
void GoombaAICollisionUpdate() {
	std::set<std::pair<GoombaAIType, std::pair<float, float>>> GoombaAIDeleteSet;
	std::set<int> coll_set;
	sf::FloatRect hitbox_loop;
	bool flag = false;
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIDisabledList[i] || GoombaAIAppearingList[i]) continue;
		hitbox_loop = getGlobalHitbox(GoombaAIList[i].hitboxMain, GoombaAIList[i].property);
		flag = false;
		for (int j = 0; j < GoombaAIList.size(); ++j) {
			if (f_abs(GoombaAIList[i].property.getPosition().x - GoombaAIList[j].property.getPosition().x) >= 160.0f) continue;
			if (GoombaAIDisabledList[j] || GoombaAIAppearingList[j] || i == j) continue;
			if (isCollide(GoombaAIList[j].hitboxMain, GoombaAIList[j].property, hitbox_loop)) {
				if (GoombaAITypeList[j] != SHELL_MOVING && GoombaAITypeList[i] != SHELL_MOVING) {
					if (!GoombaAICollisionList[i].first && !GoombaAICollisionList[j].first || (GoombaAICollisionList[i].second != j && GoombaAICollisionList[i].first)) {
						if (GoombaAIDirectionList[i] == GoombaAIDirectionList[j] && (GoombaAIXveloList[i] > 0.0f && GoombaAIXveloList[j] > 0.0f)) continue;
						if (GoombaAIDirectionList[i] == RIGHT && GoombaAIDirectionList[j] == LEFT) continue;

						GoombaAICollisionList[i].first = true;
						GoombaAICollisionList[i].second = j;
						flag = true;

						coll_set.insert(i);
						coll_set.insert(j);
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
					AddGoombaAIEffect(GoombaAITypeList[j], NONE, GoombaAISkinIDList[j], GoombaAIList[j].property.getPosition().x, GoombaAIList[j].property.getPosition().y);
					GoombaAIDeleteSet.insert({ GoombaAITypeList[j], { GoombaAIList[j].property.getPosition().x , GoombaAIList[j].property.getPosition().y } });
					break;
				}
				//else if (GoombaAITypeList[i] != SHELL_MOVING && GoombaAITypeList[j] == SHELL_MOVING) {
				//	Kicking(j, i);
				//	AddGoombaAIEffect(GoombaAITypeList[i], NONE, GoombaAISkinIDList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y);
				//	GoombaAIDeleteSet.insert({ GoombaAITypeList[i],  { GoombaAIList[i].property.getPosition().x , GoombaAIList[i].property.getPosition().y } });
				//}
				else if (GoombaAITypeList[i] == SHELL_MOVING && GoombaAITypeList[j] == SHELL_MOVING) {
					Sounds.PlaySound("Kick2");
					AddScoreEffect(SCORE_100, GoombaAIList[j].property.getPosition().x - 15.0f, GoombaAIList[j].property.getPosition().y - GoombaAIHitboxList[j].second);
					AddScoreEffect(SCORE_100, GoombaAIList[i].property.getPosition().x - 15.0f, GoombaAIList[i].property.getPosition().y - GoombaAIHitboxList[i].second);
					AddGoombaAIEffect(GoombaAITypeList[i], NONE, GoombaAISkinIDList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y);
					GoombaAIDeleteSet.insert({ GoombaAITypeList[i],  { GoombaAIList[i].property.getPosition().x , GoombaAIList[i].property.getPosition().y } });
					AddGoombaAIEffect(GoombaAITypeList[j], NONE, GoombaAISkinIDList[j], GoombaAIList[j].property.getPosition().x, GoombaAIList[j].property.getPosition().y);
					GoombaAIDeleteSet.insert({ GoombaAITypeList[j], { GoombaAIList[j].property.getPosition().x , GoombaAIList[j].property.getPosition().y } });
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
	for (const auto& i : GoombaAIDeleteSet) {
		DeleteGoombaAI(i.first, i.second.first, i.second.second);
	}
}
void GoombaAIUpdate() {
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		//if (GoombaAIDirectionList[i] == LEFT) GoombaAIList[i].property.setColor(sf::Color(255, 0, 0));
		//else GoombaAIList[i].property.setColor(sf::Color(0, 0, 255));
		if (!isOutScreen(GoombaAIList[i].property.getPosition().x - GoombaAIList[i].property.getOrigin().x, GoombaAIList[i].property.getPosition().y, 32, 80) && !GoombaAIDisabledList[i]) {
			if (GoombaAIDirectionList[i] == RIGHT) {
				GoombaAIAnimationList[i].first.update(GoombaAIList[i].property, GoombaAITextureManager.GetAnimatedTexture(GoombaAITextureNameList[i].first));

				GoombaAIAnimationList[i].second.silentupdate(GoombaAIList[i].property);

				if (GoombaAIList[i].property.getOrigin() != GoombaAIOriginList[i].first) GoombaAIList[i].property.setOrigin(GoombaAIOriginList[i].first);
			}
			else if (GoombaAIDirectionList[i] == LEFT) {
				GoombaAIAnimationList[i].second.update(GoombaAIList[i].property, GoombaAITextureManager.GetAnimatedTexture(GoombaAITextureNameList[i].second));

				GoombaAIAnimationList[i].first.silentupdate(GoombaAIList[i].property);

				if (GoombaAIList[i].property.getOrigin() != GoombaAIOriginList[i].second) GoombaAIList[i].property.setOrigin(GoombaAIOriginList[i].second);
			}
			window.draw(GoombaAIList[i].property);
		}
		else if (isOutScreen(GoombaAIList[i].property.getPosition().x - GoombaAIList[i].property.getOrigin().x, GoombaAIList[i].property.getPosition().y, 32, 80) && !GoombaAIDisabledList[i]) {
			GoombaAIAnimationList[i].second.silentupdate(GoombaAIList[i].property);
			GoombaAIAnimationList[i].first.silentupdate(GoombaAIList[i].property);
		}
	}
}