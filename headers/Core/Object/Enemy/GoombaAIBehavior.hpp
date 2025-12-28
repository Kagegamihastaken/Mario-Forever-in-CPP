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

    static GoombaAIData GoombaAIEffectYMove(const GoombaAIData& data, float deltaTime);
    static void GoombaAICollision(MFCPP::Enemy* EnemyA, MFCPP::Enemy* EnemyB);
    static void GoombaAIStomping();

    static bool GoombaAIEffectDisappearing(float& clock, float& alpha, float deltaTime);
};

#endif //MFCPP_GOOMBAAIBEHAVIOR_HPP