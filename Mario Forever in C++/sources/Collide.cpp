#include <SFML/Graphics.hpp>
#include <array>
#include <functional>
#include <vector>

#include "../headers/Core/WindowFrame.hpp"
#include "../headers/Core/Collision/Collide.hpp"
#include "../headers/Object/Mario.hpp"
#include "../headers/Block/Obstacles.hpp"
#include "../headers/Block/Slopes.hpp"

void setHitbox(sf::FloatRect& hitbox, const sf::FloatRect& Sethitbox) {
	hitbox = Sethitbox;
}
sf::FloatRect getGlobalHitbox(const sf::FloatRect& hitbox, const sf::Sprite& sprite) {
	return sprite.getTransform().transformRect(hitbox);
}
bool isCollide(const sf::FloatRect& hitbox, const sf::Sprite& sprite, const sf::FloatRect& other) {
	return getGlobalHitbox(hitbox, sprite).intersects(other);
}

// Mario && GoombaAI only
bool isCollideLeft2(const MovableObject& object, std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList = {}) {
	bool flag = false;
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (SaveList.size() > 0) {
			hitbox_loop.top = SaveList[i].second;
			hitbox_loop.left = SaveList[i].first;
		}
		if (isCollide(object.hitboxLeft2, object.property, hitbox_loop)) return true;
	}
	return false;
}
bool isCollideRight2(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList = {}) {
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (SaveList.size() > 0) {
			hitbox_loop.top = SaveList[i].second;
			hitbox_loop.left = SaveList[i].first;
		}
		if (isCollide(object.hitboxRight2, object.property, hitbox_loop)) return true;
	}
	return false;
}
std::pair<bool, bool> isOrCollideSide(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList = {}) {
	sf::FloatRect hitbox_loop;
	bool isLeft = false, isRight = false;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (SaveList.size() > 0) {
			hitbox_loop.top = SaveList[i].second;
			hitbox_loop.left = SaveList[i].first;
		}
		if (isCollide(object.hitboxRight2, object.property, hitbox_loop)) isRight = true;
		if (isCollide(object.hitboxLeft2, object.property, hitbox_loop)) isLeft = true;
		if (isRight && isLeft) break;
	}
	return { isRight, isLeft };
}
std::pair<bool, bool> isAccurateCollideSide(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, const std::vector<std::pair<float, float>>& SaveList = {}) {
	bool isCollideLeftBool = false, isCollideRightBool = false;
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (SaveList.size() > 0) {
			hitbox_loop.top = SaveList[i].second;
			hitbox_loop.left = SaveList[i].first;
		}
		// Check if collide
		if (isCollide(object.hitboxLeft, object.property, hitbox_loop)) {
			isCollideLeftBool = true;
			if (CurrPosXCollide != hitbox_loop.getPosition().x || CurrPosYCollide != hitbox_loop.getPosition().y) {
				if (!NoAdd) {
					CurrPosXCollide = hitbox_loop.getPosition().x;
					CurrPosYCollide = hitbox_loop.getPosition().y;
					NoAdd = true;
				}
			}
			break;
		}
		if (isCollide(object.hitboxRight, object.property, hitbox_loop)) {
			isCollideRightBool = true;
			if (CurrPosXCollide != hitbox_loop.getPosition().x || CurrPosYCollide != hitbox_loop.getPosition().y) {
				if (!NoAdd) {
					CurrPosXCollide = hitbox_loop.getPosition().x;
					CurrPosYCollide = hitbox_loop.getPosition().y;
					NoAdd = true;
				}
			}
			break;
		}
	}
	return { isCollideLeftBool, isCollideRightBool };
}
//Y
bool isCollideBotSlope(const MovableObject& object) {
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < SlopesList.size(); ++i) {
		hitbox_loop = getGlobalHitbox(SlopesList[i].hitbox, SlopesList[i].property);
		if (isCollide(object.hitboxBot2, object.property, hitbox_loop)) {
			for (auto& j : SlopeRectList[SlopesIDList[i]]) {
				hitbox_loop = getGlobalHitbox(j, SlopesList[i].property);
				if (isCollide(object.hitboxBot2, object.property, hitbox_loop)) return true;
			}
		}
	}
	return false;
}
bool isCollideBot(const MovableObject& object, const std::vector<Obstacles>& OL) {
	sf::FloatRect hitbox_loop;
	for (auto& i : OL) {
		hitbox_loop = getGlobalHitbox(i.hitbox, i.property);
		if (isCollide(object.hitboxBot, object.property, hitbox_loop)) return true;
	}
	return false;
}
std::pair<bool, std::pair<bool, bool>> isAccurateCollideBot(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosYCollide, bool& NoAdd) {
	bool isCollideBotBool = false, isCollideRight = false, isCollideLeft = false;
	sf::FloatRect hitbox_loop;
	for (auto& i : OL) {
		hitbox_loop = getGlobalHitbox(i.hitbox, i.property);
		if (isCollide(object.hitboxBot, object.property, hitbox_loop)) {
			isCollideBotBool = true;
			if (!NoAdd) {
				CurrPosYCollide = hitbox_loop.getPosition().y;
				NoAdd = true;
			}
		}
		if (isCollide(object.hitboxRight, object.property, hitbox_loop)) isCollideRight = true;
		if (isCollide(object.hitboxLeft, object.property, hitbox_loop)) isCollideLeft = true;
		if (isCollideBotBool && isCollideRight && isCollideLeft) break;
	}
	return { isCollideBotBool, { isCollideRight, isCollideLeft } };
}
std::pair<bool, std::pair<bool, bool>> isAccuratelyCollideBotSlope(const MovableObject& object, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, float& ID, std::vector<std::array<float, 3>>& SlopeTemp) {
	bool isCollideBotBool = false, isCollideRight = false, isCollideLeft = false;
	sf::FloatRect hitbox_loop;
	const sf::FloatRect hitbox = sf::FloatRect(0.0f, -32.0f, 32.0f, 64.0f);
	for (int i = 0; i < SlopesList.size(); ++i) {
		hitbox_loop = getGlobalHitbox(hitbox, SlopesList[i].property);
		if (isCollide(object.hitboxBot, object.property, hitbox_loop)) {
			isCollideBotBool = true;
			if (!NoAdd) {
				CurrPosXCollide = SlopesList[i].property.getPosition().x;
				CurrPosYCollide = hitbox_loop.getPosition().y + 32.0f;
				ID = SlopesIDList[i];
				NoAdd = true;
			}
			CurrPosXCollide = SlopesList[i].property.getPosition().x;
			CurrPosYCollide = hitbox_loop.getPosition().y + 32.0f;
			ID = SlopesIDList[i];
			SlopeTemp.push_back({ CurrPosXCollide, CurrPosYCollide, ID });
		}
		if (isCollide(object.hitboxRight, object.property, hitbox_loop)) isCollideRight = true;
		if (isCollide(object.hitboxLeft, object.property, hitbox_loop)) isCollideLeft = true;
		if (isCollideBotBool && isCollideRight && isCollideLeft) break;
	}
	return { isCollideBotBool, { isCollideRight, isCollideLeft } };
}
std::pair<bool, std::pair<bool, bool>> isAccuratelyCollideBot(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosXCollide, float& CurrPosYCollide, bool& NoAdd, float& ID, std::vector<std::array<float, 3>>& SlopeTemp) {
	bool isCollideBotBool = false, isCollideRight = false, isCollideLeft = false;
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (isCollide(object.hitboxBot, object.property, hitbox_loop)) {
			isCollideBotBool = true;
			if (!NoAdd) {
				CurrPosXCollide = OL[i].property.getPosition().x;
				CurrPosYCollide = hitbox_loop.getPosition().y;
				ID = SlopesIDList[i];
				NoAdd = true;
			}
			CurrPosXCollide = OL[i].property.getPosition().x;
			CurrPosYCollide = hitbox_loop.getPosition().y;
			ID = SlopesIDList[i];
			SlopeTemp.push_back({ CurrPosXCollide, CurrPosYCollide, ID });
		}
		if (isCollide(object.hitboxRight, object.property, hitbox_loop)) isCollideRight = true;
		if (isCollide(object.hitboxLeft, object.property, hitbox_loop)) isCollideLeft = true;
		if (isCollideBotBool && isCollideRight && isCollideLeft) break;
	}
	return { isCollideBotBool, { isCollideRight, isCollideLeft } };
}
bool isCollideTop(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList = {}) {
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (SaveList.size() > 0) hitbox_loop.top = SaveList[i].second;
		if (isCollide(object.hitboxTop, object.property, hitbox_loop)) return true;
	}
	return false;
}
std::vector<std::pair<float, float>> isCollideTopDetailed(const MovableObject& object, const std::vector<Obstacles>& OL, const std::vector<std::pair<float, float>>& SaveList = {}) {
	std::vector<std::pair<float, float>> result;
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (SaveList.size() > 0) hitbox_loop.top = SaveList[i].second;
		if (isCollide(object.hitboxTop, object.property, hitbox_loop)) result.push_back({ hitbox_loop.getPosition().x, hitbox_loop.getPosition().y });
	}
	return result;
}
std::pair<bool, std::pair<bool, bool>> isAccurateCollideTop(const MovableObject& object, const std::vector<Obstacles>& OL, float& CurrPosYCollide, bool& NoAdd, const std::vector<std::pair<float, float>>& SaveList = {}) {
	bool isCollideTopBool = false, isCollideRight = false, isCollideLeft = false;
	sf::FloatRect hitbox_loop;
	for (int i = 0; i < OL.size(); ++i) {
		hitbox_loop = getGlobalHitbox(OL[i].hitbox, OL[i].property);
		if (SaveList.size() > 0) hitbox_loop.top = SaveList[i].second;
		if (isCollide(object.hitboxTop, object.property, hitbox_loop)) {
			isCollideTopBool = true;
			if (!NoAdd) {
				CurrPosYCollide = hitbox_loop.getPosition().y;
				NoAdd = true;
			}
		}
		if (isCollide(object.hitboxRight, object.property, hitbox_loop)) isCollideRight = true;
		if (isCollide(object.hitboxLeft, object.property, hitbox_loop)) isCollideLeft = true;
		if (isCollideTopBool && isCollideRight && isCollideLeft) break;
	}
	return { isCollideTopBool, { isCollideRight, isCollideLeft } };
}