#include "Core/Tilemap.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Logging.hpp"
namespace MFCPP {
    std::unordered_map<int32_t, Tilemap::Tile> Tilemap::ObstaclesTilemap;
    std::unordered_map<int32_t, Tilemap::Collectable> Tilemap::CollectableMap;
    int32_t Tilemap::tileMapSizeX, Tilemap::tileMapSizeY;
    void Tilemap::setTileMapSize(const float LevelX, const float LevelY) {
        ObstaclesTilemap.clear();
        CollectableMap.clear();
        tileMapSizeX = static_cast<int>(LevelX / tileSize) + TILEMAP_MAX;
        tileMapSizeY = static_cast<int>(LevelY / tileSize) + TILEMAP_MAX;
    }
    int32_t Tilemap::getTilemap(const int32_t x, const int32_t y) {
        return std::max(-TILEMAP_MAX, std::min(x, tileMapSizeX)) + std::max(-TILEMAP_MAX, std::min(y, tileMapSizeY)) * (tileMapSizeX + TILEMAP_MAX + 1);
    }
    sf::Vector2f Tilemap::getTilemap(const int32_t id) {
        return {static_cast<float>((id + TILEMAP_MAX) % (tileMapSizeX + TILEMAP_MAX + 1) - TILEMAP_MAX) * tileSize, static_cast<float>((id + TILEMAP_MAX) / (tileMapSizeX + TILEMAP_MAX + 1)) * tileSize};
    }
    void Tilemap::setIndexCollectableMapID(const float x, const float y, const uint32_t val) {
        CollectableMap[getTilemap(static_cast<int32_t>(std::floor(x / tileSize)), static_cast<int32_t>(std::floor(y / tileSize)))].id = val;
    }
    uint32_t Tilemap::getIndexCollectableMapID(const int32_t x, const int32_t y) {
        return CollectableMap[getTilemap(x, y)].id;
    }
    uint32_t Tilemap::getIndexCollectableMapID(const float x, const float y) {
        return CollectableMap[getTilemap(static_cast<int32_t>(std::floor(x / tileSize)), static_cast<int32_t>(std::floor(y / tileSize)))].id;
    }
    void Tilemap::setIndexCollectableMapCollision(const float x, const float y, const bool val) {
        CollectableMap[getTilemap(static_cast<int>(std::floor(x / tileSize)), static_cast<int32_t>(std::floor(y / tileSize)))].collide = val;
    }
    bool Tilemap::getIndexCollectableMapCollision(const int32_t x, const int32_t y) {
        return CollectableMap[getTilemap(x, y)].collide;
    }
    bool Tilemap::getIndexCollectableMapCollision(const float x, const float y) {
        return CollectableMap[getTilemap(static_cast<int32_t>(std::floor(x / tileSize)), static_cast<int32_t>(std::floor(y / tileSize)))].collide;
    }
    void Tilemap::setIndexTilemapCollision(const float x, const float y, const bool val) {
        ObstaclesTilemap[getTilemap(static_cast<int32_t>(std::floor(x / tileSize)), static_cast<int32_t>(std::floor(y / tileSize)))].collide = val;
    }
    bool Tilemap::getIndexTilemapCollision(const int32_t x, const int32_t y) {
        return ObstaclesTilemap[getTilemap(x, y)].collide;
    }
    bool Tilemap::getIndexTilemapCollision(const float x, const float y) {
        return ObstaclesTilemap[getTilemap(static_cast<int32_t>(std::floor(x / tileSize)), static_cast<int32_t>(std::floor(y / tileSize)))].collide;
    }
    void Tilemap::setIndexTilemapID(const float x, const float y, const uint32_t val) {
        ObstaclesTilemap[getTilemap(static_cast<int32_t>(std::floor(x / tileSize)), static_cast<int32_t>(std::floor(y / tileSize)))].id = val;
    }
    void Tilemap::setIndexTilemapFloorY(const float x, const float y, const std::pair<float, float> &val) {
        ObstaclesTilemap[getTilemap(static_cast<int32_t>(std::floor(x / tileSize)), static_cast<int32_t>(std::floor(y / tileSize)))].floorY = val;
    }
    std::pair<float, float> Tilemap::getIndexTilemapFloorY(const int32_t x, const int32_t y) {
        return ObstaclesTilemap[getTilemap(x, y)].floorY;
    }
    std::pair<float, float> Tilemap::getIndexTilemapFloorY(const float x, const float y) {
        return ObstaclesTilemap[getTilemap(static_cast<int32_t>(std::floor(x / tileSize)), static_cast<int32_t>(std::floor(y / tileSize)))].floorY;
    }
    uint32_t Tilemap::getIndexTilemapID(const int32_t x, const int32_t y) {
        return ObstaclesTilemap[getTilemap(x, y)].id;
    }
    uint32_t Tilemap::getIndexTilemapID(const float x, const float y) {
        return ObstaclesTilemap[getTilemap(static_cast<int32_t>(std::floor(x / tileSize)), static_cast<int32_t>(std::floor(y / tileSize)))].id;
    }
    void Tilemap::printTileMapSize() {
        Log::InfoPrint(fmt::format("{} {}", tileMapSizeX, tileMapSizeY));
    }
    float Tilemap::getTileSize() {
        return tileSize;
    }
    void Tilemap::drawHitboxMap() {
        for (const auto &i : ObstaclesTilemap) {
            if (i.second.collide)
                HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(sf::FloatRect({0.f, i.second.id != 3 ? i.second.floorY.first : 0.f}, {32.f, i.second.id != 3 ? i.second.floorY.second - i.second.floorY.first : 32.f}), getTilemap(i.first), sf::Color::Green));
        }
    }
}