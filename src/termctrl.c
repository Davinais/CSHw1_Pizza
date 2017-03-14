#include "termctrl.h"

static struct termios ori_term, sys_term;

void startsystem()
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

void stopsystem()
{
    tcsetattr(0, TCSANOW, &ori_term);
    setvbuf(stdout, NULL, _IOLBF, BUFSIZ);
}

