#ifndef __TET_H__
#define __TET_H__ 1
#include"tetris.h"
#endif

void BoardPane::transe_draw()
{

 int i,j,k,l,cnt;

    for(i=5;i<HIGHT+3;i++)
    {
        for(j=1;j<WIDTH-1;j++)
        {
 		        wattron(win_,COLOR_PAIR(18));
                mvwaddch(win_,i-4,j,ACS_CKBOARD);
 		        wattroff(win_,COLOR_PAIR(18));
        }
    }
 for(i=5;i<HIGHT+3;i++)
    {
        for(j=1;j<WIDTH-1;j++)
        {
            if(board[i][j].check ==2 && board[i][j].item != 2)
            {
 		        wattron(win_,COLOR_PAIR(board[i][j].color));
                mvwaddch(win_,HIGHT-1-(i-4),j,ACS_CKBOARD);
                wattroff(win_,COLOR_PAIR(board[i][j].color));
            }
            else if(board[i][j].check ==2 && (board[i][j].item ==2 || board[i][j].item ==3))
            {

 		        wattron(win_,COLOR_PAIR(14));
                mvwaddch(win_,HIGHT-1-(i-4),j,ACS_DEGREE);
 		        wattroff(win_,COLOR_PAIR(14));
            }
            else if(board[i][j].check ==1)//그리는 함수 호출
            {
                for(k=0;k<5;k++)
                {
                    for(l=0;l<10;l++)
                    {
                        if(block_data[k][l]==1|| block_data[k][l]== 2)
                        {
 		                    wattron(win_,COLOR_PAIR(block_num+10));
                            mvwaddch(win_,HIGHT-1-(i-4+k),j-4+l,ACS_CKBOARD);
 		                    wattroff(win_,COLOR_PAIR(block_num+10));
                        }
                        else if(block_data[k][l]>=3)
                        {
 		                    wattron(win_,COLOR_PAIR(14));
                            mvwaddch(win_,HIGHT-1-(i-4+k),j-4+l,ACS_DEGREE);
 		                    wattroff(win_,COLOR_PAIR(14));
                        }
                    }
                }
            }
        }
    }
    wrefresh(win_);
}
int BoardPane::bomb(StatePane * anotherPlayer)
{
    init_pair(20, COLOR_BLUE, COLOR_BLUE);
    bool bomb_check;
    bool item_check;
    int i,j,k,q;
    int score=0;
    for(i=HIGHT+2;i>4;i--)
    {
        bomb_check = true;
        item_check = false;
        for(j=1;j<WIDTH-1;j++)
        {
            if(board[i][j].check != 2)
                bomb_check=false;
            if(board[i][j].item == 2)
                item_check = true;
        }
        if(bomb_check)
        { //한줄씩 접어버리는 식으로 진행..
            //여기에 item 체크후 function이 나와야할것 같음.
            for(k=1;k<WIDTH-1;k++)
            {

     		        wattron(win_,COLOR_PAIR(20));
                    mvwaddch(win_,i-4-score,k,ACS_CKBOARD);
 		            wattroff(win_,COLOR_PAIR(20));
                    usleep(10000);
                    wrefresh(win_);
            }
            usleep(10000);
            for(q=i-1;q>0;q--)
            {
                for(k=1;k<WIDTH-1;k++)
                {
                    board[q+1][k] = board[q][k];
                }
            }
            score++;
            i++;
        }
        if(item_check&&bomb_check)
            anotherPlayer->make_transe();
    }
    return score;
}

