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

//詢問函數，question為題目，options為合法選項字元，quesline為題目印出行數位置，errorline為錯誤輸出行數位置
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
        if(ch == EOF || ch == '\n') //若為EOF或換行字元，不進flush以免再度停頓在getchar()
            err = true;
        else if(flush_in() > 0) //flush函式回傳值為不包括\n或EOF字元的被清除字元數，若大於0即代表為不合法輸入
            err = true;
        else if(!strchr(options, ch)) //檢查是否在合法選項字元之內
            err = true;
        else
        {
            gotorc(errorline, 1);
            printf("\e[2K"); //當輸入無誤時，清除錯誤行
            err = false;
        }
    } while(err);
    return ch;
}

//檢查配料輸入是否合法
bool ingrecheck(Ingre ingres[], int ingresel, Pizza tastes[], int tastesel, int errorline)
{
    if(ingres[ingresel].amount == 0) //檢查配料是否有庫存
    {
        gotorc(errorline, 1);
        printcolor("這個配料已經沒有庫存了，請重新輸入！", RED);
        return false;
    }
    else if(tastes[tastesel].vegetarian && ingres[ingresel].meat) //檢查是否為蔬食披薩搭配肉類配料
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
    int boxstartrow = 1, boxstartcol = 1, boxwidth = 80, boxheight = 22, titleheight = 2, menuwidth = 29, pricewidth = 16, cmdline = 23, errorline = 24,
        ordernamestart = boxstartcol+menuwidth+1, ordernamewidth = boxwidth-2-menuwidth-pricewidth-1, pricecolstart = boxwidth-pricewidth,
        tasteindent = 5, inoutselline = 4, inoutoptlength = (menuwidth-1)/2, subnameline = inoutselline, pizzatasteselline = inoutselline+3,
        ingreindent = 3, ingreleftindent = strlen("1.NAME_COL - COSTx00 |"), ingreselline = pizzatasteselline+TOTAL_PIZZA_TASTE+3,
        totalpriceline = boxheight-1, deliverline = totalpriceline-2, discountline = deliverline-1, sublinemax = discountline-subnameline-2;
    int i = 0, sublineappend = 0;
    char *sys_name = "披　薩　點　餐　系　統";
    clearscr();
    //畫出點餐系統的表格
    drawbox(boxstartrow, boxstartcol, boxheight, boxwidth);
    gotorc(boxstartrow+1, (boxwidth-(int)strlen(sys_name)/3*2)/2); //由於計算字串長度真的很複雜，在已知此字串為UTF-8編碼且全中文的情況下，直接除以1.5得終端機上顯示字寬
    printf("%s", sys_name);
    drawhline(boxstartrow+titleheight, boxstartcol+1, boxwidth-2); //標題下方橫線
    drawvline(boxstartcol+menuwidth, boxstartrow+1+titleheight, boxheight-2-titleheight); //分隔左右側的直線
    drawvline(boxwidth-pricewidth-1, boxstartrow+1+titleheight, boxheight-2-titleheight); //分隔品名與價格區的直線
    drawhline(inoutselline+1, boxstartcol+1, menuwidth-1); //內用外帶下方橫線
    drawhline(inoutselline+1, boxstartcol+menuwidth+1, boxwidth-menuwidth-2); //品名價格下方橫線
    drawhline(pizzatasteselline+TOTAL_PIZZA_TASTE+1, boxstartcol+1, menuwidth-1); //口味下方橫線
    drawhline(totalpriceline-1, boxstartcol+menuwidth+1, boxwidth-menuwidth-2); //總計上方橫線
    //建立披薩口味清單
    Pizza tastes[TOTAL_PIZZA_TASTE] = {
        {"夏威夷", 450, false},
        {"巧克力香蕉", 480, false},
        {"蔬食", 380, true},
        {"瑪格麗特", 350, false},
        {"總匯", 500, false}
    };
    //建立配料清單
    Ingre ingres[TOTAL_INGRE] = {
        {"不加配料", -1, false, -1},
        {"起司", 30, false, 5},
        {"香腸", 35, true, 5},
        {"章魚", 40, true, 5},
        {"鳳梨", 20, false, 5},
        {"海苔", 10, false, 5}
    }; //-1象徵無用值
    //設定右方訂單區標題
    {
        char tempsub[80];
        gotorc(subnameline, ordernamestart);
        printf("%s", strcenter(tempsub, "點　餐　項　目", 14, ordernamewidth));
        gotorc(subnameline, pricecolstart);
        printf("%s", strcenter(tempsub, "價　格", 6, pricewidth));
        gotorc(totalpriceline, ordernamestart);
        printf("%s", strright(tempsub, "總　計", 6, ordernamewidth));
    }
    int totalprice = 0, othersum = 0; //othersum是為了在訂購項目滿出來時，顯示「其他…」的那項做金額加總用的
    //內用外帶選項：
    char in[30], out[30];
    //置中選項
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
    bool pizzaordermore = false;
    do
    {
        //口味選項：
        //印出各種口味與價錢
        for(i = 0; i < TOTAL_PIZZA_TASTE; i++)
        {
            gotorc(pizzatasteselline+i, boxstartcol+tasteindent);
            char tastename[20];
            printf("%d.%s - %d元", i+1, strleft(tastename, tastes[i].name, (int)strlen(tastes[i].name)/3*2, 10), tastes[i].price);
        }
        char tastech = ask("請選擇披薩種類[1-5]：", "12345", cmdline, errorline);
        int taste = chartoint(tastech)-1; //-1的目的是為了對應陣列
        totalprice += tastes[taste].price; //價錢加總
        //回到上方選單處上色
        gotorc(pizzatasteselline+taste, boxstartcol+tasteindent);
        {
            char tastename[30], tasteselect[80];
            sprintf(tasteselect, "%d.%s - %d元", taste+1, strleft(tastename, tastes[taste].name, (int)strlen(tastes[taste].name)/3*2, 10), tastes[taste].price);
            printcolor(tasteselect, GREEN);
            //在一旁的訂購清單添加項目
            sublineappend++;
            //檢查右側是否已滿，若已滿則以「其他項目…」代替
            if(sublineappend < sublinemax)
            {
                gotorc(subnameline+1+sublineappend, ordernamestart); //+1是因為有那條橫線存在
                sprintf(tastename, "%s口味披薩", tastes[taste].name);
                printf("%s", strleft(tasteselect, tastename, (int)strlen(tastename)/3*2, ordernamewidth));
                gotorc(subnameline+1+sublineappend, pricecolstart); //+1是因為有那條橫線存在
                sprintf(tastename, "%d元", tastes[taste].price);
                printf("%s", strright(tasteselect, tastename, strlen(tastename)-strlen("元")+2, pricewidth));
            }
            else
            {
                //檢查是否要印出「其他項目…」，若已經印過則可以不用再印
                if(sublineappend == sublinemax)
                {
                    gotorc(subnameline+1+sublinemax, ordernamestart); //+1是因為有那條橫線存在
                    printf("%s", strleft(tasteselect, "其他項目…", (int)strlen("其他項目…")/3*2, ordernamewidth));
                }
                gotorc(subnameline+1+sublinemax, pricecolstart); //+1是因為有那條橫線存在
                othersum += tastes[taste].price; //othersum項目加總
                sprintf(tastename, "%d元", othersum);
                printf("%s", strright(tasteselect, tastename, strlen(tastename)-strlen("元")+2, pricewidth));
            }
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
            //檢查是否是不用配料選項，其印法與其他配料不同，不會印出價錢、已選數目以及庫存
            if(ingres[i].price < 0 && ingres[i].amount < 0)
            {
                printf("%d.%s", i, ingres[i].name);
            }
            else
            {
                printf("%d.%s - %d元×%2d", i, strleft(ingrename, ingres[i].name, (int)strlen(ingres[i].name)/3*2, 8), ingres[i].price, ingreselamount[i]);
                //檢查庫存數量，當庫存數量歸零時則上色
                if(ingres[i].amount > 0)
                {
                    printf(" | %2d", ingres[i].amount);
                }
                else
                {
                    char ingreleftamount[8];
                    sprintf(ingreleftamount, " | %2d", ingres[i].amount);
                    printcolor(ingreleftamount, RED);
                }
            }
        }
        do
        {
            int ingresel = 0;
            do
            {
                char ingrech = ask("請選擇欲加點的配料[0-5]：", "012345", cmdline, errorline);
                ingresel = chartoint(ingrech);
            }while(!ingrecheck(ingres, ingresel, tastes, taste, errorline)); //檢查配料選擇是否合法
            //清除可能出現的錯誤訊息
            gotorc(errorline, 1);
            printf("\e[2K");
            //回到上方選單處上色
            gotorc(ingreselline+ingresel, boxstartcol+ingreindent);
            {
                char ingrename[20], ingreselstr[50];
                //檢查是否是不用配料選項，其印法與其他配料不同，不會印出價錢、已選數目以及庫存
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
                    //檢查庫存數量，當庫存數量歸零時則上色
                    if(ingres[ingresel].amount > 0)
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
            //若選擇非不加配料時，詢問是否繼續加選配料
            if(ingresel != 0)
            {
                char ingreselconti = ask("是否還要繼續加選配料？[1-是, 2-否]：", "12", cmdline, errorline);
                ingreselmore = (chartoint(ingreselconti)==1)?true:false;
            }
            else
                ingreselmore = false;
        }while(ingreselmore);
        int ingresum = 0;
        //計算所選配料總價
        for(i = 1; i < TOTAL_INGRE; i++)
        {
            ingresum += ingres[i].price * ingreselamount[i];
        }
        totalprice += ingresum; //價錢加總
        //放入旁邊的點餐選項
        if(ingresum > 0)
        {
            //在一旁的訂購清單添加項目
            sublineappend++;
            char temp[80], temp2[30];
            //檢查右側是否已滿，若已滿則以「其他項目…」代替
            if(sublineappend < sublinemax)
            {
                gotorc(subnameline+1+sublineappend, ordernamestart);
                printf("%s", strright(temp, "加選披薩配料", 12, ordernamewidth));
                gotorc(subnameline+1+sublineappend, pricecolstart);
                sprintf(temp2, "%d元", ingresum);
                printf("%s", strright(temp, temp2, strlen(temp2)-strlen("元")+2, pricewidth));
            }
            else
            {
                //檢查是否要印出「其他項目…」，若已經印過則可以不用再印
                if(sublineappend == sublinemax)
                {
                    gotorc(subnameline+1+sublinemax, ordernamestart); //+1是因為有那條橫線存在
                    printf("%s", strleft(temp, "其他項目…", (int)strlen("其他項目…")/3*2, ordernamewidth));
                }
                gotorc(subnameline+1+sublinemax, pricecolstart); //+1是因為有那條橫線存在
                othersum += ingresum; //othersum項目加總
                sprintf(temp2, "%d元", othersum);
                printf("%s", strright(temp, temp2, strlen(temp2)-strlen("元")+2, pricewidth));
            }
        }
        char pizzaorderconti = ask("是否還要繼續加點其他披薩？[1-是, 2-否]：", "12", cmdline, errorline);
        pizzaordermore = (chartoint(pizzaorderconti)==1)?true:false;
        //檢查是否繼續加選披薩
        if(pizzaordermore)
        {
            //清除披薩選擇欄與配料選擇欄，以便稍候重新開始
            for(i = 0; i < TOTAL_PIZZA_TASTE; i++)
            {
                gotorc(pizzatasteselline+i, boxstartcol+1);
                printf("%*s", menuwidth-1, "");
            }
            for(i = -1; i < TOTAL_INGRE; i++) //從-1開始的原因是因為還有庫存一欄要清除
            {
                gotorc(ingreselline+i, boxstartcol+1);
                printf("%*s", menuwidth-1, "");
            }
        }
    }while(pizzaordermore);
    //判斷限時優惠
    if(totalprice > 1000)
    {
        char temp[80], temp2[20];
        //在右方訂購清單加上限時優惠項目
        gotorc(discountline, ordernamestart);
        printf("%s", strright(temp, "千元以上限時９折優惠", 20, ordernamewidth));
        gotorc(discountline, pricecolstart);
        int discount = totalprice*0.1;
        totalprice -= discount;
        sprintf(temp2, "%d元", discount*(-1));
        printf("%s", strright(temp, temp2, strlen(temp2)-strlen("元")+2, pricewidth));
    }
    //外送運費計算
    if(inout == 1)
    {
        char temp[80], temp2[20];
        //在右方訂購清單加上運費項目
        gotorc(deliverline, ordernamestart);
        printf("%s", strright(temp, "外送10％運費", 12, ordernamewidth));
        gotorc(deliverline, pricecolstart);
        int deliver = totalprice*0.1;
        totalprice += deliver;
        sprintf(temp2, "%d元", deliver);
        printf("%s", strright(temp, temp2, strlen(temp2)-strlen("元")+2, pricewidth));
    }
    //顯示總計
    gotorc(totalpriceline, pricecolstart);
    char sumtext[20], sumtemp[40];
    sprintf(sumtext, "%d元", totalprice);
    printcolor(strright(sumtemp, sumtext, strlen(sumtext)-strlen("元")+2, pricewidth), YELLOW);
    gotorc(cmdline, 1);
    printf("\e[2K"); //清除該行
    printf("總計金額為：");
    printcolor(sumtext, YELLOW);
    printf("，感謝使用本點餐系統！");
    //clearscr();
    gotorc(24, 1);
    return 0;
}
