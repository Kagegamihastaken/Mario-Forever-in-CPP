#pragma once
// enum for skin

#ifndef ENUM_HPP
#define ENUM_HPP

enum BrickID { BRICK_NORMAL, BRICK_GRAY };
enum CoinID { COIN_NORMAL };
enum LuckyBlockID { LUCKY_BLOCK, TREE_LUCKY_BLOCK };

// enum for attribute
enum BrickAtt { NORMAL, MULTICOIN };
enum CoinAtt { ONE_COIN };
enum LuckyBlockAtt { LUCKY_COIN, LUCKY_MUSHROOM, LUCKY_FIRE_FLOWER, LUCKY_GREEN_MUSHROOM};

// IMPORTANT enum
enum ScoreID { SCORE_100, SCORE_200, SCORE_500, SCORE_1000, SCORE_2000, SCORE_5000, SCORE_10000, SCORE_1UP };
enum TextMarginID { LEFT_MARGIN, RIGHT_MARGIN };
enum AnimationDirection {ANIM_RIGHT, ANIM_LEFT};

//enum for GoombsAI
enum GoombaAIType { GOOMBA, MUSHROOM, KOOPA, SHELL, SHELL_MOVING, SPINY, FIRE_FLOWER };
enum GoombaAIDirection { RIGHT, LEFT };
enum GoombaAICollisionType { YES, NO, FULL };
enum GoombaAIBehaviour {GOOMBAAI_NORMAL, GOOMBAAI_REDKOOPA};
//enum for goombaAIEffect
enum GoombaAIEffectID { NONE, COLLIDE };
//enum for PiranhaAI
enum PiranhaID { GREEN, PINK };
enum PiranhaDirection {PIRANHA_UP, PIRANHA_DOWN, PIRANHA_RIGHT, PIRANHA_LEFT};
enum PiranhaProjectileType {PIRANHA_FIREBALL};
//enum for Spike
enum SpikeID { PIRANHA_GROUND, SPIKE_NORMAL };
//enum for BroAI
enum BroAIType {HAMMER_BRO};
enum BroAIMovementType {CAN_JUMP, CANNOT_JUMP};
enum BroAIProjectileType {HAMMER};
enum BroAIProjectileBehavior {HAMMER_BEHAVIOUR};
//enum for MarioProjectile
enum MarioProjectileType {FIREBALL};
enum MarioProjectileBehavior {FIREBALL_BEHAVIOUR};
//enum for Sound
enum SoundEnvironment {OVERWORLD, UNDERGROUND};
//enum for Bullet
enum BulletType {BULLET_NORMAL};

#endif // ENUM_HPP