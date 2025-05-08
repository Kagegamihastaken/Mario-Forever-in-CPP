#pragma once
#ifndef BG_HPP
#define BG_HPP

extern void BgInit();
extern void AddBg(int type, int parallax);
extern void BgUpdatePos();
extern void DeleteAllBg();
extern void BgDraw();

#endif