#include "stringutil.h"
#include <stdio.h>

void strcenter(char* result , char* str, int length, int space)
{
    if(length >= space)
        return;
    else
    {
        int prefixpadding = (space-length)/2;
        int suffixpadding = space - length - prefixpadding;
        sprintf(result, "%*s%s%*s", prefixpadding, "", str, suffixpadding, "");
    }
}

int chartoint(char c)
{
    return c - '0';
}
