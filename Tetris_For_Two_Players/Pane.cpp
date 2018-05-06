#ifndef __TET_H__
#define __TET_H__ 1
#include"tetris.h"
#endif

Pane::Pane(int y, int x, int h, int w) :y_(y), x_(x),height_(h),width_(w)
{
 	win_ = newwin(h, w, y, x);
	box(win_, 0, 0);
 	wrefresh(win_);
}

void Pane::draw()
{
 	box(win_, 0, 0);
 	wrefresh(win_);
}
