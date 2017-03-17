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

void flush_in(void)
{
    char c;
    while((c = getchar()) != '\n' && c != EOF);
}

void drawvline(int startcol, int startrow, int length)
{
    printf("\e[s");
    gotorc(startrow, startcol);
    int i = 0;
    for(i = 0; i < length; i++)
    {
        printf("│");
        printf("\e[D\e[B");
    }
    printf("\e[r");
}

void drawhline(int startrow, int startcol, int length)
{
    printf("\e[s");
    gotorc(startrow, startcol);
    int i = 0;
    for(i = 0; i < length; i++)
    {
        printf("─");
    }
    printf("\e[r");
}

void drawbox(int startrow, int startcol, int endrow, int endcol)
{
    int rowcount = endrow - startrow + 1, colcount = endcol - startcol + 1;
    if(rowcount < 2 || colcount < 2)
        return;
    printf("\e[s");
    gotorc(startrow, startcol);
    printf("┌");
    int i = 0;
    for(i = 0; i < colcount-2; i++)
        printf("─");
    printf("┐");
    for(i = 1; i <= rowcount-2; i++)
    {
        gotorc(startrow+i, endcol);
        printf("│");
    }
    gotorc(endrow, endcol);
    printf("┘");
    gotorc(startrow+1, startcol);
    for(i = 0; i < rowcount-2; i++)
    {
        printf("│");
        printf("\e[D\e[B");
    }
    printf("└");
    for(i = 0; i < colcount-2; i++)
        printf("─");
    printf("\e[r");
}
