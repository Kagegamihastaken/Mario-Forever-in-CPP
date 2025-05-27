#ifndef EDITOR_HPP
#define EDITOR_HPP

extern sf::Vector2f EditorPos;
extern sf::Vector2f EditorInterpolatedPos;
extern void PlaceTile();
extern void DrawTile();
extern void EditorScreenMove(float dt);
extern void SetPrevEditor();
extern void InterpolateEditorPos(float alpha);

#endif //EDITOR_HPP
