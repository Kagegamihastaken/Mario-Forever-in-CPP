#ifndef MFCPP_CHECKPOINT_HPP
#define MFCPP_CHECKPOINT_HPP

extern void CheckpointInit();
extern void AddCheckpoint(const sf::Vector2f& pos);
extern void CheckpointAnimationUpdate();
extern void CheckpointCollision();
extern void CheckpointDraw();
extern void setStartPosition(const sf::Vector2f& pos);
extern sf::Vector2f getStartPosition();

#endif //MFCPP_CHECKPOINT_HPP