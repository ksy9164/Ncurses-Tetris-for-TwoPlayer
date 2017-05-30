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
    public:
    int level;
    friend class NextPane;
    bool check;
	public:
 	BoardPane(int y, int x, int h, int w) : Pane(y,x,h,w){}
    /*
 	BoardPane(int x, int y, int w, int h) : Pane(x,y,w,h)
    {
        int i,j;
        level = 10;
        for(i=0;i<HIGHT+5;i++)
            for(j=0;j<WIDTH;j++)
            {
                present[i][j].check =0;present[i][j].color =0;present[i][j].item =0;
                next[i][j].check =0;next[i][j].color =0;next[i][j].item =0;
            }
    }
    */
	void draw();
   // bool is_death();
   // bool is_touch();
   // void freezing();
   // int bomb();
};

void BoardPane::draw()
{
 	mvwvline(win_, 1, 0, ACS_DEGREE, height_ );//left
 	mvwvline(win_, 1, width_-1, ACS_DEGREE, height_ );//right
 	mvwhline(win_, 0, 0, ACS_DEGREE, width_);
 	mvwhline(win_, height_-1, 1,ACS_DEGREE, width_);

    wrefresh(win_);
}

class NextPane : public Pane
{
    friend class BoardPane;
	public:
    //void cpy_block(BoardPane * boardpane);
    //void make_block();
 	NextPane(int y, int x, int h, int w) : Pane(y,x,h,w){}
 	void draw();
};
void NextPane::draw()
{
 	box(win_, 0, 0);
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
Tetris::Tetris()
{
	initscr();
    cbreak();
    noecho();
    nodelay(stdscr, TRUE);
    keypad(stdscr, TRUE);

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
    while(1)
    {
        //updateScreen();
        timer->draw();
        usleep(100);
    }

}
Tetris::~Tetris()
{
 	endwin();
}

int main()
{
    Tetris t;
}

