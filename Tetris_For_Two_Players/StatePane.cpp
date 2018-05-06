#ifndef __TET_H__
#define __TET_H__ 1
#include"tetris.h"
#endif
void StatePane::check_time(TimerPane * timer)
{
    if(timer->end.tv_sec-transe.tv_sec >10)
        status = 0;
}
void StatePane::make_transe()
{
    status = 1;
    gettimeofday(&transe,NULL);
}
void StatePane::draw()
{
 	box(win_, 0, 0);
    wrefresh(win_);
    mvwprintw(win_,4,2,"SCORE   :  %d ",score);
    mvwprintw(win_,6,2,"STATUS  :  %s ",table[status]);
}
