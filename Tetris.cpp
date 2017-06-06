#ifndef __TET_H__
#define __TET_H__ 1
#include"tetris.h"
#endif

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
    //움직였을때 밑에 있는 블록이랑 닿는다면 freezing을 시킵니다.
    if(player_1->is_touch())
    {
        make_timeterm();//바닥에 닿았을때 움직일수있는 기회를 줍니다.
        if(player_1->is_touch())
        {
            player_1->freezing();
            //블럭을 터트리는 순간 상대방 블럭을 아래로 떨어뜨립니다
            if(state_1->status ==1)
                bomb_line = player_1->transe_bomb();
            else
                 bomb_line = player_1->bomb(state_2);
            for(i=0;i<bomb_line*4;i++)
                control(KEY_DOWN);
            state_1->add_score(bomb_line*10);
            level -= bomb_line*20;
            next_1 ->cpy_next(player_1);
            next_1 ->make_block();
            if(!(player_1->can_move(0,0)))
                return 1;
        }
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
            for(i=0;i<bomb_line*4;i++)
                control('f');
            state_2->add_score(bomb_line*10);
            level -= bomb_line*20;
            next_2 ->cpy_next(player_2);
            next_2 ->make_block();
            if(!(player_2->can_move(0,0)))
                return 2;
        }
    }
    state_2->check_time(timer);
    state_1->check_time(timer);
    //시간이 다되면 포인트 높은 사람이 이깁니다.
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
            if(input == 'q')
                return 0;
            control(input);
            win = detect(player_1,player_2);
            usleep(level);//이게 0.01초임
            updateScreen();
            if(win != 0)
                break;
        }
        if(win != 0)
            break;
      player_1->mv_block(2);
      player_2->mv_block(2);
    }
    sleep(1);
    EndPane * endgame = new EndPane(0,0,35,115,win-1);
    endgame->draw();
    return win;
}
Tetris::Tetris()
{
    level =5000;

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
