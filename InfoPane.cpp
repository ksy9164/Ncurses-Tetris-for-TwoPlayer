#ifndef __TET_H__
#define __TET_H__ 1
#include"tetris.h"
#endif

void InfoPane::draw()
{
	mvwprintw(win_, 0,38, "  Tetris Instruction  ");
	mvwprintw(win_, 2,33, "  This is Tetris for two players  ");
	mvwprintw(win_, 4,5, "                   PLAYER 1                                   PLAYER 2");
	mvwprintw(win_, 8,5, "         LEFT  : D                                  LEFT  : KEYBOARD_LEFT");
	mvwprintw(win_, 10,5, "         RIGHT : G                                  RIGHT : KEYBOARD_RIGHT");
	mvwprintw(win_, 12,5, "         DOWN  : F                                  DOWN  : KEYBOARD_DOWN");
	mvwprintw(win_, 16,5, "         MOVE TO FLOOR : Z                          MOVE TO FLOOR : . ");
	mvwprintw(win_, 14,5, "         ROTAION : R                                ROTATION : KEYBOARD_UP ");
	mvwprintw(win_, 20,15, "  If you read this you can press 'n' and start Tetris game good luck!!  ");

 	wrefresh(win_);
}
