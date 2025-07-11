#ifndef EDITOR_HPP
#define EDITOR_HPP

#include "SFML/Window/Event.hpp"

extern sf::Vector2f EditorPos;
extern sf::Vector2f EditorInterpolatedPos;

extern void AlphaUpdate(float& alpha, bool& state, float min, float max, float change, float dt);
extern void PlaceTile();
extern void DrawTile();
extern void EditorScreenMove(float dt);
extern void SetPrevEditor();
extern void InterpolateEditorPos(float alpha);
extern void EditorEvent(const std::optional<sf::Event>& event);
extern void TilePosUpdate(float dt);
extern void SelectedTilePosUpdate();
extern void EditorInit();

#endif //EDITOR_HPP
