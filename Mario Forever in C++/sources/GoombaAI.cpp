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
//include "../headers/Mario.hpp"

#include "../resource.h"

std::vector<MovableObject> GoombaAIList;
std::vector<GoombaAIType> GoombaAITypeList;
std::array<sf::Texture, 1> GoombaAITexture; //
std::vector<std::pair<float, float>> GoombaAIHitboxList{ {31, 32} }; //
std::vector<GoombaAIDirection> GoombaAIDirectionList;
std::vector<float> GoombaAIYveloList;
std::vector<float> GoombaAIXveloList;
std::vector<bool> GoombaAIDisabledList;
std::vector<GoombaAIHittable> GoombaAIHittableList;
std::vector<std::array<float, 4>> GoombaAIDefinationList;
std::vector<LocalAnimationManager> GoombaAIAnimationList;
std::vector<sf::Clock> GoombaAIClock;

sf::SoundBuffer StompSoundBuffer;
sf::Sound StompSound;

//Function For Checking Collision
//X
//back to normal
int GoombaAILoadRes() {
	/*
		0: Goomba
	*/
	LoadTexture(GoombaAITexture[0], GOOMBA_TEXTURE, IMAGEFILE);

	LoadAudio(StompSoundBuffer, STOMP_SOUND, SOUNDFILE);
	StompSound.setBuffer(StompSoundBuffer);

	return 6;
}
int iniGoombaAI = GoombaAILoadRes();
void AddGoombaAI(GoombaAIType type, float x, float y) {
	MovableObject Init;
	LocalAnimationManager in;
	if (type == GOOMBA) {
		Init.property.setOrigin(16, 31);
		Init.property.setTexture(GoombaAITexture[0]);
		Init.property.setTextureRect(sf::IntRect(0, 0, 32, 32));
		GoombaAIXveloList.push_back(1.0f);
		GoombaAIHittableList.push_back(YES);
		GoombaAIDefinationList.push_back({ 31.0f, 32.0f, 0.0f, 0.0f });
		in.setAnimation({ 31,32 }, { 0,0 }, { 2,0 }, 11);
	}
	setHitbox(Init.hitboxMain, { 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], GoombaAIHitboxList[type].first, GoombaAIHitboxList[type].second });
	setHitbox(Init.hitboxTop, { 1.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], GoombaAIHitboxList[type].first - 2.0f, 2.0f });
	setHitbox(Init.hitboxBot, { 1.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], GoombaAIHitboxList[type].second - 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], GoombaAIHitboxList[type].first - 2.0f, 2.0f });
	setHitbox(Init.hitboxRight2, { GoombaAIHitboxList[type].first - 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], 3.0f, GoombaAIHitboxList[type].second - 9.0f });
	setHitbox(Init.hitboxLeft2, { -1.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], 3.0f, GoombaAIHitboxList[type].second - 9.0f });
	setHitbox(Init.hitboxRight, { GoombaAIHitboxList[type].first - 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], 2.0f, GoombaAIHitboxList[type].second - 9.0f });
	setHitbox(Init.hitboxLeft, { 0.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][2], 2.0f + GoombaAIDefinationList[GoombaAIDefinationList.size() - 1][3], 2.0f, GoombaAIHitboxList[type].second - 9.0f });
	Init.property.setPosition(x, y);
	GoombaAIAnimationList.push_back(in);
	GoombaAIClock.push_back(sf::Clock());
	GoombaAIClock[GoombaAIClock.size() - 1].restart().asMilliseconds();
	GoombaAIList.push_back(Init);
	GoombaAIDirectionList.push_back(LEFT);
	GoombaAITypeList.push_back(type);
	GoombaAIYveloList.push_back(0.0f);
	GoombaAIDisabledList.push_back(true);
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
			break;
		}
	}
}
void GoombaStatusUpdate() {
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (!isOutScreen(GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y, 32, 32) && GoombaAIDisabledList[i]) GoombaAIDisabledList[i] = false;
	}
}
void GoombaAICheckCollide() {
	sf::FloatRect hitbox_mario = getGlobalHitbox(player.hitboxMain, player.property);
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIDisabledList[i]) continue;
		if (isCollide(GoombaAIList[i].hitboxMain, GoombaAIList[i].property, hitbox_mario)) {
			if (GoombaAIHittableList[i] == YES) {
				if (((GoombaAIList[i].property.getPosition().y - 16.0f) > player.property.getPosition().y) && Yvelo > 0.0f) {
					if (!sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) Yvelo = -8.0f;
					else Yvelo = -13.0f;
					StompSound.play();
					if (GoombaAITypeList[i] == GOOMBA) AddScoreEffect(SCORE_100, GoombaAIList[i].property.getPosition().x - 15.0f, GoombaAIList[i].property.getPosition().y - GoombaAIHitboxList[0].second);
					DeleteGoombaAI(GoombaAITypeList[i], GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y);
					break;
				}
			}
			if ((GoombaAIList[i].property.getPosition().y - 16.0f) < player.property.getPosition().y) {
				window.close();
				break;
			}
		}
	}
}
void GoombaAIVertXUpdate() {
	sf::FloatRect hitbox_loop;
	bool isCollideLeftBool, isCollideRightBool;
	// Check if a GoombaAI collide with left or right
	for (int i = 0; i < GoombaAIList.size(); ++i) {
		if (GoombaAIDisabledList[i]) continue;
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
		if (GoombaAIDisabledList[i]) continue;
		// bottom update
		ObstacleCheck = isCollideBot(GoombaAIList[i], ObstaclesList);
		BrickCheck = isCollideBot(GoombaAIList[i], Bricks);
		LuckyCheck = isCollideBot(GoombaAIList[i], LuckyBlock);
		if (!ObstacleCheck && !BrickCheck && !LuckyCheck) {
			GoombaAIYveloList[i] += (GoombaAIYveloList[i] >= 10.0f ? 0.0f : 1.0f * deltaTime);
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
			if (GoombaAITypeList[i] == GOOMBA) GoombaAIAnimationList[i].update(GoombaAIList[i].property, GoombaAITexture[0]);
			window.draw(GoombaAIList[i].property);
		}
		else if (isOutScreen(GoombaAIList[i].property.getPosition().x, GoombaAIList[i].property.getPosition().y, 32, 32) && !GoombaAIDisabledList[i]) {
			if (GoombaAITypeList[i] == GOOMBA) GoombaAIAnimationList[i].silentupdate(GoombaAIList[i].property);
		}
	}
}