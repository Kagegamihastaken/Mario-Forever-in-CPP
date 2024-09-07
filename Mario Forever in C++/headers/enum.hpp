#pragma once
// enum for skin
enum BrickID { BRICK_NORMAL, BRICK_GRAY };
enum CoinID { COIN_NORMAL };
enum LuckyBlockID { LUCKY_BLOCK, TREE_LUCKY_BLOCK };

// enum for attribute
enum BrickAtt { NORMAL, MULTICOIN };
enum CoinAtt { ONE_COIN };
enum LuckyBlockAtt { LUCKY_COIN, LUCKY_MUSHROOM };

// IMPORTANT enum
enum ScoreID { SCORE_100, SCORE_200, SCORE_500, SCORE_1000, SCORE_2000, SCORE_5000, SCORE_10000, SCORE_1UP };
enum TextMarginID { LEFT_MARGIN, RIGHT_MARGIN };

enum GoombaAIType { GOOMBA, MUSHROOM, GREEN_KOOPA };
enum GoombaAIDirection { LEFT, RIGHT };
enum GoombaAIHittable { YES, NO, FULL };

enum GoombaAIEffectID { NONE, COLLIDE };

#ifndef ENUM_HPP
#define ENUM_HPP

#endif // ENUM_HPP