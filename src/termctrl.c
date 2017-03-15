#include "termctrl.h"

static struct termios ori_term, sys_term;

void startrtsystem(void)
{
    tcgetattr(0, &ori_term); //取得原設定並儲存
    setvbuf(stdout, NULL, _IONBF, 0); //取消緩衝區
    sys_term = ori_term;
    sys_term.c_lflag &= ~ICANON; //取消標準前置處理
    sys_term.c_lflag &= ~ECHO; //取消echo
    sys_term.c_cc[VMIN] = 1; //設為1不設為0，以免在未輸入時也回傳字元
    sys_term.c_cc[VTIME] = 0; //不設timeout時間
    tcsetattr(0, TCSANOW, &sys_term);
}

void stoprtsystem(void)
{
    tcsetattr(0, TCSANOW, &ori_term);
    setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
}

void gotorc(int row, int col)
{
    printf("\e[%d;%dH", row, col);
}

void clearscr(void)
{
    printf("\e[2J");
}

void printcolor(char *str, Color color)
{
    int colornum = 8;
    int lightcode = color/colornum;
    int colorcode = color%colornum;
    if(lightcode > 1)
        printf("%s", str);
    else
        printf("\e[%d;3%dm%s\e[m", lightcode, colorcode, str);
}
