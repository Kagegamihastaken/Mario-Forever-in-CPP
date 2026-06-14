#pragma once
// enum for skin

#ifndef ENUM_HPP
#define ENUM_HPP

enum class BrickID : uint_fast8_t { BRICK_NORMAL, BRICK_GRAY };
enum class CoinID : uint_fast8_t { COIN_NORMAL };
enum class LuckyBlockID : uint_fast8_t { LUCKY_BLOCK, TREE_LUCKY_BLOCK };

// enum for attribute
enum class BrickAtt : uint_fast8_t { NORMAL, MULTICOIN };
enum class CoinAtt : uint_fast8_t { ONE_COIN };
enum class LuckyBlockAtt : uint_fast8_t { LUCKY_COIN, LUCKY_MUSHROOM, LUCKY_FIRE_FLOWER, LUCKY_GREEN_MUSHROOM, LUCKY_BEETROOT};

// IMPORTANT enum
enum class ScoreID : uint_fast8_t { SCORE_100, SCORE_200, SCORE_500, SCORE_1000, SCORE_2000, SCORE_5000, SCORE_10000, SCORE_1UP };
enum class TextMarginID : uint_fast8_t { LEFT_MARGIN, RIGHT_MARGIN };
enum AnimationDirection {ANIM_RIGHT, ANIM_LEFT};

//enum for GoombsAI
enum class GoombaAIType : uint_fast8_t { GOOMBA, MUSHROOM, KOOPA, SHELL, SHELL_MOVING, SPINY, FIRE_FLOWER, KOOPA_PARATROOPA };
enum class GoombaAIDirection : uint_fast8_t { RIGHT, LEFT };
enum class GoombaAICollisionType : uint_fast8_t { YES, NO, FULL };
enum class GoombaAIBehaviour : uint_fast8_t {GOOMBAAI_NORMAL, GOOMBAAI_REDKOOPA};
//enum for goombaAIEffect
enum class GoombaAIEffectID : uint_fast8_t { NONE, COLLIDE };
//enum for PiranhaAI
enum class PiranhaID : uint_fast8_t { GREEN, PINK };
enum class PiranhaDirection : uint_fast8_t {PIRANHA_UP, PIRANHA_DOWN, PIRANHA_RIGHT, PIRANHA_LEFT};
enum class PiranhaProjectileType : uint_fast8_t {PIRANHA_FIREBALL};
//enum for Spike
enum class SpikeID : uint_fast8_t { PIRANHA_GROUND, SPIKE_NORMAL };
//enum for BroAI
enum class BroAIType : uint_fast8_t {HAMMER_BRO, FIRE_BRO};
enum class BroAIMovementType : uint_fast8_t {CAN_JUMP, CANNOT_JUMP};
enum class BroAIProjectileType : uint_fast8_t {BROAI_HAMMER, BROAI_FIREBALL};
enum class BroAIProjectileBehavior : uint_fast8_t {BROAI_HAMMER_BEHAVIOUR, BROAI_FIREBALL_BEHAVIOUR};
//enum for MarioProjectile
enum class MarioProjectileType : uint_fast8_t {FIREBALL};
enum class MarioProjectileBehavior : uint_fast8_t {FIREBALL_BEHAVIOUR};
//enum for Sound
enum class SoundEnvironment : uint_fast8_t {OVERWORLD, UNDERGROUND};
//enum for Bullet
enum class BulletType : uint_fast8_t {BULLET_NORMAL, BULLET_NORMAL_FLIPPED, FIRE_LAUNCHER, FIRE_LAUNCHER_FLIPPED, FIRE_LAUNCHER_STAND};

#endif // ENUM_HPP