#include<iostream>
#include<ncurses.h>
#include<ctime>
#include<cstdlib>
#include<fstream>
#include<cstdio>
#include<sys/time.h>
#include<unistd.h>
#include<math.h>

#define WIDTH 22
#define HIGHT 24
#define TIME_LIMIT 180
using namespace std;
typedef struct board_data
{
    int block_num;
    int rotate_num;
    int color;
    int check;
    int item;
}board_data;
class Pane
{
	protected:
 	int width_, height_;
 	int x_, y_;
 	WINDOW *win_;

	public:
 	Pane(int y, int x, int h, int w);

 	~Pane()
	{
        clear();
 		delwin(win_);
        attron(COLOR_PAIR(1)); // �ķ�Ʈ ����
        border('*', '*', '*', '*', '*', '*', '*', '*');
        attroff(COLOR_PAIR(1)); // �ķ�Ʈ ����
        refresh();
	}

	virtual void draw();
};

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

class TimerPane : public Pane
{
	public:
    struct timeval st,end;
    int dead_time_sec;
    int dead_time_usec;
 	TimerPane(int y, int x, int h, int w) : Pane(y,x,h,w)
    {
        gettimeofday(&st,NULL);
    }
    ~TimerPane()
    {
        wclear(win_);
        wrefresh(win_);
    }
 	void draw();
    int time_return();
};
class StatePane : public Pane
{
    public:
    int status;
    private:
    int score;
    struct timeval transe;
    const char *table[2]={{"normal"},{"transe"}};
	public:
 	StatePane(int y, int x, int h, int w) : Pane(y,x,h,w){score=0;status=0;}
    ~StatePane()
    {

        wclear(win_);
        wrefresh(win_);
    }
    void add_score(int s){score += s;}
    int return_score(void){return score;}
 	void draw();
    void make_transe();
    void check_time(TimerPane * timer);
};
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
class InfoPane : public Pane
{
	public:
 	InfoPane(int y, int x, int h, int w) : Pane(y,x,h,w)
    {}
    ~InfoPane()
    {
        wclear(win_);
        wrefresh(win_);
    }