int BoardPane::transe_bomb()
{
    init_pair(20, COLOR_BLUE, COLOR_BLUE);
    bool bomb_check;
    int i,j,k,q;
    int score=0;
    for(i=HIGHT+2;i>4;i--)
    {
        bomb_check = true;
        for(j=1;j<WIDTH-1;j++)
        {
            if(board[i][j].check != 2)
                bomb_check=false;
        }
        if(bomb_check)
        { //한줄씩 접어버리는 식으로 진행..
            //여기에 item 체크후 function이 나와야할것 같음.
            for(k=1;k<WIDTH-1;k++)
            {

     		        wattron(win_,COLOR_PAIR(20));
                    mvwaddch(win_,HIGHT-1-(i-4-score),k,ACS_CKBOARD);
 		            wattroff(win_,COLOR_PAIR(20));
                    usleep(10000);
                    wrefresh(win_);
            }
            usleep(10000);
            for(q=i-1;q>0;q--)
            {
                for(k=1;k<WIDTH-1;k++)
                {
                    board[q+1][k] = board[q][k];
                }
            }
            score++;
            i++;
        }
    }
    return score;
}
void BoardPane::show_ghost()
{
    int i,j,k,l;
    int cnt =0;
    while(can_move(cnt,0))
    {cnt++;}
    if(cnt>2)
    {
        for(i=5;i<HIGHT+3;i++)
        {
            for(j=1;j<WIDTH-1;j++)
            {
                if(board[i][j].check ==1)
                {
                    for(k=0;k<5;k++)
                    {
                        for(l=0;l<10;l++)
                        {
                            if(block_data[k][l]!=0)
                            {
                                wattron(win_,COLOR_PAIR(19));
                                mvwaddch(win_,i-4+k+cnt-1,j-4+l,ACS_CKBOARD);
                                wattroff(win_,COLOR_PAIR(19));
                            }
                        }
                    }
                }
            }
        }
    }
}
void BoardPane::freezing()
{
    int i,j,k,l;
    for(i=0;i<HIGHT+4;i++)
        {
            for(j=0;j<WIDTH;j++)
            {
                if(board[i][j].check ==1)
                {
                    board[i][j].check =0;
                    for(k=0;k<5;k++)
                        for(l=0;l<10;l++)
                        {
                            if(block_data[k][l]==1 || block_data[k][l] == 2)
                            {
                                board[i+k][j+l-4].check=2;
                                board[i+k][j+l-4].color=block_num+10;
                            }
                            if(block_data[k][l]>=3)
                            {
                                board[i+k][j+l-4].check=2;
                                board[i+k][j+l-4].item=2;
                                board[i+k][j+l-4].color=14;
                            }
                        }
                }
            }
        }
}
bool BoardPane::is_touch()
{
    if(can_move(1,0))
        return false;
    else
        return true;
}
bool BoardPane::can_move(int y, int x)
{
    int i,j,k,l;
    int temp1[HIGHT+4][WIDTH];
    int temp2[HIGHT+4][WIDTH];
    for(i=0;i<HIGHT+4;i++)
        for(j=0;j<WIDTH;j++)
        {
            temp1[i][j] =0;
            temp2[i][j] =0;
        }
    for(i=0;i<HIGHT+4;i++) //temp라는 두개의 배열에 현재 고정된 블럭과 움직였을시에 생길 블럭의 위치를 저장합니다.
        for(j=0;j<WIDTH;j++)
        {
            if(board[i][j].check ==2)
                temp1[i][j] = 1;

            if(board[i][j].check ==1) //check가 1인곳을 찾아서 그 부분에 블록을 생성해주며 동시에 temp2 배열에 값을 채워서 temp1과 충돌이 일어나는지 확인
            {
                for(k=0;k<5;k++)
                    for(l=0;l<10;l++)
                        if(block_data[k][l]!=0)
                           temp2[i+y+k][j+x-4+l]=1;
            }
        }
    for(i=0;i<HIGHT+4;i++)
        for(j=0;j<WIDTH;j++)
        {
            if(temp1[i][j] ==1 && temp2[i][j] ==1)
                return false;
            if(temp2[i][j] ==1 && (i<4 || i>HIGHT+2 || j<1|| j>WIDTH-2))
                return false;
        }
    return true;
}
void BoardPane::mv_block(int go)
{
    int i,j,k,l,cnt;
    int y[4] = {0,0,1,0};
    int x[4] = {2,-2,0,0};
    cnt =0;
    if(go ==3)
    {
        while(can_move(cnt,0))
        {cnt++;}

        y[3] = cnt-1;
    }
    if(can_move(y[go],x[go]))
    {
        for(i=0;i<HIGHT+4;i++)
            for(j=0;j<WIDTH;j++)
                if(board[i][j].check ==1)
                {
                    board[i][j].check =0;
                    board[i+y[go]][j+x[go]].check = 1;
                    return;
                }
    }
}
void BoardPane::make_data()
{
    int i,j;
    for(i=0;i<5;i++)
        for(j=0;j<10;j++)
        block_data[i][j] =0;
    for(i=0;i<5;i++)
    {
        for(j=0;j<5;j++)
        {
            if(block[i][j] == 1)
            {
                block_data[i][j*2] =1;
                block_data[i][j*2+1] =1;
            }
            else if(block[i][j] >1)
            {
                block_data[i][j*2] =block[i][j];
                block_data[i][j*2+1] =block[i][j];
            }
            else
            {
                block_data[i][j*2] =0;
                block_data[i][j*2+1] =0;
            }
        }
    }
}
void BoardPane::rotation()
{
    int i,j;
    int temp[5][5];
    int remember[5][5];
    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            remember[i][j] = block[i][j];

    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            temp[i][j] = block[4-j][i];

    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            block[i][j] = temp[i][j];
    make_data();
    //rotation시 충돌이 일어나는지 확인하고 일어난다면 remember 를 이용해서 원래 블록으로 돌아옵니다.
    if(can_move(0,0))
        return;
    else
    {
        for(i=0;i<5;i++)
            for(j=0;j<5;j++)
                block[i][j] = remember[i][j];
        make_data();
    }
}
void BoardPane::draw()
{
 	mvwvline(win_, 1, 0, ACS_DEGREE, height_ );//left
 	mvwvline(win_, 1, width_-1, ACS_DEGREE, height_ );//right
 	mvwhline(win_, 0, 0, ACS_DEGREE, width_);
 	mvwhline(win_, height_-1, 1,ACS_DEGREE, width_);

 	init_pair(10, COLOR_WHITE, COLOR_YELLOW);
 	init_pair(11, COLOR_WHITE, COLOR_BLUE);
 	init_pair(12, COLOR_WHITE, COLOR_GREEN);
 	init_pair(13, COLOR_WHITE, COLOR_MAGENTA);
 	init_pair(14, COLOR_WHITE, COLOR_RED);
 	init_pair(15, COLOR_WHITE, COLOR_CYAN);
 	init_pair(16, COLOR_WHITE, COLOR_WHITE);
 	init_pair(18, COLOR_BLACK, COLOR_BLACK);
 	init_pair(17, COLOR_WHITE, COLOR_WHITE);
 	init_pair(19, COLOR_WHITE, COLOR_BLACK);
    int i,j,k,l,cnt;
    for(i=5;i<HIGHT+3;i++)
    {
        for(j=1;j<WIDTH-1;j++)
        {
 		        wattron(win_,COLOR_PAIR(18));
                mvwaddch(win_,i-4,j,ACS_CKBOARD);
 		        wattroff(win_,COLOR_PAIR(18));
        }
    }
    for(i=5;i<HIGHT+3;i++)
    {
        for(j=1;j<WIDTH-1;j++)
        {
            if(board[i][j].check ==2 && board[i][j].item != 2)
            {
 		        wattron(win_,COLOR_PAIR(board[i][j].color));
                mvwaddch(win_,i-4,j,ACS_CKBOARD);
                wattroff(win_,COLOR_PAIR(board[i][j].color));
            }
            else if(board[i][j].check ==2 && (board[i][j].item ==2 || board[i][j].item ==3))
            {

 		        wattron(win_,COLOR_PAIR(14));
                mvwaddch(win_,i-4,j,ACS_DEGREE);
 		        wattroff(win_,COLOR_PAIR(14));
            }
            else if(board[i][j].check ==1)//그리는 함수 호출
            {
                for(k=0;k<5;k++)
                {
                    for(l=0;l<10;l++)
                    {
                        if(block_data[k][l]==1|| block_data[k][l]== 2)
                        {
 		                    wattron(win_,COLOR_PAIR(block_num+10));
                            mvwaddch(win_,i-4+k,j-4+l,ACS_CKBOARD);
 		                    wattroff(win_,COLOR_PAIR(block_num+10));
                        }
                        else if(block_data[k][l]>=3)
                        {
 		                    wattron(win_,COLOR_PAIR(14));
                            mvwaddch(win_,i-4+k,j-4+l,ACS_DEGREE);
 		                    wattroff(win_,COLOR_PAIR(14));
                        }
                    }
                }
            }
        }
    }

   show_ghost();
    //ghost
    wrefresh(win_);
}
