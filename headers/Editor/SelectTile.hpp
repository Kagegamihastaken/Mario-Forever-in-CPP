#ifndef SELECTTILE_HPP
#define SELECTTILE_HPP

#include "Editor/TabButton.hpp"

struct SelectTileData {
    std::string name;
    sf::Vector2f position;
    sf::Vector2i texPos = sf::Vector2i(0, 0);
    //other
    sf::Vector2f origin = sf::Vector2f(0.0f, 0.0f);
    int categoryID;
    int objectID;
    int customID1 = 0;
    int customID2 = 0;
};
extern bool EDITOR_SELECTTILE;
extern sf::Clock EDITOR_SELECTILE_CLOCK;
extern const std::array<std::vector<SelectTileData>, 4> TilePage;
extern std::array<MFCPP::TabButton, 4> TabList;

extern int CurrSelectTile;
extern int PrevSelectTile;
extern int CurrPage;
extern int PrevPage;
extern int PreviewPage;

extern const int LevelTab;

extern void SelectTileInit();
extern void SelectTilePosUpdate();
extern void SelectTileDraw();
extern void SelectTileAlphaUpdate(float dt);
extern void SelectTileDisplayUpdate();
extern int CheckExistPos();

#endif //SELECTTILE_HPP
