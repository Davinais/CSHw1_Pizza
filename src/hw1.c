#include "termctrl.h"
#include "stringutil.h"
#include <string.h>
#include <stdbool.h>
#define TOTAL_PIZZA_TASTE 5 //const int 放在array裡面還是不給我過，只好用define下策

typedef struct pizza
{
    char name[20];
    int price;
    bool vegetarian;
}Pizza;

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
    int boxstartrow = 1, boxstartcol = 1, boxwidth = 80, boxheight = 22, titleheight = 2, menuwidth = 29, cmdline = 23, errorline = 24,
        indent = 5, inoutselline = 4, inoutoptlength = (menuwidth-1)/2, pizzatasteselline = inoutselline+3,
        ingreselline = pizzatasteselline+TOTAL_PIZZA_TASTE+2;
    int i = 0;
    char *sys_name = "披　薩　點　餐　系　統";
    clearscr();
    drawbox(boxstartrow, boxstartcol, boxheight, boxwidth);
    gotorc(boxstartrow+1, (boxwidth-(int)strlen(sys_name)/3*2)/2); //由於計算字串長度真的很複雜，在已知此字串為UTF-8編碼且全中文的情況下，直接除以1.5得終端機上顯示字寬
    printf("%s", sys_name);
    drawhline(boxstartrow+titleheight, boxstartcol+1, boxwidth-2); //標題下方橫線
    drawvline(boxstartcol+menuwidth, boxstartrow+1+titleheight, boxheight-2-titleheight); //分隔左右側的直線
    drawhline(inoutselline+1, boxstartcol+1, menuwidth-1); //內用外帶下方橫線
    drawhline(pizzatasteselline+TOTAL_PIZZA_TASTE+1, boxstartcol+1, menuwidth-1); //口味下方橫線
    //內用外帶選項：
    char in[30], out[30];
    strcenter(in, "1.內用", 6, inoutoptlength);
    strcenter(out, "2.外帶", 6, inoutoptlength);
    gotorc(inoutselline, boxstartcol+1);
    printf("%s%s", in, out);
    gotorc(cmdline, 1);
    char inoutch = ask("請選擇 [1]內用 或是 [2]外帶 ：", "12", cmdline, errorline);
    int inout = chartoint(inoutch)-1;
    gotorc(inoutselline, boxstartcol+1+inout*inoutoptlength);
    printcolor((inout==0)?in:out, YELLOW);
    //口味選項：
    Pizza tastes[TOTAL_PIZZA_TASTE] = {
        {"夏威夷", 450, false},
        {"巧克力香蕉", 480, false},
        {"蔬食", 380, true},
        {"瑪格麗特", 350, false},
        {"總匯", 500, false}
    };
    for(i = 0; i < TOTAL_PIZZA_TASTE; i++)
    {
        gotorc(pizzatasteselline+i, boxstartcol+indent);
        char tastename[20];
        printf("%d.%s - %d元", i+1, strleft(tastename, tastes[i].name, (int)strlen(tastes[i].name)/3*2, 10), tastes[i].price);
    }
    char tastech = ask("請選擇披薩種類[1-5]：", "12345", cmdline, errorline);
    int taste = chartoint(tastech)-1;
    gotorc(pizzatasteselline+taste, boxstartcol+indent);
    {
        char tastename[20], tasteselect[40];
        sprintf(tasteselect, "%d.%s - %d元", taste+1, strleft(tastename, tastes[taste].name, (int)strlen(tastes[taste].name)/3*2, 10), tastes[taste].price);
        printcolor(tasteselect, GREEN);
    }
    //配料選項：
    char ingrech = ask("請選擇欲加點的配料[0-5]：", "012345", cmdline, errorline);
    clearscr();
    gotorc(1, 1);
    return 0;
}
