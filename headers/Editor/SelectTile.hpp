#ifndef SELECTTILE_HPP
#define SELECTTILE_HPP

extern bool EDITOR_SELECTTILE;
extern sf::Clock EDITOR_SELECTILE_CLOCK;
extern void SelectTileInit();
extern void SelectTilePosUpdate();
extern void SelectTileDraw();
extern void SelectTileAlphaUpdate(float dt);
extern void SelectTileDisplayUpdate();
extern int CheckExistPos();

#endif //SELECTTILE_HPP
