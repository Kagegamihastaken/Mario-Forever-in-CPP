#pragma once
#ifndef BG_HPP
#define BG_HPP

extern void BgInit();
extern void AddBg(const std::string& name, const sf::Vector2f& parallax);
extern void BgUpdatePos();
extern void DeleteAllBg();
extern void BgDraw();

#endif