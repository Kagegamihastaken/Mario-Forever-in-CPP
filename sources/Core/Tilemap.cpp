#include "Core/Tilemap.hpp"

#include "Core/HitboxUtils.hpp"
#include "Core/Logging.hpp"
constexpr int TILEMAP_MAX = 5;
namespace MFCPP {
    std::unordered_map<int, Tile> ObstaclesTilemap;
    std::unordered_map<int, Collectable> CollectableMap;
    static int tileMapSizeX, tileMapSizeY;
    static constexpr float tileSize = 32.f;
    void setTileMapSize(const float LevelX, const float LevelY) {
        ObstaclesTilemap.clear();
        CollectableMap.clear();
        tileMapSizeX = static_cast<int>(LevelX / tileSize) + TILEMAP_MAX;
        tileMapSizeY = static_cast<int>(LevelY / tileSize) + TILEMAP_MAX;
    }
    int getTilemap(const int x, const int y) {
        return std::max(-TILEMAP_MAX, std::min(x, tileMapSizeX)) + std::max(-TILEMAP_MAX, std::min(y, tileMapSizeY)) * (tileMapSizeX + TILEMAP_MAX + 1);
    }
    sf::Vector2f getTilemap(const int id) {
        return {static_cast<float>((id + TILEMAP_MAX) % (tileMapSizeX + TILEMAP_MAX + 1) - TILEMAP_MAX) * tileSize, static_cast<float>((id + TILEMAP_MAX) / (tileMapSizeX + TILEMAP_MAX + 1)) * tileSize};
    }
    void setIndexCollectableMapID(const float x, const float y, const unsigned val) {
        CollectableMap[getTilemap(static_cast<int>(std::floor(x / tileSize)), static_cast<int>(std::floor(y / tileSize)))].id = val;
    }
    unsigned getIndexCollectableMapID(const int x, const int y) {
        return CollectableMap[getTilemap(x, y)].id;
    }
    unsigned getIndexCollectableMapID(const float x, const float y) {
        return CollectableMap[getTilemap(static_cast<int>(std::floor(x / tileSize)), static_cast<int>(std::floor(y / tileSize)))].id;
    }
    void setIndexCollectableMapCollision(const float x, const float y, const bool val) {
        CollectableMap[getTilemap(static_cast<int>(std::floor(x / tileSize)), static_cast<int>(std::floor(y / tileSize)))].collide = val;
    }
    bool getIndexCollectableMapCollision(const int x, const int y) {
        return CollectableMap[getTilemap(x, y)].collide;
    }
    bool getIndexCollectableMapCollision(const float x, const float y) {
        return CollectableMap[getTilemap(static_cast<int>(std::floor(x / tileSize)), static_cast<int>(std::floor(y / tileSize)))].collide;
    }

    void setIndexTilemapCollision(const float x, const float y, const bool val) {
        ObstaclesTilemap[getTilemap(static_cast<int>(std::floor(x / tileSize)), static_cast<int>(std::floor(y / tileSize)))].collide = val;
    }
    bool getIndexTilemapCollision(const int x, const int y) {
        return ObstaclesTilemap[getTilemap(x, y)].collide;
    }
    bool getIndexTilemapCollision(const float x, const float y) {
        return ObstaclesTilemap[getTilemap(static_cast<int>(std::floor(x / tileSize)), static_cast<int>(std::floor(y / tileSize)))].collide;
    }
    void setIndexTilemapID(const float x, const float y, const unsigned val) {
        ObstaclesTilemap[getTilemap(static_cast<int>(std::floor(x / tileSize)), static_cast<int>(std::floor(y / tileSize)))].id = val;
    }
    void setIndexTilemapFloorY(const float x, const float y, const std::pair<float, float> &val) {
        ObstaclesTilemap[getTilemap(static_cast<int>(std::floor(x / tileSize)), static_cast<int>(std::floor(y / tileSize)))].floorY = val;
    }
    std::pair<float, float> getIndexTilemapFloorY(const float x, const float y) {
        return ObstaclesTilemap[getTilemap(static_cast<int>(std::floor(x / tileSize)), static_cast<int>(std::floor(y / tileSize)))].floorY;
    }
    unsigned getIndexTilemapID(const int x, const int y) {
        return ObstaclesTilemap[getTilemap(x, y)].id;
    }
    unsigned getIndexTilemapID(const float x, const float y) {
        return ObstaclesTilemap[getTilemap(static_cast<int>(std::floor(x / tileSize)), static_cast<int>(std::floor(y / tileSize)))].id;
    }
    void printTileMapSize() {
        Log::InfoPrint(fmt::format("{} {}", tileMapSizeX, tileMapSizeY));
    }
    float getTileSize() {
        return tileSize;
    }
    void drawHitboxMap() {
        for (const auto &i : ObstaclesTilemap) {
            if (i.second.collide) HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(sf::FloatRect({0.f, 0.f}, {32.f, 32.f}), getTilemap(i.first), sf::Color::Green));
        }
    }
}