#ifndef __TET_H__
#define __TET_H__ 1
#include"tetris.h"
#endif

void NextPane::make_block()
{
   srand(time(NULL));
   block_rand++;
   //o-block 이랑 막대 블록 생성확률을 더 높여서 난이도를 낮췄습니다.
   if((rand()+block_rand)%5 ==0)
        next_block_num = 6;
    else if((rand()+block_rand)%5 ==1)
        next_block_num = 0;
    else
        next_block_num = (rand()+block_rand)%7;
   int i,j;
    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            block_next_block[i][j] = block_info[next_block_num][i][j];
    if((rand()+block_rand)%11 ==0) //1/11 확률로 아이템 생성
    {
        for(i=0;i<5;i++)
            for(j=0;j<5;j++)
                if(block_next_block[i][j] ==2)
                    block_next_block[i][j] = rand()%2+3;//item 이 있다면 2~3까지 숫자를 가질것.
    }
}
void NextPane::cpy_next(BoardPane * boardpane)
{
    int i,j;

    boardpane->block_num = next_block_num;

    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            boardpane->block[i][j] = 0;
    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            boardpane->block[i][j] = block_next_block[i][j];
    boardpane->make_data();
    boardpane->board[5][9].check = 1;
    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            block_next_block[i][j] =0;

}
void NextPane::draw()
{
    int i,j;
 	box(win_, 0, 0);
 	init_pair(10, COLOR_WHITE, COLOR_YELLOW);
 	init_pair(11, COLOR_WHITE, COLOR_BLUE);
 	init_pair(12, COLOR_WHITE, COLOR_GREEN);
 	init_pair(13, COLOR_WHITE, COLOR_MAGENTA);
 	init_pair(14, COLOR_WHITE, COLOR_RED);
 	init_pair(15, COLOR_WHITE, COLOR_CYAN);
 	init_pair(16, COLOR_WHITE, COLOR_WHITE);
 	init_pair(18, COLOR_BLACK, COLOR_BLACK);
    for(i=1;i<6;i++)
    {
        for(j=1;j<21;j++)
            {
 		        wattron(win_,COLOR_PAIR(18));
                mvwaddch(win_,i,j,ACS_CKBOARD);
 		        wattroff(win_,COLOR_PAIR(18));
            }
    }
    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
        {
            if(block_next_block[i][j] ==1 || block_next_block[i][j] ==2 )
            {
 		        wattron(win_,COLOR_PAIR(next_block_num+10));
                mvwaddch(win_,i+2,6+j*2,ACS_CKBOARD);
                mvwaddch(win_,i+2,6+j*2+1,ACS_CKBOARD);
 		        wattroff(win_,COLOR_PAIR(next_block_num+10));
            }

            else if(block_next_block[i][j]>=3)
            {
                wattron(win_,COLOR_PAIR(14));
                mvwaddch(win_,i+2,6+j*2,ACS_BULLET);
                mvwaddch(win_,i+2,6+j*2+1,ACS_DEGREE);
                wattroff(win_,COLOR_PAIR(14));
            }
        }
    wrefresh(win_);
}
