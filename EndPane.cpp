#ifndef __TET_H__
#define __TET_H__ 1
#include"tetris.h"
#endif

void EndPane::draw()
{
    clear();
    refresh();
    attron(COLOR_PAIR(3));
    border('*', '*', '*', '*', '*', '*', '*', '*');
	mvprintw(3,50, "  congratuation!  ");
	mvprintw(6,50, "  %s  ",id[win_num]);
	mvprintw(10,45, "  You can press 'q' to exit  ",id[win_num]);
    attroff(COLOR_PAIR(3));
 	refresh();
    int input =0;
    while(1)
    {
        input = getch();
        if(input == 'q')
            return;
    }
}
