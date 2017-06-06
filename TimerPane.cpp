#ifndef __TET_H__
#define __TET_H__ 1
#include"tetris.h"
#endif

int TimerPane::time_return()
{
    return dead_time_sec;
}
void TimerPane::draw()
{
    gettimeofday(&end,NULL);
    dead_time_sec = TIME_LIMIT-(int)(end.tv_sec-st.tv_sec);
    dead_time_usec = abs(((int)(end.tv_usec-st.tv_usec))/10000);
    mvwprintw(win_,1,1,"      %d:%d   ",dead_time_sec,dead_time_usec);
    box(win_, 0, 0);
    wrefresh(win_);
}
