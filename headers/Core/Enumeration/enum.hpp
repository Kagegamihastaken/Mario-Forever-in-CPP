#pragma once
// enum for skin

#ifndef ENUM_HPP
#define ENUM_HPP

enum class BrickID : uint8_t { NORMAL, GRAY };
enum class CoinID : uint8_t { NORMAL };
enum class LuckyBlockID : uint8_t { NORMAL, TREE };

// enum for attribute
enum class BrickAtt : uint8_t { NORMAL, MULTICOIN };
enum class CoinAtt : uint8_t { ONE_COIN };
enum class LuckyBlockAtt : uint8_t { COIN, MUSHROOM, FIRE_FLOWER, GREEN_MUSHROOM, BEETROOT};

// IMPORTANT enum
enum class ScoreID : uint8_t { SCORE_100, SCORE_200, SCORE_500, SCORE_1000, SCORE_2000, SCORE_5000, SCORE_10000, SCORE_1UP };
enum class TextMarginID : uint8_t { LEFT_MARGIN, RIGHT_MARGIN };
enum AnimationDirection {ANIM_RIGHT, ANIM_LEFT};

//enum for GoombsAI
enum class GoombaAIType : uint8_t { GOOMBA, MUSHROOM, KOOPA, SHELL, SHELL_MOVING, SPINY, FIRE_FLOWER, KOOPA_PARATROOPA };
enum class GoombaAIDirection : uint8_t { RIGHT, LEFT };
enum class GoombaAICollisionType : uint8_t { STOMP, SPIKE, FULL };
enum class GoombaAIBehaviour : uint8_t {NORMAL, REDKOOPA};
//enum for goombaAIEffect
enum class GoombaAIEffectID : uint8_t { NONE, COLLIDABLE };
//enum for PiranhaAI
enum class PiranhaID : uint8_t { GREEN, PINK };
enum class PiranhaDirection : uint8_t {UP, DOWN, RIGHT, LEFT};
enum class PiranhaProjectileType : uint8_t {FIREBALL};
//enum for Spike
enum class SpikeID : uint8_t {PIRANHA_GROUND, SPIKE_NORMAL};
//enum for BroAI
enum class BroAIType : uint8_t {HAMMER_BRO, FIRE_BRO};
enum class BroAIMovementType : uint8_t {CAN_JUMP, CANNOT_JUMP};
enum class BroAIProjectileType : uint8_t {HAMMER, FIREBALL};
enum class BroAIProjectileBehavior : uint8_t {HAMMER, FIREBALL};
//enum for MarioProjectile
enum class MarioProjectileType : uint8_t {FIREBALL};
enum class MarioProjectileBehavior : uint8_t {FIREBALL};
//enum for Sound
enum class SoundEnvironment : uint8_t {OVERWORLD, UNDERGROUND};
//enum for Bullet
enum class BulletType : uint8_t {BULLET_NORMAL, BULLET_NORMAL_FLIPPED, FIRE_LAUNCHER, FIRE_LAUNCHER_FLIPPED, FIRE_LAUNCHER_STAND};

#endif // ENUM_HPP