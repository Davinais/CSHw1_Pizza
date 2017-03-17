#include "termctrl.h"
#include <string.h>

int main(void)
{
    int boxstartrow = 1, boxstartcol = 1, boxwidth = 80, boxheight = 22, titleheight = 2, menuwidth = 24, cmdline = 23;
    char *sys_name = "披　薩　點　餐　系　統";
    drawbox(boxstartrow, boxstartcol, boxheight, boxwidth);
    gotorc(boxstartrow+1, (boxwidth-(int)strlen(sys_name)/3*2)/2); //由於計算字串長度真的很複雜，在已知此字串為UTF-8編碼且全中文的情況下，直接除以1.5得終端機上顯示字寬
    printf("%s", sys_name);
    drawhline(boxstartrow+titleheight, boxstartcol+1, boxwidth-2);
    drawvline(boxstartcol+menuwidth, boxstartrow+1+titleheight, boxheight-2-titleheight);
    gotorc(cmdline, 1);
    char ch = '\0';
    do
    {
        printf("\e[2K請輸入測試選項(1~5)：");
        ch = getchar();
        if(ch == EOF) break;
        flush_in();
        gotorc(cmdline, 1);
    } while(strchr("12345", ch));
    clearscr();
    gotorc(1, 1);
    return 0;
}
