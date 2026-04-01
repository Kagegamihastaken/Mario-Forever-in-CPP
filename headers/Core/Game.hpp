#ifndef GAME_HPP
#define GAME_HPP

extern void GameObjectInit();
extern void GameObjectEditText();
extern void GameObjectSetPrev();
extern void GameObjectDeltaMovement(float dt);
extern void GameObjectInterpolateMovement(float alpha);
extern void GameObjectCollision();
extern void GameObjectMiscUpdate();
extern void GameObjectDraw();
extern void GameObjectRetrieveEvent(const std::optional<sf::Event>& event);
extern void GameCleanUp();
extern void GameObjectUpdateView();

#endif //GAME_HPP
