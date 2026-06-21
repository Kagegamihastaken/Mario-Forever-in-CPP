#ifndef TILEMAP_HPP
#define TILEMAP_HPP

namespace MFCPP {
    class Tilemap final {
        struct Tile {
            bool collide = false;
            uint32_t id = 0;
            std::pair<float, float> floorY = {0.f, 0.f};
        };
        struct Collectable {
            bool collide = false;
            uint32_t id = 0;
        };
        static constexpr int32_t TILEMAP_MAX = 5;
        static std::unordered_map<int32_t, Tile> ObstaclesTilemap;
        static std::unordered_map<int32_t, Collectable> CollectableMap;
        static int32_t tileMapSizeX, tileMapSizeY;
        static constexpr float tileSize = 32.f;
    public:
        Tilemap() = delete;
        Tilemap(const Tilemap&) = delete;
        Tilemap(Tilemap&&) = delete;
        Tilemap& operator=(const Tilemap&) = delete;
        Tilemap& operator=(Tilemap&&) = delete;

        static void setIndexCollectableMapID(float x, float y, uint32_t val);
        static uint32_t getIndexCollectableMapID(int32_t x, int32_t y);
        static uint32_t getIndexCollectableMapID(float x, float y);
        static void setIndexCollectableMapCollision(float x, float y, bool val);
        static bool getIndexCollectableMapCollision(int32_t x, int32_t y);
        static bool getIndexCollectableMapCollision(float x, float y);

        static void setTileMapSize(float LevelX, float LevelY);
        static void printTileMapSize();
        static int32_t getTilemap(int32_t x, int32_t y);
        static sf::Vector2f getTilemap(int id);
        static void setIndexTilemapCollision(float x, float y, bool val);
        static bool getIndexTilemapCollision(float x, float y);
        static bool getIndexTilemapCollision(int32_t x, int32_t y);
        static void setIndexTilemapID(float x, float y, uint32_t val);
        static uint32_t getIndexTilemapID(int32_t x, int32_t y);
        static uint32_t getIndexTilemapID(float x, float y);
        static void setIndexTilemapFloorY(float x, float y, const std::pair<float, float> &val);
        static std::pair<float, float> getIndexTilemapFloorY(float x, float y);
        static std::pair<float, float> getIndexTilemapFloorY(int32_t x, int32_t y);
        static float getTileSize();
        static void drawHitboxMap();
    };
}
#endif //TILEMAP_HPP
