#ifndef __TET_H__
#define __TET_H__ 1
#include"tetris.h"
#endif
int main()
{
    Tetris *t = new Tetris;
    t->play();
    delete t;
}
