#include "termctrl.h"
#include "stringutil.h"
#include <string.h>
#include <stdbool.h>

char ask(char* question, char* options, int quesline, int errorline)
{
    bool err = false;
    char ch = '\0';
    //gotorc(quesline, 1);
    do
    {
        if(err)
        {
            gotorc(errorline, 1);
            printcolor("錯誤的輸入，請重新輸入！", RED);
        }
        gotorc(quesline, 1);
        printf("\e[2K%s", question); //\e[2K的作用為清除此行
        ch = getchar();
        if(ch == EOF || ch == '\n')
            err = true;
        else if(flush_in() > 0)
            err = true;
        else if(!strchr(options, ch))
            err = true;
        else
        {
            gotorc(errorline, 1);
            printf("\e[2K");
            err = false;
        }
    } while(err);
    return ch;
}

int main(void)
{
    int boxstartrow = 1, boxstartcol = 1, boxwidth = 80, boxheight = 22, titleheight = 2, menuwidth = 23, cmdline = 23, errorline = 24;
    int inoutselline = 4, inoutoptlength = 10;
    char *sys_name = "披　薩　點　餐　系　統";
    clearscr();
    drawbox(boxstartrow, boxstartcol, boxheight, boxwidth);
    gotorc(boxstartrow+1, (boxwidth-(int)strlen(sys_name)/3*2)/2); //由於計算字串長度真的很複雜，在已知此字串為UTF-8編碼且全中文的情況下，直接除以1.5得終端機上顯示字寬
    printf("%s", sys_name);
    drawhline(boxstartrow+titleheight, boxstartcol+1, boxwidth-2);
    drawvline(boxstartcol+menuwidth, boxstartrow+1+titleheight, boxheight-2-titleheight);
    //內用外帶選項：
    char in[15], out[15];
    strcenter(in, "1.內用", 6, inoutoptlength);
    strcenter(out, "2.外帶", 6, inoutoptlength);
    gotorc(inoutselline, boxstartcol+1);
    printf("%s%s", in, out);
    drawhline(inoutselline+1, boxstartcol+1, menuwidth-1);
    gotorc(cmdline, 1);
    char inoutch = ask("請選擇 [1]內用 或是 [2]外帶 ：", "12", cmdline, errorline);
    int inout = chartoint(inoutch);
    gotorc(inoutselline, boxstartcol+1+(inout-1)*inoutoptlength);
    printcolor((inout==1)?in:out, YELLOW);
    //口味選項：
    char taste = ask("請選擇披薩種類[1-5]：", "12345", cmdline, errorline);
    clearscr();
    gotorc(1, 1);
    return 0;
}
