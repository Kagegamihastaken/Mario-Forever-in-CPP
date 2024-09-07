#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <array>

#include "../headers/GoombaAI.hpp"
#include "../headers/enum.hpp"
#include "../headers/Loading.hpp"
#include "../headers/Scroll.hpp"
#include "../headers/Obstacles.hpp"
#include "../headers/Brick.hpp"
#include "../headers/LuckyBlock.hpp"
#include "../headers/WindowFrame.hpp"
#include "../headers/LocalAnimationManager.hpp"
#include "../headers/ScoreEffect.hpp"
#include "../headers/Collide.hpp"
#include "../headers/GoombaAIEffect.hpp"
#include "../headers/Mario.hpp"
#include "../headers/Sound.hpp"
#include "../headers/Level.hpp"

#include "../resource.h"

std::vector<MovableObject> GoombaAIList;
std::vector<GoombaAIType> GoombaAITypeList;
std::array<sf::Texture, 3> GoombaAITexture; //
std::vector<std::pair<float, float>> GoombaAIHitboxList; //
std::vector<GoombaAIDirection> GoombaAIDirectionList;
std::vector<float> GoombaAIYveloList;
std::vector<float> GoombaAIXveloList;
std::vector<bool> GoombaAIDisabledList;
std::vector<GoombaAIHittable> GoombaAIHittableList;
std::vector<std::array<float, 4>> GoombaAIDefinationList;
std::vector<std::pair<LocalAnimationManager, LocalAnimationManager>> GoombaAIAnimationList;
std::vector<sf::Clock> GoombaAIClock;
std::vector<bool> GoombaAIAppearingList;
std::vector<float> GoombaAIAppearingYList;