    void draw();
};

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
class BoardPane : public Pane
{
    friend class NextPane;
    friend class Tetris;
    private: //2��� ���ڴ� �������� ��Ÿ���� �ϱ� ����
    int block_num; //�������� ���� ��ȣ
    int block[5][5]; //���� ����� �ð�ȭ ���� �ڷ�
    int block_data[5][10]; //����� ���忡 ���������� �ڷ�, ��,ncurses ȭ���� ����� �迭
    bool check;
    board_data board[HIGHT+4][WIDTH]; //������
	public:
 	BoardPane(int y, int x, int h, int w) : Pane(y,x,h,w)
    {
        int i,j;
        for(i=0;i<HIGHT+4;i++)
            for(j=0;j<WIDTH;j++)
            {
                board[i][j].block_num=0;
                board[i][j].rotate_num=0;
                board[i][j].color=0;
                board[i][j].check=0;
                board[i][j].block_num=0;
            }
    }
    ~BoardPane()
    {
        wclear(win_);
        wrefresh(win_);
    }
    void make_new_block();
    void make_data();
    void rotation();
	void draw();
    void cpy_next();
    void mv_block(int go);
    bool can_move(int y , int x);
   // bool is_death();
    bool is_touch();
    void freezing();
    void show_ghost();
    int transe_bomb();
    void transe_draw();
   // void check_item();
    int bomb(StatePane * anotherPlayer);
};
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
            else if(board[i][j].check ==1)//�׸��� �Լ� ȣ��
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
        { //���پ� ��������� ������ ����..
            //���⿡ item üũ�� function�� ���;��Ұ� ����.
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
        { //���پ� ��������� ������ ����..
            //���⿡ item üũ�� function�� ���;��Ұ� ����.
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
    for(i=0;i<HIGHT+4;i++) //temp��� �ΰ��� �迭�� ���� ������ ���� ���������ÿ� ���� ���� ��ġ�� �����մϴ�.
        for(j=0;j<WIDTH;j++)
        {
            if(board[i][j].check ==2)
                temp1[i][j] = 1;

            if(board[i][j].check ==1) //check�� 1�ΰ��� ã�Ƽ� �� �κп� ����� �������ָ� ���ÿ� temp2 �迭�� ���� ä���� temp1�� �浹�� �Ͼ���� Ȯ��
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
    //rotation�� �浹�� �Ͼ���� Ȯ���ϰ� �Ͼ�ٸ� remember �� �̿��ؼ� ���� ������� ���ƿɴϴ�.
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
            else if(board[i][j].check ==1)//�׸��� �Լ� ȣ��
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

class NextPane : public Pane
{
    friend class BoardPane;
    private:
    int block_info[7][5][5]={
    {
        {0,0,0,0,0},
        {0,0,1,1,0},
        {0,0,2,1,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    },
    {
        {0,0,2,0,0},
        {0,0,1,0,0},
        {0,0,1,1,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    },
    {
        {0,0,1,0,0},
        {0,0,2,0,0},
        {0,1,1,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    },
    {
        {0,0,1,0,0},
        {0,0,2,1,0},
        {0,0,0,1,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    },
    {
        {0,0,0,1,0},
        {0,0,1,1,0},
        {0,0,2,0,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    },
    {
        {0,0,0,0,0},
        {0,0,2,0,0},
        {0,1,1,1,0},
        {0,0,0,0,0},
        {0,0,0,0,0}
    },
    {
        {0,0,1,0,0},
        {0,0,1,0,0},
        {0,0,2,0,0},
        {0,0,1,0,0},
        {0,0,0,0,0}
    },
    };
    int block_rand;
    int next_block_num; //���� ����� ���� ��ȣ
    int block_next_block[5][5]; // ���� ����� �ð�ȭ ���� �ڷ�
	public:
 	NextPane(int y, int x, int h, int w, int ran) : Pane(y,x,h,w){block_rand=ran;}
    ~NextPane()
    {
        wclear(win_);
        wrefresh(win_);
    }
    void cpy_next(BoardPane * board);
    void make_block();
 	void draw();
};
void NextPane::make_block()
{
   srand(time(NULL));
   block_rand++;
   //o-block �̶� ���� ��� ����Ȯ���� �� ������ ���̵��� ������ϴ�.
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
    if(rand()%13 ==0) //1/13 Ȯ���� ������ ����
    {
        for(i=0;i<5;i++)
            for(j=0;j<5;j++)
                if(block_next_block[i][j] ==2)
                    block_next_block[i][j] = rand()%2+3;//item �� �ִٸ� 2~3���� ���ڸ� ������.
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
class EndPane : public Pane
{
    private:
    int win_num;
    const char *id[2] = {{"Player 2 win!!"},{"Player 1 win!!"}};
	public:
 	EndPane(int y, int x, int h, int w,int win) : Pane(y,x,h,w)
    {win_num = win;}

    void draw();
};

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
class Tetris
{
    int level;
    InfoPane *infoPane_;
    BoardPane *player_1;
    BoardPane *player_2;
    NextPane *next_1;
    NextPane *next_2;
    StatePane *state_1;
    StatePane *state_2;
    TimerPane *timer;
    public:
	Tetris();
	~Tetris( );
 	int play();
    void control(int input);
	void updateScreen();
    void show_info();
    int detect(BoardPane * player1 , BoardPane * player2);
    void make_timeterm();
    void base_screen_show();
};
void Tetris::base_screen_show()
{
    attron(COLOR_PAIR(1));
    border('*', '*', '*', '*', '*', '*', '*', '*');
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(1));
    mvprintw(3,11,"PLYAER 1");
    mvprintw(3,95,"PLYAER 2");
    attroff(COLOR_PAIR(1));

    attron(COLOR_PAIR(3));
    mvprintw(7,39,"NEXT BLOCK");
    mvprintw(7,64,"NEXT BLOCK");
    attroff(COLOR_PAIR(3));

    mvprintw(16,42,"STATE");
    mvprintw(16,67,"STATE");

    mvprintw(2,55,"TIMER");
}
void Tetris::make_timeterm()
{
    int i,input;
    for(i=0;i<10;i++)
    {
        input=getch();
        control(input);
        usleep(level);
    }
}
int Tetris::detect(BoardPane * player1 , BoardPane * player2)
{
    int bomb_line,i;
    //���������� �ؿ� �ִ� ����̶� ��´ٸ� freezing�� ��ŵ�ϴ�.
    if(player_1->is_touch())
    {
        make_timeterm();//�ٴڿ� ������� �����ϼ��ִ� ��ȸ�� �ݴϴ�.
        if(player_1->is_touch())
        {
            player_1->freezing();
            //���� ��Ʈ���� ���� ���� ���� �Ʒ��� ����߸��ϴ�
            if(state_1->status ==1)
                bomb_line = player_1->transe_bomb();
            else
                 bomb_line = player_1->bomb(state_2);
            for(i=0;i<bomb_line*2;i++)
                control(KEY_DOWN);
            state_1->add_score(bomb_line*10);
            next_1 ->cpy_next(player_1);
            next_1 ->make_block();
            if(!(player_1->can_move(0,0)))
                return 1;


            //�׾����� �ߴ� �Լ��� ȣ��! if(!(player_1->can_move(0,0)))
         }   //���� �������ڸ��� ��ģ�ٸ� �״°���.
    }
    if(player_2->is_touch())
    {
        make_timeterm();
        if(player_2->is_touch())
        {
            player_2->freezing();
            if(state_2->status ==1)
                bomb_line = player_2->transe_bomb();
            else
                 bomb_line = player_2->bomb(state_1);
            bomb_line = player_2->bomb(state_1);
            for(i=0;i<bomb_line*2;i++)
                control('f');
            state_2->add_score(bomb_line*10);
            next_2 ->cpy_next(player_2);
            next_2 ->make_block();
            if(!(player_2->can_move(0,0)))
                return 2;
            //if(!(player_2->can_move(0,0)))// �׾����� �ߴ� �Լ��� ȣ��!
        }
    }
    state_2->check_time(timer);
    state_1->check_time(timer);
    //�ð��� �ٵǸ� ����Ʈ ���� ����� �̱�ϴ�.
    if(timer->time_return() < 0)
    {
        if((state_1->return_score()) > (state_2->return_score()))
            return 2;
        else
            return 1;
    }
    return 0;
}

void Tetris::control(int input)
{
    switch(input)
    {
        case 'f':
        player_1->mv_block(2);
        break;

        case 'd':
        player_1->mv_block(1);
        break;

        case 'g':
        player_1->mv_block(0);
        break;

        case 'z':
        player_1->mv_block(3);
        break;

        case 'r':
        player_1->rotation();
        break;

        case KEY_DOWN:
        player_2->mv_block(2);
        break;

        case KEY_LEFT:
        player_2->mv_block(1);
        break;

        case KEY_RIGHT:
        player_2->mv_block(0);
        break;

        case KEY_UP:
        player_2->rotation();
        break;

        case '.':
        player_2->mv_block(3);
        break;

        case 'q':
        return;

        case 'p':
        input =0;
        while(input != 'p')
        {
            input =getch();
        }

    }
}
void Tetris::show_info()
{
 	init_pair(2, COLOR_BLACK, COLOR_WHITE);
 	attron(COLOR_PAIR(2));
    infoPane_->draw();
 	attroff(COLOR_PAIR(2));
    int input =0;
    while(input != 'n')
    {
        input=getch();
    }
}

void Tetris::updateScreen()
{
    if(state_1->status ==1)
        player_1->transe_draw();
    else
        player_1->draw();
    if(state_2->status ==1)
        player_2->transe_draw();
    else
        player_2->draw();
    next_1->draw();
    next_2->draw();
    state_1->draw();
    state_2->draw();
    timer ->draw();
}
int Tetris::play()
{
    int input,i,win;
    while(1)
    {
        for(i=0;i<100;i++)
        {
            input = getch();
            control(input);
            win = detect(player_1,player_2);
            usleep(level);//�̰� 0.01����
            updateScreen();
            if(win != 0)
                break;
        }
//         check �迭�� ��������� �ּ��� �����ϼ���
//        for(int j=0;j<HIGHT+4;j++)
//        {
//            for(int l=0;l<WIDTH+2;l++)
//            {
//                mvprintw(0+j,26+l,"%d ",player_1->board[j][l].check);
//            }
//        }
        if(win != 0)
            break;
      player_1->mv_block(2);
      player_2->mv_block(2);
    }
    EndPane * endgame = new EndPane(0,0,35,115,win-1);
    endgame->draw();
    return win;
}
Tetris::Tetris()
{
    level =10000;

	initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);
    curs_set(0);

    start_color();
    resize_term(35,115);

    init_pair(1,COLOR_BLUE,COLOR_BLACK);
    init_pair(3,COLOR_YELLOW,COLOR_BLACK);
    attron(COLOR_PAIR(1));
    border('*', '*', '*', '*', '*', '*', '*', '*');
    attroff(COLOR_PAIR(1));
	refresh();

 	infoPane_ = new InfoPane(5,10,25,95);
    show_info();
    delete infoPane_;

    player_1 = new BoardPane(5,4,HIGHT,WIDTH);
    player_2 = new BoardPane(5,114-(4+22),HIGHT,WIDTH);
    next_1 = new NextPane(8,33,7,WIDTH,1);
    next_2 = new NextPane(8,58,7,WIDTH,4);
    state_1 = new StatePane(17,33,16,WIDTH);
    state_2 = new StatePane(17,58,16,WIDTH);
    timer = new TimerPane(3,47,3,19);

    base_screen_show();

    next_1->make_block();
    next_1->cpy_next(player_1);
    next_1->make_block();
    next_2->make_block();
    next_2->cpy_next(player_2);
    next_2->make_block();

    refresh();
}
Tetris::~Tetris()
{
   delete player_1;
   delete next_1;
   delete state_1;
   delete player_2;
   delete next_2;
   delete state_2;
   delete timer;
   endwin();
}

int main()
{
    Tetris *t = new Tetris;
    t->play();
    delete t;
}
