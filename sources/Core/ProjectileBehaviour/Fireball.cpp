#include "Core/ProjectileBehaviour/Fireball.hpp"
#include "Core/Class/CollisionObjectClass.hpp"
#include "Core/Collision/Collide.hpp"
#include <SFML/System/Vector2.hpp>

#include "SFML/Graphics/Rect.hpp"

FireballData FireballX(const sf::Vector2f& pos, const float Xvelo, const float Yvelo, const bool direction, const float deltaTime, const sf::FloatRect& hitbox, const sf::Vector2f& origin) {
    FireballData data{Xvelo, Yvelo, pos.x, pos.y, false};
    if (direction) data.X -= data.Xvelo * deltaTime;
    else data.X += data.Xvelo * deltaTime;

    if (float PlatDistance; PlatformXCollision(MFCPP::CollisionObject(sf::Vector2f(data.X, data.Y), origin, hitbox), PlatDistance, data.Yvelo))
        data.X += PlatDistance;

    float CurrPosXCollide = 0, CurrPosYCollide = 0;
    const auto [fst, snd] = QuickCheckSideCollision(
        MFCPP::CollisionObject(sf::Vector2f(data.X, data.Y), origin, hitbox), direction,
        CurrPosXCollide, CurrPosYCollide);
    if (fst || snd)
        data.remove = true;
    return data;
}
FireballData FireballY(const sf::Vector2f& pos, const float Xvelo, const float Yvelo, const float deltaTime, const sf::FloatRect& hitbox, const sf::Vector2f& origin) {
    FireballData data{Xvelo, Yvelo, pos.x, pos.y, false};
    data.Y += data.Yvelo * deltaTime;
    data.Yvelo += (data.Yvelo >= 10.0f ? 0.0f : 1.f * deltaTime * 0.3f);
    if (data.Yvelo > 10.f) data.Yvelo = 10.f;

    if (float PlatPosY; PlatformYCollision(MFCPP::CollisionObject(sf::Vector2f(data.X, data.Y), origin, hitbox), PlatPosY, data.Yvelo, false)) {
        data.Y = PlatPosY;
        data.Yvelo = -5.f;
    }
    float CurrPosYCollide, CurrPosXCollide;
    //if (QuickCheckTopCollision(MFCPP::CollisionObject(sf::Vector2f(data.X, data.Y), origin, hitbox), CurrPosXCollide, CurrPosYCollide))
        //data.remove = true;
    if (QuickCheckBotCollision(MFCPP::CollisionObject(sf::Vector2f(data.X, data.Y), origin, hitbox), CurrPosXCollide, CurrPosYCollide)) {
        if (const float offset = data.Xvelo + 1.f; data.Yvelo >= -data.Xvelo) {
            const float floorY = GetCurrFloorY(sf::Vector2f(data.X, data.Y), CurrPosXCollide, CurrPosYCollide);
            if (data.Y < CurrPosYCollide + floorY - offset) return data;
            data.Y = CurrPosYCollide + floorY - (hitbox.size.y - origin.y);
            data.Yvelo = -5.f;
        }
    }
    return data;
}