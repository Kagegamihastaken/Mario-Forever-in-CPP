#ifndef MFCPP_GOOMBAAIBEHAVIOR_HPP
#define MFCPP_GOOMBAAIBEHAVIOR_HPP

#include "Core/Object/Enemy.hpp"
#include "SFML/System/Vector2.hpp"

class GoombaAIBehavior {
public:
    struct GoombaAIData {
        sf::Vector2f position, velocity;
        bool direction;
    };
    static GoombaAIData GoombaAIYMove(const GoombaAIData& data, float deltaTime);
    static GoombaAIData GoombaAIYCollision(const GoombaAIData& data, const sf::FloatRect& hitbox, const sf::Vector2f& origin, bool RedKoopaAIEnabled = false, float prevX = 0.f);
    static GoombaAIData GoombaAIXMove(const GoombaAIData& data, float deltaTime);
    static GoombaAIData GoombaAIXCollision(const GoombaAIData& data, const sf::FloatRect& hitbox, const sf::FloatRect& hitbox_wall, const sf::Vector2f& origin);
    static GoombaAIData ShellXCollision(const GoombaAIData& data, const sf::FloatRect& hitbox, const sf::FloatRect& hitbox_wall, const sf::Vector2f& origin);

    static GoombaAIData GoombaAIEffectYMove(const GoombaAIData& data, float deltaTime);
    static void GoombaAICollision(MFCPP::Enemy* EnemyA, MFCPP::Enemy* EnemyB);

    static void ShellMovingCollision(MFCPP::Enemy* Shell, MFCPP::Enemy* Other, unsigned short& val);

    static void GoombaAIStomping();

    static bool GoombaAIEffectDisappearing(float& clock, float& alpha, float deltaTime);
    static GoombaAIData MushroomAppearing(const GoombaAIData& data, float& appearY, float appearYSpeed, float appearYMax, bool& isAppearing, bool isDisabled, float deltaTime);
private:
    static GoombaAIData EnemyPlatformXCollision(const GoombaAIData &data, const sf::FloatRect &hitbox, const sf::Vector2f &origin);
    static GoombaAIData EnemyAdjustXCollision(const GoombaAIData &data, const sf::FloatRect &hitbox, const sf::Vector2f &origin, float CurrPosXCollide, bool side);
    static short unsigned int Kicking(const sf::Vector2f& pos, const sf::Vector2f& origin, short unsigned int val);
};

#endif //MFCPP_GOOMBAAIBEHAVIOR_HPP