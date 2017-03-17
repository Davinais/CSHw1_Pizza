#ifndef _TERMCTRL_H_
#define _TERMCTRL_H_
/*
參考自"https://www.cyut.edu.tw/~ckhung/b/mi/textmode.php"之內容
並做些微改動，以符合需求
*/
#include <stdio.h>
#include <termios.h>

typedef enum Color
{
    BLACK, DARK_RED, DARK_GREEN, DARK_YELLOW, DARK_BLUE, DARK_MAGENTA, DARK_CYAN, GRAY,
    DARK_GRAY, RED, GREEN, YELLOW, BLUE, MAGENTA, CYAN, WHITE,
    NONE
}Color;

void startrtsystem(void);
void stoprtsystem(void);
void gotorc(int, int);
void clearscr(void);
void printcolor(char*, Color);
void drawvline(int, int, int);
void drawhline(int, int, int);
void drawbox(int , int, int, int);
#endif
