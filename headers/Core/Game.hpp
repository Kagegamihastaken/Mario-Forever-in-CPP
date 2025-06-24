#ifndef GAME_HPP
#define GAME_HPP

extern void GameObjectInit();
extern void GameTextInit();
extern void GameLoadLevel();
extern void GameObjectEditText();
extern void GameObjectSetPrev();
extern void GameObjectDeltaMovement(float dt);
extern void GameObjectInterpolateMovement(float alpha);
extern void GameObjectCollision();
extern void GameObjectMiscUpdate();
extern void GameObjectDraw();
extern void GameObjectEditorUpdate();
extern void GameObjectAnimation();
extern void GameObjectRetrieveEvent(const std::optional<sf::Event>& event);
extern void GameCleanUp();

#endif //GAME_HPP