//Function For Checking Collision
//X
//back to normal
int GoombaAILoadRes() {
	/*
		0: Goomba
		1: Mushroom
		2: Koopa (Green)
	*/
	LoadTexture(GoombaAITexture[0], GOOMBA_TEXTURE);
	LoadTexture(GoombaAITexture[1], MUSHROOM_TEXTURE);
	LoadTexture(GoombaAITexture[2], GREEN_KOOPA_TEXTURE);
	return 6;
}
int iniGoombaAI = GoombaAILoadRes();
void AddGoombaAI(GoombaAIType type, float x, float y) {
	MovableObject Init;
	LocalAnimationManager in, inri;
	if (type != MUSHROOM) {
		if (type == GOOMBA) {
			Init.property.setOrigin(16, 31);
			GoombaAIDefinationList.push_back({ 31.0f, 32.0f, 0.0f, 0.0f });
			GoombaAIHitboxList.push_back({ 31, 32 });
			in.setAnimation({ 31,32 }, { 0,0 }, { 2,0 }, 11);
			inri.setAnimation({ 31,32 }, { 0,0 }, { 2,0 }, 11);
		}
		else if (type == GREEN_KOOPA) {
			Init.property.setOrigin(13, 44);
			GoombaAIDefinationList.push_back({ 32.0f, 47.0f, 0.0f, 0.0f });
			GoombaAIHitboxList.push_back({ 32, 47 });
			in.setAnimation({ 32,47 }, { 0,0 }, { 2,0 }, 12);
			inri.setAnimation({ 32,47 }, { 0,1 }, { 2,1 }, 12);
		}
		GoombaAIXveloList.push_back(1.0f);
		GoombaAIHittableList.push_back(YES);
		GoombaAIAppearingList.push_back(false);
	}
	else {
		Init.property.setOrigin(16, 31);
		in.setAnimation({ 31,32 }, { 0,0 }, { 1,0 }, 100);
		inri.setAnimation({ 31,32 }, { 0,0 }, { 1,0 }, 100);
		GoombaAIXveloList.push_back(2.0f);
		GoombaAIHittableList.push_back(FULL);
		GoombaAIDefinationList.push_back({ 31.0f, 32.0f, 0.0f, 0.0f });
		GoombaAIHitboxList.push_back({ 31, 32 });
		GoombaAIAppearingList.push_back(true);
	}
	setHitbox(Init.hitboxMain, { 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second });
	setHitbox(Init.hitboxTop, { 1.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first - 2.0f, 2.0f });
	setHitbox(Init.hitboxBot, { 1.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first - 2.0f, 2.0f });
	setHitbox(Init.hitboxRight2, { GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first - 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], 3.0f, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 9.0f });
	setHitbox(Init.hitboxLeft2, { -1.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], 3.0f, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 9.0f });
	setHitbox(Init.hitboxRight, { GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].first - 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], 2.0f, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 9.0f });
	setHitbox(Init.hitboxLeft, { 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], 2.0f, GoombaAIHitboxList[GoombaAIHitboxList.size() - 1].second - 9.0f });
	Init.property.setPosition(x, y);
	GoombaAIAnimationList.push_back({ in, inri });
	GoombaAIClock.push_back(sf::Clock());
	GoombaAIClock[GoombaAIClock.size() - 1].restart().asMilliseconds();
	GoombaAIList.push_back(Init);
	GoombaAIDirectionList.push_back(LEFT);
	GoombaAITypeList.push_back(type);
	GoombaAIYveloList.push_back(0.0f);
	GoombaAIDisabledList.push_back(true);
	GoombaAIAppearingYList.push_back(0.0f);
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
			GoombaAIClock.erase(GoombaAIClock.begin() + i);
			GoombaAIAnimationList.erase(GoombaAIAnimationList.begin() + i);
			GoombaAIHittableList.erase(GoombaAIHittableList.begin() + i);
			GoombaAIDefinationList.erase(GoombaAIDefinationList.begin() + i);
			GoombaAIHitboxList.erase(GoombaAIHitboxList.begin() + i);
			GoombaAIAppearingList.erase(GoombaAIAppearingList.begin() + i);
			GoombaAIAppearingYList.erase(GoombaAIAppearingYList.begin() + i);
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
	GoombaAIClock.clear();
	GoombaAIAnimationList.clear();
	GoombaAIHittableList.clear();
	GoombaAIDefinationList.clear();
	GoombaAIHitboxList.clear();
	GoombaAIAppearingList.clear();
	GoombaAIAppearingYList.clear();
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
				if (((GoombaAIList[i].property.getPosition().y - 16.0f) > player.property.getPosition().y) && Yvelo > 0.0f) {
					if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) Yvelo = -8.0f;
					else Yvelo = -13.0f;
					StompSound.play();
					AddGoombaAIEffect(GoombaAITypeList[i], COLLIDE, GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y);
					if (GoombaAITypeList[i] == GOOMBA) AddScoreEffect(SCORE_100, GoombaAIList[i].property.getPosition().x - 15.0f, GoombaAIList[i].property.getPosition().y - GoombaAIHitboxList[0].second);
					DeleteGoombaAI(GoombaAITypeList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y);
					break;
				}
			}
			if (GoombaAIHittableList[i] == YES || GoombaAIHittableList[i] == NO) {
				if ((GoombaAIList[i].property.getPosition().y - 16.0f) < player.property.getPosition().y) {
					//Objectbuilding();
					PowerDown();
					break;
				}
			}
			else if (GoombaAIHittableList[i] == FULL) {
				if (GoombaAITypeList[i] == MUSHROOM) {
					Powerup.play();
					if (GoombaAITypeList[i] == MUSHROOM) AddScoreEffect(SCORE_1000, GoombaAIList[i].property.getPosition().x - 15.0f, GoombaAIList[i].property.getPosition().y - GoombaAIHitboxList[0].second);
					if (PowerState == 0) PowerState = 1;
					DeleteGoombaAI(GoombaAITypeList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y);
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
			if (GoombaAIClock[i].getElapsedTime().asMilliseconds() >= 25.0f * deltaTime) {
				GoombaAIYveloList[i] += (GoombaAIYveloList[i] >= 10.0f ? 0.0f : 1.0f * deltaTime);
				GoombaAIClock[i].restart().asMilliseconds();
			}
			GoombaAIList[i].property.move(0.0f, GoombaAIYveloList[i] * deltaTime);
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
void GoombaAIUpdate() {
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (!isOutScreen(GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y, 32, 32) && !GoombaAIDisabledList[i]) {
			if (GoombaAIDirectionList[i] == RIGHT) {
				GoombaAIAnimationList[i].first.update(GoombaAIList[i].property, GoombaAITexture[GoombaAITypeList[i]]);

				GoombaAIAnimationList[i].second.silentupdate(GoombaAIList[i].property);
			}
			else if (GoombaAIDirectionList[i] == LEFT) {
				GoombaAIAnimationList[i].second.update(GoombaAIList[i].property, GoombaAITexture[GoombaAITypeList[i]]);

				GoombaAIAnimationList[i].first.silentupdate(GoombaAIList[i].property);
			}
			window.draw(GoombaAIList[i].property);
		}
		else if (isOutScreen(GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y, 32, 32) && !GoombaAIDisabledList[i]) {
			GoombaAIAnimationList[i].second.silentupdate(GoombaAIList[i].property);
			GoombaAIAnimationList[i].first.silentupdate(GoombaAIList[i].property);
		}
	}
}