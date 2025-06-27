#include "Core/Tilemap.hpp"

namespace MFCPP {
    std::unordered_map<int, Tile> ObstaclesTilemap;
    std::unordered_map<int, Collectable> CollectableMap;
    static int tileMapSizeX, tileMapSizeY;
    static constexpr float tileSizeX = 32.f, tileSizeY = 32.f;
    void setTileMapSize(const float LevelX, const float LevelY) {
        ObstaclesTilemap.clear();
        CollectableMap.clear();
        tileMapSizeX = static_cast<int>(LevelX / tileSizeX) + 5;
        tileMapSizeY = static_cast<int>(LevelY / tileSizeY) + 5;
    }
    int getTilemap(const int x, const int y) {
        return x + y * tileMapSizeX;
    }

    void setIndexCollectableMapID(const float x, const float y, const unsigned val) {
        CollectableMap[getTilemap(static_cast<int>(x / tileSizeX), static_cast<int>(y / tileSizeY))].id = val;
    }
    unsigned getIndexCollectableMapID(const int x, const int y) {
        return CollectableMap[getTilemap(x, y)].id;
    }
    unsigned getIndexCollectableMapID(const float x, const float y) {
        return CollectableMap[getTilemap(static_cast<int>(x / tileSizeX), static_cast<int>(y / tileSizeY))].id;
    }
    void setIndexCollectableMapCollision(const float x, const float y, const bool val) {
        CollectableMap[getTilemap(static_cast<int>(x / tileSizeX), static_cast<int>(y / tileSizeY))].collide = val;
    }
    bool getIndexCollectableMapCollision(const int x, const int y) {
        return CollectableMap[getTilemap(x, y)].collide;
    }
    bool getIndexCollectableMapCollision(const float x, const float y) {
        return CollectableMap[getTilemap(static_cast<int>(x / tileSizeX), static_cast<int>(y / tileSizeY))].collide;
    }

    void setIndexTilemapCollision(const float x, const float y, const bool val) {
        ObstaclesTilemap[getTilemap(static_cast<int>(x / tileSizeX), static_cast<int>(y / tileSizeY))].collide = val;
    }
    bool getIndexTilemapCollision(const int x, const int y) {
        return ObstaclesTilemap[getTilemap(x, y)].collide;
    }
    bool getIndexTilemapCollision(const float x, const float y) {
        return ObstaclesTilemap[getTilemap(static_cast<int>(x / tileSizeX), static_cast<int>(y / tileSizeY))].collide;
    }
    void setIndexTilemapID(const float x, const float y, const unsigned val) {
        ObstaclesTilemap[getTilemap(static_cast<int>(x / tileSizeX), static_cast<int>(y / tileSizeY))].id = val;
    }
    unsigned getIndexTilemapID(const int x, const int y) {
        return ObstaclesTilemap[getTilemap(x, y)].id;
    }
    unsigned getIndexTilemapID(const float x, const float y) {
        return ObstaclesTilemap[getTilemap(static_cast<int>(x / tileSizeX), static_cast<int>(y / tileSizeY))].id;
    }
    void printTileMapSize() {
        std::cout << tileMapSizeX << " " << tileMapSizeY << "\n";
    }
    sf::Vector2f getTileSize() {
        return sf::Vector2f(tileSizeX, tileSizeY);
    }
}