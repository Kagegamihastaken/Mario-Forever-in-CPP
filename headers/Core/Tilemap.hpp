#ifndef TILEMAP_HPP
#define TILEMAP_HPP

namespace MFCPP {
    struct Tile {
        bool collide = false;
        unsigned id = 0;
        std::pair<float, float> floorY = {0.f, 0.f};
    };
    struct Collectable {
        bool collide = false;
        unsigned id = 0;
    };
    extern void setIndexCollectableMapID(float x, float y, unsigned val);
    extern unsigned getIndexCollectableMapID(int x, int y);
    extern unsigned getIndexCollectableMapID(float x, float y);
    extern void setIndexCollectableMapCollision(float x, float y, bool val);
    extern bool getIndexCollectableMapCollision(int x, int y);
    extern bool getIndexCollectableMapCollision(float x, float y);

    extern void setTileMapSize(float LevelX, float LevelY);
    extern void printTileMapSize();
    extern int getTilemap(int x, int y);
    extern void setIndexTilemapCollision(float x, float y, bool val);
    extern bool getIndexTilemapCollision(float x, float y);
    extern bool getIndexTilemapCollision(int x, int y);
    extern void setIndexTilemapID(float x, float y, unsigned val);
    extern unsigned getIndexTilemapID(int x, int y);
    extern unsigned getIndexTilemapID(float x, float y);
    extern void setIndexTilemapFloorY(float x, float y, std::pair<float, float> val);
    extern std::pair<float, float> getIndexTilemapFloorY(float x, float y);
    extern float getTileSize();
}
#endif //TILEMAP_HPP
