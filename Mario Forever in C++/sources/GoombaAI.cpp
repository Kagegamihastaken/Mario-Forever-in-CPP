#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <array>

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

#include "../resource.h"

std::vector<MovableObject> GoombaAIList;
std::vector<GoombaAIType> GoombaAITypeList;
std::array<sf::Texture, 4> GoombaAITexture; //
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
TextureManager GoombaAITextureManager;

void LoadGoombaAITexture(sf::Texture* texture, std::string name, int start, int end, int y, int sizex, int sizey) {
	sf::Texture* tex = new sf::Texture();
	std::vector<sf::Texture*>* ani = new std::vector<sf::Texture*>();
	for (int i = start; i <= end; ++i) {
		tex->loadFromImage(texture->copyToImage(), sf::IntRect(i * sizex, y * sizey, sizex, sizey));
		ani->push_back(tex);
		tex = new sf::Texture();
	}
	GoombaAITextureManager.AddAnimatedTexture(name, *ani);
	delete tex;
	delete ani;
}
void GoombaAILoadRes() {
	/*
		0: Goomba
		1: Mushroom
		2: Koopa (Green)
		3: Shell (Green)
	*/
	LoadTexture(GoombaAITexture[0], GOOMBA_TEXTURE);
	LoadTexture(GoombaAITexture[1], MUSHROOM_TEXTURE);
	LoadTexture(GoombaAITexture[2], GREEN_KOOPA_TEXTURE);
	LoadTexture(GoombaAITexture[3], GREEN_KOOPA_SHELL_TEXTURE);

	LoadGoombaAITexture(&GoombaAITexture[0], "Goomba", 0, 1, 0, 31, 32);
	LoadGoombaAITexture(&GoombaAITexture[1], "Mushroom", 0, 0, 0, 31, 32);
	LoadGoombaAITexture(&GoombaAITexture[2], "Koopa_right_green", 0, 1, 0, 32, 47);
	LoadGoombaAITexture(&GoombaAITexture[2], "Koopa_left_green", 0, 1, 1, 32, 47);
	LoadGoombaAITexture(&GoombaAITexture[3], "Koopa_Shell_green_idle", 3, 3, 0, 33, 28);
	LoadGoombaAITexture(&GoombaAITexture[3], "Koopa_Shell_green_moving", 0, 3, 0, 33, 28);
}
int getGoombaAISKin(GoombaAIType type, int SkinID) {
	if (type == GOOMBA) return 0;
	else if (type == MUSHROOM) return 1;
	else if (type == KOOPA) return 2;
	else if (type == SHELL) return 3;
	else if (type == SHELL_MOVING) return 4;

	return -1;
}
void AddGoombaAI(GoombaAIType type, int SkinID, float x, float y, GoombaAIDirection Dir = LEFT) {
	MovableObject Init;
	LocalAnimationManager in, inri;
	switch (type) {
	case GOOMBA:
		Init.property.setOrigin(15, 31);
		GoombaAIDefinationList.push_back({ 31.0f, 32.0f, 0.0f, 0.0f });
		GoombaAIHitboxList.push_back({ 31, 32 });
		in.setAnimation(0, 1, 11);
		inri.setAnimation(0, 1, 11);
		GoombaAITextureNameList.push_back({ "Goomba", "Goomba" });
		GoombaAIXveloList.push_back(1.0f);
		GoombaAIHittableList.push_back(YES);
		GoombaAIAppearingList.push_back(false);
		GoombaAIOriginList.push_back({ sf::Vector2f(15, 31), sf::Vector2f(15, 31) });
		GoombaAIInvincibleTimerList.push_back(sf::Clock());
		GoombaAIInvincibleSecondLimitList.push_back(0.0f);
		break;
	case KOOPA:
		Init.property.setOrigin(13, 44);
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
		Init.property.setOrigin(16, 31);
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
		Init.property.setOrigin(16, 27);
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
		Init.property.setOrigin(16, 27);
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
	}
	GoombaAIInvincibleTimerList.push_back(sf::Clock());
	GoombaAIShellHitCount.push_back(0);
	setHitbox(Init.hitboxMain, { 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second });
	setHitbox(Init.hitboxTop, { 1.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first - 2.0f, 2.0f });
	setHitbox(Init.hitboxBot, { 1.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first - 2.0f, 2.0f });
	setHitbox(Init.hitboxRight2, { GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first - 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], 3.0f, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 9.0f });
	setHitbox(Init.hitboxLeft2, { -1.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], 3.0f, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 9.0f });
	setHitbox(Init.hitboxRight, { GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first - 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], 2.0f, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 9.0f });
	setHitbox(Init.hitboxLeft, { 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], 2.0f, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 9.0f });
	Init.property.setPosition(x, y);
	GoombaAIAnimationList.push_back({ in, inri });
	GoombaAIList.push_back(Init);
	GoombaAIDirectionList.push_back(Dir);
	GoombaAITypeList.push_back(type);
	GoombaAIYveloList.push_back(0.0f);
	GoombaAIDisabledList.push_back(true);
	GoombaAIAppearingYList.push_back(0.0f);
	GoombaAISkinIDList.push_back(SkinID);
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
}
void GoombaStatusUpdate() {
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (!isOutScreen(GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y, 32, 32) && GoombaAIDisabledList[i]) GoombaAIDisabledList[i] = false;
		if (GoombaAIAppearingList[i] && !GoombaAIDisabledList[i]) {
			GoombaAIList[i].property.move(0.0f, -0.5f * deltaTime);
			GoombaAIAppearingYList[i] += 0.5f * deltaTime;
			if (GoombaAIAppearingYList[i] >= 32.0f) GoombaAIAppearingList[i] = false;
		}
	}
}
void GoombaAICheckCollide() {
	sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.property);
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIDisabledList[i] || GoombaAIAppearingList[i]) continue;
		if (isCollide(GoombaAIList[i].hitboxMain, GoombaAIList[i].property, hitbox_mario)) {
			if (GoombaAIHittableList[i] == YES) {
				if ((GoombaAIInvincibleTimerList[i].getElapsedTime().asSeconds() >= GoombaAIInvincibleSecondLimitList[i] && GoombaAIInvincibleSecondLimitList[i] > 0.0f) || GoombaAIInvincibleSecondLimitList[i] == 0.0f) {
					if (((GoombaAIList[i].property.getPosition().y - 16.0f) > player.property.getPosition().y) && Yvelo > 0.0f) {
						if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) Yvelo = -8.0f;
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
						}
						DeleteGoombaAI(GoombaAITypeList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y);
						break;
					}
				}
			}
			if (GoombaAIHittableList[i] == YES || GoombaAIHittableList[i] == NO) {
				if ((GoombaAIInvincibleTimerList[i].getElapsedTime().asSeconds() > GoombaAIInvincibleSecondLimitList[i] && GoombaAIInvincibleSecondLimitList[i] > 0.0f) || GoombaAIInvincibleSecondLimitList[i] == 0.0f) {
					if ((GoombaAIList[i].property.getPosition().y - 16.0f) < player.property.getPosition().y) {
						//Objectbuilding();
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
			}
		}
	}
}
void GoombaAIVertXUpdate() {
	sf::FloatRect hitbox_loop;
	bool isCollideLeftBool, isCollideRightBool;
	// Check if a GoombaAI collide with left or right
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIDisabledList[i] || GoombaAIAppearingList[i]) continue;
		//move
		if (GoombaAIDirectionList[i] == LEFT) GoombaAIList[i].property.move(-GoombaAIXveloList[i] * deltaTime, 0.0f);
		else GoombaAIList[i].property.move(GoombaAIXveloList[i] * deltaTime, 0.0f);
		//check collide
		std::pair<bool, bool> ObstacleCheck = isOrCollideSide(GoombaAIList[i], ObstaclesList, {});
		std::pair<bool, bool> BrickCheck = isOrCollideSide(GoombaAIList[i], Bricks, BrickSaveList);
		std::pair<bool, bool> LuckyCheck = isOrCollideSide(GoombaAIList[i], LuckyBlock, LuckyBlockSaveList);
		bool isTrueCollide = false;
		if (ObstacleCheck.first || ObstacleCheck.second || BrickCheck.first || BrickCheck.second || LuckyCheck.first || LuckyCheck.second) {
			//shell moving break block
			if (GoombaAITypeList[i] == SHELL_MOVING) {
				float BrickCurrPosX = 0, BrickCurrPosY = 0;
				std::pair<bool, bool> BrickCollideRemove = isAccurateCollideSide2(GoombaAIList[i], Bricks, BrickCurrPosX, BrickCurrPosY, BrickSaveList);
				if (BrickCollideRemove.first || BrickCollideRemove.second) {
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
				std::pair<bool, bool> LuckyCollideRemove = isAccurateCollideSide2(GoombaAIList[i], LuckyBlock, LuckyCurrPosX, LuckyCurrPosY, LuckyBlockSaveList);
				if (LuckyCollideRemove.first || LuckyCollideRemove.second) LuckyHit(LuckyCurrPosX, LuckyCurrPosY - 32.0f, getLuckyIndex(LuckyCurrPosX, LuckyCurrPosY));
			}
			// Stop on wall
			if ((ObstacleCheck.first && GoombaAIDirectionList[i] == RIGHT) || (ObstacleCheck.second && GoombaAIDirectionList[i] == LEFT)) {
				if (GoombaAIDirectionList[i] == RIGHT) GoombaAIDirectionList[i] = LEFT;
				else GoombaAIDirectionList[i] = RIGHT;
			}
			if ((BrickCheck.first && GoombaAIDirectionList[i] == RIGHT) || (BrickCheck.second && GoombaAIDirectionList[i] == LEFT)) {
				if (GoombaAIDirectionList[i] == RIGHT) GoombaAIDirectionList[i] = LEFT;
				else GoombaAIDirectionList[i] = RIGHT;
			}
			if ((LuckyCheck.first && GoombaAIDirectionList[i] == RIGHT) || (LuckyCheck.second && GoombaAIDirectionList[i] == LEFT)) {
				if (GoombaAIDirectionList[i] == RIGHT) GoombaAIDirectionList[i] = LEFT;
				else GoombaAIDirectionList[i] = RIGHT;
			}
			// Count if size AllCollideList equal to CollideAddCounter
			float CurrPosXCollide = 0, CurrPosYCollide = 0;
			bool isCollideSide = false;
			// 0 for right; 1 for left
			bool NoAdd = false;
			std::pair<bool, bool> ObstacleCollide, BrickCollide, LuckyCollide;
			isCollideLeftBool = false;
			isCollideRightBool = false;
			// Loop through obstacles
			ObstacleCollide = isAccurateCollideSide(GoombaAIList[i], ObstaclesList, CurrPosXCollide, CurrPosYCollide, NoAdd, {});
			BrickCollide = isAccurateCollideSide(GoombaAIList[i], Bricks, CurrPosXCollide, CurrPosYCollide, NoAdd, BrickSaveList);
			LuckyCollide = isAccurateCollideSide(GoombaAIList[i], LuckyBlock, CurrPosXCollide, CurrPosYCollide, NoAdd, LuckyBlockSaveList);
			// Break if size AllCollideList is not equal to CollideAddCounter
			//if (AllCollideList.size() != CollideAddCounter) break;
			// Adjust Position if collide

			if (ObstacleCollide.first) GoombaAIList[i].property.setPosition(CurrPosXCollide + 32.0f - GoombaAIDefinationList[i][2] + GoombaAIList[i].property.getOrigin().x, GoombaAIList[i].property.getPosition().y);
			if (ObstacleCollide.second) GoombaAIList[i].property.setPosition(CurrPosXCollide - (1.0f + GoombaAIDefinationList[i][2] + (GoombaAIDefinationList[i][0] - GoombaAIList[i].property.getOrigin().x)), GoombaAIList[i].property.getPosition().y);
			if (BrickCollide.first) GoombaAIList[i].property.setPosition(CurrPosXCollide + 32.0f - GoombaAIDefinationList[i][2] + GoombaAIList[i].property.getOrigin().x, GoombaAIList[i].property.getPosition().y);
			if (BrickCollide.second) GoombaAIList[i].property.setPosition(CurrPosXCollide - (1.0f + GoombaAIDefinationList[i][2] + (GoombaAIDefinationList[i][0] - GoombaAIList[i].property.getOrigin().x)), GoombaAIList[i].property.getPosition().y);
			if (LuckyCollide.first) GoombaAIList[i].property.setPosition(CurrPosXCollide + 32.0f - GoombaAIDefinationList[i][2] + GoombaAIList[i].property.getOrigin().x, GoombaAIList[i].property.getPosition().y);
			if (LuckyCollide.second) GoombaAIList[i].property.setPosition(CurrPosXCollide - (1.0f + GoombaAIDefinationList[i][2] + (GoombaAIDefinationList[i][0] - GoombaAIList[i].property.getOrigin().x)), GoombaAIList[i].property.getPosition().y);

			//if (!ObstacleCollide.first && !ObstacleCollide.second && !BrickCollide.first && !BrickCollide.second && !LuckyCollide.first && !LuckyCollide.second) break;
			// Break if no collide
		}
	}
}
void GoombaAIVertYUpdate() {
	std::pair<bool, std::pair<bool, bool>> ObstacleCollide, BrickCollide, LuckyCollide;
	bool ObstacleCheck, BrickCheck, LuckyCheck, isLanding;
	float CurrPosYCollide;
	bool NoAdd;
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIDisabledList[i] || GoombaAIAppearingList[i]) continue;
		// bottom update
		ObstacleCheck = isCollideBot(GoombaAIList[i], ObstaclesList);
		BrickCheck = isCollideBot(GoombaAIList[i], Bricks);
		LuckyCheck = isCollideBot(GoombaAIList[i], LuckyBlock);
		if (!ObstacleCheck && !BrickCheck && !LuckyCheck) {
			GoombaAIYveloList[i] += (GoombaAIYveloList[i] >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f);
			GoombaAIList[i].property.move(0.0f, GoombaAIYveloList[i] * deltaTime);
			GoombaAIYveloList[i] += (GoombaAIYveloList[i] >= 10.0f ? 0.0f : 0.5f * deltaTime * 0.3f);
		}
		ObstacleCheck = isCollideBot(GoombaAIList[i], ObstaclesList);
		BrickCheck = isCollideBot(GoombaAIList[i], Bricks);
		LuckyCheck = isCollideBot(GoombaAIList[i], LuckyBlock);
		//recolide
		if (ObstacleCheck || BrickCheck || LuckyCheck) {
			NoAdd = false;
			if (GoombaAIYveloList[i] >= 0.0f) {
				GoombaAIYveloList[i] = 0.0f;
				isLanding = true;
			}
			else {
				GoombaAIYveloList[i] = 0.0f;
				isLanding = false;
			}
			ObstacleCollide = isAccurateCollideBot(GoombaAIList[i], ObstaclesList, CurrPosYCollide, NoAdd);
			BrickCollide = isAccurateCollideBot(GoombaAIList[i], Bricks, CurrPosYCollide, NoAdd);
			LuckyCollide = isAccurateCollideBot(GoombaAIList[i], LuckyBlock, CurrPosYCollide, NoAdd);
			if ((ObstacleCollide.first || BrickCollide.first || LuckyCollide.first) && isLanding) {
				if (ObstacleCollide.second.first || ObstacleCollide.second.second || BrickCollide.second.first || BrickCollide.second.second || LuckyCollide.second.first || LuckyCollide.second.second) {
					GoombaAIList[i].property.setPosition(GoombaAIList[i].property.getPosition().x, CurrPosYCollide - (GoombaAIDefinationList[i][1] - GoombaAIList[i].property.getOrigin().y + GoombaAIDefinationList[i][3]));
				}
				if (!ObstacleCollide.second.first && !ObstacleCollide.second.second && !BrickCollide.second.first && !BrickCollide.second.second && !LuckyCollide.second.first && !LuckyCollide.second.second) {
					GoombaAIList[i].property.setPosition(GoombaAIList[i].property.getPosition().x, CurrPosYCollide - (GoombaAIDefinationList[i][1] - GoombaAIList[i].property.getOrigin().y + GoombaAIDefinationList[i][3]));
				}
			}
		}
		// top update
		ObstacleCheck = isCollideTop(GoombaAIList[i], ObstaclesList, {});
		BrickCheck = isCollideTop(GoombaAIList[i], Bricks, BrickSaveList);
		LuckyCheck = isCollideTop(GoombaAIList[i], LuckyBlock, LuckyBlockSaveList);
		if ((ObstacleCheck || BrickCheck || LuckyCheck) && GoombaAIYveloList[i] < 0.0f) {
			GoombaAIYveloList[i] = 0.0f;
			ObstacleCollide = isAccurateCollideTop(GoombaAIList[i], ObstaclesList, CurrPosYCollide, NoAdd, {});
			BrickCollide = isAccurateCollideTop(GoombaAIList[i], Bricks, CurrPosYCollide, NoAdd, BrickSaveList);
			LuckyCollide = isAccurateCollideTop(GoombaAIList[i], LuckyBlock, CurrPosYCollide, NoAdd, LuckyBlockSaveList);
			if ((ObstacleCollide.first || BrickCollide.first || LuckyCollide.first)) {
				if (ObstacleCollide.second.first || ObstacleCollide.second.second || BrickCollide.second.first || BrickCollide.second.second || LuckyCollide.second.first || LuckyCollide.second.second) {
					GoombaAIList[i].property.setPosition(GoombaAIList[i].property.getPosition().x, CurrPosYCollide + (31.0f + GoombaAIList[i].property.getOrigin().y - GoombaAIDefinationList[i][3]));
				}
				if (!ObstacleCollide.second.first && !ObstacleCollide.second.second && !BrickCollide.second.first && !BrickCollide.second.second && !LuckyCollide.second.first && !LuckyCollide.second.second) {
					GoombaAIList[i].property.setPosition(GoombaAIList[i].property.getPosition().x, CurrPosYCollide + (31.0f + GoombaAIList[i].property.getOrigin().y - GoombaAIDefinationList[i][3]));
				}
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
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		hitbox_loop = getGlobalHitbox(GoombaAIList[i].hitboxMain, GoombaAIList[i].property);
		for (int j = i + 1; j < GoombaAIList.size(); ++j) {
			if (isCollide(GoombaAIList[j].hitboxMain, GoombaAIList[j].property, hitbox_loop) && !GoombaAIDisabledList[i] && !GoombaAIDisabledList[j]) {
				if (GoombaAITypeList[j] != SHELL_MOVING && GoombaAITypeList[i] != SHELL_MOVING) {
					if (GoombaAIDirectionList[i] == LEFT) GoombaAIDirectionList[i] = RIGHT;
					else GoombaAIDirectionList[i] = LEFT;

					if (GoombaAIDirectionList[j] == LEFT) GoombaAIDirectionList[j] = RIGHT;
					else GoombaAIDirectionList[j] = LEFT;
				}
				else if (GoombaAITypeList[i] == SHELL_MOVING && GoombaAITypeList[j] != SHELL_MOVING) {
					Kicking(i, j);
					AddGoombaAIEffect(GoombaAITypeList[j], NONE, GoombaAISkinIDList[j], GoombaAIList[j].property.getPosition().x, GoombaAIList[j].property.getPosition().y);
					GoombaAIDeleteSet.insert({ GoombaAITypeList[j], { GoombaAIList[j].property.getPosition().x , GoombaAIList[j].property.getPosition().y } });
				}
				else if (GoombaAITypeList[i] != SHELL_MOVING && GoombaAITypeList[j] == SHELL_MOVING) {
					Kicking(j, i);
					AddGoombaAIEffect(GoombaAITypeList[i], NONE, GoombaAISkinIDList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y);
					GoombaAIDeleteSet.insert({ GoombaAITypeList[i],  { GoombaAIList[i].property.getPosition().x , GoombaAIList[i].property.getPosition().y } });
				}
				else {
					Sounds.PlaySound("Kick2");
					AddScoreEffect(SCORE_100, GoombaAIList[j].property.getPosition().x - 15.0f, GoombaAIList[j].property.getPosition().y - GoombaAIHitboxList[j].second);
					AddScoreEffect(SCORE_100, GoombaAIList[i].property.getPosition().x - 15.0f, GoombaAIList[i].property.getPosition().y - GoombaAIHitboxList[i].second);
					AddGoombaAIEffect(GoombaAITypeList[i], NONE, GoombaAISkinIDList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y);
					GoombaAIDeleteSet.insert({ GoombaAITypeList[i],  { GoombaAIList[i].property.getPosition().x , GoombaAIList[i].property.getPosition().y } });
					AddGoombaAIEffect(GoombaAITypeList[j], NONE, GoombaAISkinIDList[j], GoombaAIList[j].property.getPosition().x, GoombaAIList[j].property.getPosition().y);
					GoombaAIDeleteSet.insert({ GoombaAITypeList[j], { GoombaAIList[j].property.getPosition().x , GoombaAIList[j].property.getPosition().y } });
				}
			}
		}
	}
	int counter = 0;
	for (const auto& i : GoombaAIDeleteSet) {
		DeleteGoombaAI(i.first, i.second.first, i.second.second);
	}
}
void GoombaAIUpdate() {
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (!isOutScreen(GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y, 32, 32) && !GoombaAIDisabledList[i]) {
			if (GoombaAIDirectionList[i] == RIGHT) {
				//GoombaAIAnimationList[i].first.update(GoombaAIList[i].property, GoombaAITexture[getGoombaAISKin(GoombaAITypeList[i], GoombaAISkinIDList[i])]);
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
		else if (isOutScreen(GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y, 32, 32) && !GoombaAIDisabledList[i]) {
			GoombaAIAnimationList[i].second.silentupdate(GoombaAIList[i].property);
			GoombaAIAnimationList[i].first.silentupdate(GoombaAIList[i].property);
		}
	}
}