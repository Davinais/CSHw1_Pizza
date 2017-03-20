#include "termctrl.h"
#include "stringutil.h"
#include <string.h>
#include <stdbool.h>
#define TOTAL_PIZZA_TASTE 5 //const int 放在array裡面還是不給我過，只好用define下策
#define TOTAL_INGRE 6

typedef struct pizza
{
    char name[20];
    int price;
    bool vegetarian;
}Pizza;

typedef struct ingredient
{
    char name[15];
    int price;
    bool meat;
    int amount;
}Ingre;

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
            printf("\e[2K");
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

bool ingrecheck(Ingre ingres[], int ingresel, Pizza tastes[], int tastesel, int errorline)
{
    if(ingres[ingresel].amount == 0)
    {
        gotorc(errorline, 1);
        printcolor("這個配料已經沒有庫存了，請重新輸入！", RED);
        return false;
    }
    else if(tastes[tastesel].vegetarian && ingres[ingresel].meat)
    {
        gotorc(errorline, 1);
        printcolor("蔬食披薩無法搭配肉類配料喔，請重新輸入！", RED);
        return false;
    }
    else
        return true;
}

int main(void)
{
    int boxstartrow = 1, boxstartcol = 1, boxwidth = 80, boxheight = 22, titleheight = 2, menuwidth = 29, cmdline = 23, errorline = 24,
        tasteindent = 5, inoutselline = 4, inoutoptlength = (menuwidth-1)/2, pizzatasteselline = inoutselline+3,
        ingreindent = 3, ingreleftindent = 2+8+3+2+2+1+2+2, ingreselline = pizzatasteselline+TOTAL_PIZZA_TASTE+3;
    int i = 0;
    char *sys_name = "披　薩　點　餐　系　統";
    clearscr();
    drawbox(boxstartrow, boxstartcol, boxheight, boxwidth);
    gotorc(boxstartrow+1, (boxwidth-(int)strlen(sys_name)/3*2)/2); //由於計算字串長度真的很複雜，在已知此字串為UTF-8編碼且全中文的情況下，直接除以1.5得終端機上顯示字寬
    printf("%s", sys_name);
    drawhline(boxstartrow+titleheight, boxstartcol+1, boxwidth-2); //標題下方橫線
    drawvline(boxstartcol+menuwidth, boxstartrow+1+titleheight, boxheight-2-titleheight); //分隔左右側的直線
    drawhline(inoutselline+1, boxstartcol+1, menuwidth-1); //內用外帶下方橫線
    drawhline(inoutselline+1, boxstartcol+menuwidth+1, boxwidth-menuwidth-2);
    drawhline(pizzatasteselline+TOTAL_PIZZA_TASTE+1, boxstartcol+1, menuwidth-1); //口味下方橫線
    Pizza tastes[TOTAL_PIZZA_TASTE] = {
        {"夏威夷", 450, false},
        {"巧克力香蕉", 480, false},
        {"蔬食", 380, true},
        {"瑪格麗特", 350, false},
        {"總匯", 500, false}
    };
    Ingre ingres[TOTAL_INGRE] = {
        {"不加配料", -1, false, -1},
        {"起司", 30, false, 5},
        {"香腸", 35, true, 5},
        {"章魚", 40, true, 5},
        {"鳳梨", 20, false, 5},
        {"海苔", 10, false, 5}
    }; //-1象徵無用值
    //內用外帶選項：
    char in[30], out[30];
    strcenter(in, "1.內用", 6, inoutoptlength);
    strcenter(out, "2.外帶", 6, inoutoptlength);
    gotorc(inoutselline, boxstartcol+1);
    printf("%s%s", in, out);
    gotorc(cmdline, 1);
    char inoutch = ask("請選擇 [1]內用 或是 [2]外帶 ：", "12", cmdline, errorline);
    int inout = chartoint(inoutch)-1;
    //回到上方選單處上色
    gotorc(inoutselline, boxstartcol+1+inout*inoutoptlength);
    printcolor((inout==0)?in:out, YELLOW);
    //口味選項：
    for(i = 0; i < TOTAL_PIZZA_TASTE; i++)
    {
        gotorc(pizzatasteselline+i, boxstartcol+tasteindent);
        char tastename[20];
        printf("%d.%s - %d元", i+1, strleft(tastename, tastes[i].name, (int)strlen(tastes[i].name)/3*2, 10), tastes[i].price);
    }
    char tastech = ask("請選擇披薩種類[1-5]：", "12345", cmdline, errorline);
    int taste = chartoint(tastech)-1;
    //回到上方選單處上色
    gotorc(pizzatasteselline+taste, boxstartcol+tasteindent);
    {
        char tastename[20], tasteselect[40];
        sprintf(tasteselect, "%d.%s - %d元", taste+1, strleft(tastename, tastes[taste].name, (int)strlen(tastes[taste].name)/3*2, 10), tastes[taste].price);
        printcolor(tasteselect, GREEN);
    }
    //配料選項：
    int ingreselamount[TOTAL_INGRE] = {0, 0, 0, 0, 0, 0};
    bool ingreselmore = false;
    gotorc(ingreselline-1, boxstartcol+ingreindent+ingreleftindent);
    printf("庫存");
    for(i = 0; i < TOTAL_INGRE; i++)
    {
        gotorc(ingreselline+i, boxstartcol+ingreindent);
        char ingrename[20];
        if(ingres[i].price < 0 && ingres[i].amount < 0)
        {
            printf("%d.%s", i, ingres[i].name);
        }
        else
        {
            printf("%d.%s - %d元×%2d | %2d", i, strleft(ingrename, ingres[i].name, (int)strlen(ingres[i].name)/3*2, 8), ingres[i].price,
                ingreselamount[i], ingres[i].amount);
        }
    }
    do
    {
        int ingresel = 0;
        do
        {
            char ingrech = ask("請選擇欲加點的配料[0-5]：", "012345", cmdline, errorline);
            ingresel = chartoint(ingrech);
        }while(!ingrecheck(ingres, ingresel, tastes, taste, errorline));
        //清除可能出現的錯誤訊息
        gotorc(errorline, 1);
        printf("\e[2K");
        //回到上方選單處上色
        gotorc(ingreselline+ingresel, boxstartcol+ingreindent);
        {
            char ingrename[20], ingreselstr[50];
            if(ingres[ingresel].price < 0 && ingres[ingresel].amount < 0)
            {
                if(ingreselmore) //檢查是否是加點時選的，若是加點時選的，不加配料選項不用上色
                {
                    printf("%d.%s", ingresel, ingres[ingresel].name);
                }
                else
                {
                    sprintf(ingreselstr, "%d.%s", ingresel, ingres[ingresel].name);
                    printcolor(ingreselstr, CYAN);
                }
            }
            else
            {
                ingreselamount[ingresel]++;
                ingres[ingresel].amount--;
                sprintf(ingreselstr, "%d.%s - %d元×%2d", ingresel, strleft(ingrename, ingres[ingresel].name, (int)strlen(ingres[ingresel].name)/3*2, 8), ingres[ingresel].price,
                    ingreselamount[ingresel]);
                printcolor(ingreselstr, CYAN);
                if(ingres[ingresel].amount > 0) //檢查庫存數量，當庫存數量歸零時則上色
                {
                    printf(" | %2d", ingres[ingresel].amount);
                }
                else
                {
                    char ingreleftamount[8];
                    sprintf(ingreleftamount, " | %2d", ingres[ingresel].amount);
                    printcolor(ingreleftamount, RED);
                }
            }
        }
        if(ingresel != 0)
        {
            char ingreselconti = ask("是否還要繼續加選配料？[1-是, 2-否]", "12", cmdline, errorline);
            ingreselmore = (chartoint(ingreselconti)==1)?true:false;
        }
        else
            ingreselmore = false;
    }while(ingreselmore);
    char pizzaorderconti = ask("是否還要繼續加點其他披薩？[1-是, 2-否]", "12", cmdline, errorline);
    clearscr();
    gotorc(1, 1);
    return 0;
}
