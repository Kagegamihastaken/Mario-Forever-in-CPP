#pragma once
// enum for skin
enum BrickID { BRICK_NORMAL, BRICK_GRAY };
enum CoinID { COIN_NORMAL };
enum LuckyBlockID { LUCKY_COIN, TREE_LUCKY_COIN };

// enum for attribute
enum BrickAtt { NORMAL, MULTICOIN };
enum CoinAtt { ONE_COIN, THREE_COIN };
enum LuckyBlockAtt { COIN };

// IMPORTANT enum
enum ScoreID { SCORE_100, SCORE_200, SCORE_500, SCORE_1000, SCORE_2000, SCORE_5000, SCORE_10000, SCORE_1UP };
enum TextMarginID { LEFT_MARGIN, RIGHT_MARGIN };

enum GoombaAIType { GOOMBA };
enum GoombaAIDirection { LEFT, RIGHT };
enum GoombaAIHittable { YES, NO, FULL };

#ifndef ENUM_HPP
#define ENUM_HPP

#endif // ENUM_HPP