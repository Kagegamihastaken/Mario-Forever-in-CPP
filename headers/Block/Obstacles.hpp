#pragma once

#ifndef OBSTACLES_HPP
#define OBSTACLES_HPP

extern sf::VertexArray ObstaclesVA;
extern sf::RenderTexture ObstacleRTexture;
extern std::vector<std::array<int, 6>> ID_list;
extern void loadObstacleRes();
extern void ObstaclesDraw();
#endif // OBSTACLES_HPP