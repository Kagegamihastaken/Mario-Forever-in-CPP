#include "Core/Utility.hpp"

#include "Core/Tilemap.hpp"
#include "Core/Collision/Collide.hpp"

std::random_device seed;

int32_t Utility::RandomIntNumberGenerator(const int32_t a, const int32_t b) {
    std::uniform_int_distribution dis(a, b);
    return dis(seed);
}
float Utility::RandomFloatNumberGenerator(const float a, const float b) {
    std::uniform_real_distribution dis(a, b);
    return dis(seed);
}
float Utility::f_mod(const float a, const float b) { return a - std::floor(a / b) * b;}
float Utility::f_abs(const float a) { return a < 0 ? -a : a; }
float Utility::f_round(const float val) {
    if (const float fl = std::trunc(val); val - fl < 0.5f) return fl;
    else return fl+1.0f;
}
float Utility::VectorGetLength(const sf::Vector2f& val) {
    return std::sqrt(val.x * val.x + val.y * val.y);
}
sf::Vector2f Utility::VectorNormalized(const sf::Vector2f& val) {
    if (const float len = VectorGetLength(val); len > 0)
        return {val.x / len, val.y / len};
    return {0.f, 0.f};
}
int Utility::hexToInt(const std::string &hex) {
    return std::stoi(hex, nullptr, 16);
}

float Utility::speedNerf(const MFCPP::CollisionObject& collision, const sf::Vector2f& velocity, float offset, bool direction) {
    float CurrPosXCollide = 0, CurrPosYCollide = 0;
    float spd_nerf = 1.f;
    if (QuickCheckBotCollision(
            collision, offset, CurrPosXCollide, CurrPosYCollide)) {

        if (velocity.y >= -velocity.x) {
            const float floorY = GetCurrFloorY(collision.GetPosition(), CurrPosXCollide, CurrPosYCollide);
            const float t = getInterpolationFloorY(collision.GetPosition(), CurrPosXCollide, CurrPosYCollide);
            if (t >= 0.f && t <= 1.f) {
                if (collision.GetPosition().y >= CurrPosYCollide + floorY - offset) {
                    if (MFCPP::Tilemap::getIndexTilemapID(CurrPosXCollide, CurrPosYCollide) == 3) {
                        bool BlockDirection = MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide, CurrPosYCollide).first <
                                              MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide, CurrPosYCollide).second;
                        if (BlockDirection ^ direction) {
                            std::pair<float, float> CurrentfloorY = MFCPP::Tilemap::getIndexTilemapFloorY(CurrPosXCollide, CurrPosYCollide);
                            std::pair<float, float> CurrentboundX = MFCPP::Tilemap::getIndexTilemapXBound(CurrPosXCollide, CurrPosYCollide);
                            spd_nerf = std::abs(std::cos(std::atan((CurrentfloorY.second - CurrentfloorY.first) / (CurrentboundX.second - CurrentboundX.first))));
                        }
                    }
                }
            }
        }
    }
    return spd_nerf;
}