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
        attron(COLOR_PAIR(1)); // 파레트 적용
        border('*', '*', '*', '*', '*', '*', '*', '*');
        attroff(COLOR_PAIR(1)); // 파레트 해제
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

class InfoPane : public Pane
{
	public:
 	InfoPane(int y, int x, int h, int w) : Pane(y,x,h,w)
    {}

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
    private: //2라는 숫자는 아이템을 나타나게 하기 위함
    int block_num; //현재블록의 종류 번호
    int block[5][5]; //현재 블록의 시각화 형태 자료
    int block_data[5][10]; //블록이 보드에 적혀질때의 자료, 즉,ncurses 화면을 고려한 배열
    bool check;
    board_data board[HIGHT+4][WIDTH]; //보드판
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
    void make_new_block();
    void make_data();
    void rotation();
	void draw();
    void cpy_next();
    void mv_board(int go);
    bool can_move(int y , int x);
   // bool is_death();
   // bool is_touch();
   // void freezing();
   // void check_item();
   // int bomb();
};

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

            if(board[i][j].check ==1)
            {
                for(k=0;k<5;k++)
                    for(l=0;l<10;l++)
                        if(block_data[k][l]!=0)
                           temp2[i+y+k][j+x-4+l]=1;
                           //mvwaddch(win_,i-4+k,j-4+l,ACS_CKBOARD);
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
void BoardPane::mv_board(int go)
{
    int i,j;
    int y[4] = {0,0,1,0};
    int x[4] = {2,-2,0,0};
    if(go ==3)
    {
        while(can_move(1,0))
        {
            for(i=0;i<HIGHT+4;i++)
            {
                for(j=0;j<WIDTH;j++)
                {
                    if(board[i][j].check ==1)
                    {
                        board[i][j].check =0;
                        board[i+1][j].check = 1;
                        break;
                    }
                }
                if(j!=WIDTH)
                    break;
            }
        }
    }
    else if(can_move(y[go],x[go]))
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
            if(block[i][j] != 0)
            {
                block_data[i][j*2] =1;
                block_data[i][j*2+1] =1;
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
    if(can_move(0,0))
        return;
    else
    {
        for(i=0;i<5;i++)
            for(j=0;j<5;j++)
                block[i][j] = remember[i][j];
        make_data();
    }
    //if can return else change remember
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
    int i,j,k,l;
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
            if(board[i][j].check ==2)
            {
 		        wattron(win_,COLOR_PAIR(board[i][j].color));
                mvwaddch(win_,i-4,j,ACS_CKBOARD);
                wattroff(win_,COLOR_PAIR(board[i][j].color));
            }
            else if(board[i][j].check ==1)//그리는 함수 호출
            {
                for(k=0;k<5;k++)
                {
                    for(l=0;l<10;l++)
                    {
                        if(block_data[k][l]!=0)
                        {
 		                    wattron(win_,COLOR_PAIR(block_num+10));
                            mvwaddch(win_,i-4+k,j-4+l,ACS_CKBOARD);
 		                    wattroff(win_,COLOR_PAIR(block_num+10));
                        }
                    }
                }
            }
        }
    }
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
        {0,0,1,1,0},
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
        {0,0,0,0,0},
        {0,0,1,0,0},
        {0,0,2,1,0},
        {0,0,0,1,0},
        {0,0,0,0,0}
    },
    {
        {0,0,0,0,0},
        {0,0,0,1,0},
        {0,0,1,1,0},
        {0,0,2,0,0},
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
    int next_block_num; //다음 블록의 종류 번호
    int block_next_block[5][5]; // 다음 블록의 시각화 형태 자료
	public:
    void cpy_next(BoardPane * board);
    void make_block();
 	NextPane(int y, int x, int h, int w) : Pane(y,x,h,w){}
 	void draw();
};
void NextPane::make_block()
{
   srand(time(NULL));
   next_block_num = rand()%7;
   int i,j;
    for(i=0;i<5;i++)
        for(j=0;j<5;j++)
            block_next_block[i][j] = block_info[next_block_num][i][j];
    if(rand()%8 ==0) //1/8 확률로 아이템 생성
    {
        for(i=0;i<5;i++)
            for(j=0;j<5;j++)
                if(block_next_block[i][j] ==2)
                    block_next_block[i][j] = rand()%3+2;//item 이 있다면 2~4까지 숫자를 가질것.
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
            if(block_next_block[i][j] !=0)
            {
 		        wattron(win_,COLOR_PAIR(next_block_num+10));
                mvwaddch(win_,i+2,6+j*2,ACS_CKBOARD);
                mvwaddch(win_,i+2,6+j*2+1,ACS_CKBOARD);
 		        wattroff(win_,COLOR_PAIR(next_block_num+10));
            }
        }
    wrefresh(win_);
}

class StatePane : public Pane
{
	public:
 	StatePane(int y, int x, int h, int w) : Pane(y,x,h,w){}
 	void draw();
};
void StatePane::draw()
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
 	void draw();
};
void TimerPane::draw()
{
    gettimeofday(&end,NULL);
    dead_time_sec = TIME_LIMIT-(int)(end.tv_sec-st.tv_sec);
    dead_time_usec = abs(((int)(end.tv_usec-st.tv_usec))/10000);
    mvwprintw(win_,1,1,"      %d:%d   ",dead_time_sec,dead_time_usec);
    box(win_, 0, 0);
    wrefresh(win_);
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
	~Tetris();
 	void play();
	void updateScreen();
    void show_info();
};

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
    player_1->draw();
    player_2->draw();
    next_1->draw();
    next_2->draw();
    state_1->draw();
    state_2->draw();
    timer ->draw();
}
void Tetris::play()
{
    int input,i;
    next_1->make_block();
    next_1->cpy_next(player_1);
    next_1->make_block();
    next_2->make_block();
    next_2->cpy_next(player_2);
    next_2->make_block();
    while(1)
    {
        for(i=0;i<100;i++)
        {
            input = getch();
            switch(input)
            {
                case 'f':
                player_1->mv_board(2);
                break;

                case 'd':
                player_1->mv_board(1);
                break;

                case 'g':
                player_1->mv_board(0);
                break;

                case 'z':
                player_1->mv_board(3);
                break;

                case 'r':
                player_1->rotation();
                break;

                case KEY_DOWN:
                player_2->mv_board(2);
                break;

                case KEY_LEFT:
                player_2->mv_board(1);
                break;

                case KEY_RIGHT:
                player_2->mv_board(0);
                break;

                case KEY_UP:
                player_2->rotation();

                case '.':
                player_2->mv_board(3);
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
            /* check 배열을 보고싶으면 주석을 해제하세요
            for(int j=0;j<HIGHT+4;j++)
            {
                for(int l=0;l<WIDTH+2;l++)
                {
                    mvprintw(0+j,26+l,"%d ",player_1->board[j][l].check);
                }
            }*/
            usleep(level);//이게 0.01초임
            timer->draw();
            updateScreen();
        }

      player_1->mv_board(2);
      player_2->mv_board(2);
//    next_2->cpy_next(player_2);
//    next_2->make_block();
//    next_1->cpy_next(player_1);
//    next_1->make_block();
    }

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
    next_1 = new NextPane(8,33,7,WIDTH);
    next_2 = new NextPane(8,58,7,WIDTH);
    state_1 = new StatePane(17,33,16,WIDTH);
    state_2 = new StatePane(17,58,16,WIDTH);
    timer = new TimerPane(3,47,3,19);

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
    refresh();
}
Tetris::~Tetris()
{
 	endwin();
}

int main()
{
    Tetris t;
    t.play();
}
