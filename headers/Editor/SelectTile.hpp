#ifndef SELECTTILE_HPP
#define SELECTTILE_HPP

#include <SFML/System/Clock.hpp>
#include "Editor/TabButton.hpp"

struct SelectTileData {
    std::string name;
    sf::Vector2f position;
};
extern bool EDITOR_SELECTTILE;
extern sf::Clock EDITOR_SELECTILE_CLOCK;
extern std::array<std::vector<SelectTileData>, 2> TilePage;
extern std::array<MFCPP::TabButton, 2> TabList;

extern int CurrSelectTile;
extern int PrevSelectTile;
extern int CurrPage;
extern int PrevPage;
extern int PreviewPage;

extern void SelectTileInit();
extern void SelectTilePosUpdate();
extern void SelectTileDraw();
extern void SelectTileAlphaUpdate(float dt);
extern void SelectTileDisplayUpdate();
extern int CheckExistPos();

#endif //SELECTTILE_HPP
