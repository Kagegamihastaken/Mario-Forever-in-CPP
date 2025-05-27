#ifndef GAME_HPP
#define GAME_HPP

extern void GameObjectInit();
extern void GameTextInit();
extern void GameLoadLevel();
extern void GameObjectEditText();
extern void GameObjectSetPrev();
extern void GameObjectDeltaMovement(float dt);
extern void GameObjectInterpolateMovement(float alpha);
extern void GameObjectCollisionNAnimation();
extern void GameObjectMiscUpdate();
extern void GameObjectDraw();
extern void GameObjectEditorUpdate();

#endif //GAME_HPP
