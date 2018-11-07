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
    ~BoardPane()
    {
        wclear(win_);
        wrefresh(win_);
    }
    void make_data();
    void rotation();
	void draw();
    void mv_block(int go);
    bool can_move(int y , int x);
    bool is_touch();
    void freezing();
    void show_ghost();
    int transe_bomb();
    void transe_draw();
    int bomb(StatePane * anotherPlayer);
};
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
        {0,0,0,0,0},
        {0,1,0,0,0},
        {0,2,1,0,0},
        {0,0,1,0,0},
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
    int block_rand;
    int next_block_num; //다음 블록의 종류 번호
    int block_next_block[5][5]; // 다음 블록의 시각화 형태 자료
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
class EndPane : public Pane
{
    private:
    int win_num;
    const char *id[3] = {{"Player 2 win!!"},{"Player 1 win!!"},{"    draw!!"}};
	public:
 	EndPane(int y, int x, int h, int w,int win) : Pane(y,x,h,w)
    {win_num = win;}

    void draw();
};
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

