#ifndef TILEMAP_HPP
#define TILEMAP_HPP

namespace MFCPP {
    namespace Tilemap{
        void setIndexCollectableMapID(float x, float y, uint32_t val);
        uint32_t getIndexCollectableMapID(int32_t x, int32_t y);
        uint32_t getIndexCollectableMapID(float x, float y);
        void setIndexCollectableMapCollision(float x, float y, bool val);
        bool getIndexCollectableMapCollision(int32_t x, int32_t y);
        bool getIndexCollectableMapCollision(float x, float y);

        void setTileMapSize(float LevelX, float LevelY);
        void printTileMapSize();
        int32_t getTilemap(int32_t x, int32_t y);
        sf::Vector2f getTilemap(int id);
        void setIndexTilemapCollision(float x, float y, bool val);
        bool getIndexTilemapCollision(float x, float y);
        bool getIndexTilemapCollision(int32_t x, int32_t y);
        void setIndexTilemapID(float x, float y, uint32_t val);
        uint32_t getIndexTilemapID(int32_t x, int32_t y);
        uint32_t getIndexTilemapID(float x, float y);
        void setIndexTilemapFloorY(float x, float y, const std::pair<float, float> &val);
        std::pair<float, float> getIndexTilemapFloorY(float x, float y);
        std::pair<float, float> getIndexTilemapFloorY(int32_t x, int32_t y);
        float getTileSize();
        void drawHitboxMap();
    };
}
#endif //TILEMAP_HPP
