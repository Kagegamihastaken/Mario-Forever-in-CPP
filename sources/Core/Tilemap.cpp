#include "Core/Tilemap.hpp"

#include <boost/unordered/unordered_flat_map.hpp>
#include "Core/HitboxUtils.hpp"
#include "Core/Logging.hpp"
namespace MFCPP {
    struct Tile {
        bool collide = false;
        uint32_t id = 0;
        std::pair<uint8_t, uint8_t> floorY = {0.f, 0.f};
        std::pair<uint8_t, uint8_t> xBounds = {0.f, 0.f};
    };
    struct Collectable {
        bool collide = false;
        uint32_t id = 0;
    };
    boost::unordered_flat_map<int32_t, Tile> ObstaclesTilemap;
    boost::unordered_flat_map<int32_t, Collectable> CollectableMap;
    int32_t tileMapSizeX, tileMapSizeY;
    constexpr float tileSize = 32.f;
    constexpr int32_t TILEMAP_MAX = 5;

    void Tilemap::setTileMapSize(float LevelX, float LevelY) {
        ObstaclesTilemap.clear();
        CollectableMap.clear();
        tileMapSizeX = static_cast<int>(LevelX / tileSize) + TILEMAP_MAX;
        tileMapSizeY = static_cast<int>(LevelY / tileSize) + TILEMAP_MAX;
    }
    int32_t Tilemap::getTilemap(const int32_t x, const int32_t y) {
        return std::max(-TILEMAP_MAX, std::min(x, tileMapSizeX)) + std::max(-TILEMAP_MAX, std::min(y, tileMapSizeY)) * (tileMapSizeX + TILEMAP_MAX + 1);
    }
    sf::Vector2f Tilemap::getTilemap(const int32_t id) {
        return {static_cast<float>((id + TILEMAP_MAX) % (tileMapSizeX + TILEMAP_MAX + 1) - TILEMAP_MAX) * tileSize, (static_cast<float>(id + TILEMAP_MAX) / static_cast<float>(tileMapSizeX + TILEMAP_MAX + 1)) * tileSize};
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
        CollectableMap[getTilemap(static_cast<int32_t>(std::floor(x / tileSize)), static_cast<int32_t>(std::floor(y / tileSize)))].collide = val;
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
    void Tilemap::setIndexTilemapXBound(const float x, const float y, const std::pair<float, float> &val) {
        ObstaclesTilemap[getTilemap(static_cast<int32_t>(std::floor(x / tileSize)), static_cast<int32_t>(std::floor(y / tileSize)))].xBounds = val;
    }
    std::pair<float, float> Tilemap::getIndexTilemapFloorY(const int32_t x, const int32_t y) {
        return ObstaclesTilemap[getTilemap(x, y)].floorY;
    }
    std::pair<float, float> Tilemap::getIndexTilemapFloorY(const float x, const float y) {
        return ObstaclesTilemap[getTilemap(static_cast<int32_t>(std::floor(x / tileSize)), static_cast<int32_t>(std::floor(y / tileSize)))].floorY;
    }
    std::pair<float, float> Tilemap::getIndexTilemapXBound(const float x, const float y) {
        return ObstaclesTilemap[getTilemap(static_cast<int32_t>(std::floor(x / tileSize)), static_cast<int32_t>(std::floor(y / tileSize)))].xBounds;
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
                HitboxUtils::addHitboxDebug(HitboxUtils::HitboxDetail(sf::FloatRect({static_cast<float>(i.second.xBounds.first), i.second.id != 3 ? static_cast<float>(i.second.floorY.first) : 0.f}, {static_cast<float>(i.second.xBounds.second - i.second.xBounds.first), i.second.id != 3 ? static_cast<float>(i.second.floorY.second - i.second.floorY.first) : 32.f}), getTilemap(i.first), sf::Color::Green));
        }
    }
    void Tilemap::deleteAll() {
        ObstaclesTilemap.clear();
        CollectableMap.clear();
    }
}