#include "stringutil.h"
#include <stdio.h>

char* strcenter(char* result , char* str, int length, int space)
{
    if(space > length)
    {
        int prefixpadding = (space-length)/2;
        int suffixpadding = space - length - prefixpadding;
        sprintf(result, "%*s%s%*s", prefixpadding, "", str, suffixpadding, "");
        return result;
    }
    else
    {
        result = str;
        return str;
    }
}

char* strleft(char* result , char* str, int length, int space)
{
    if(space > length)
    {
        sprintf(result, "%s%*s", str, space-length, "");
        return result;
    }
    else
    {
        result = str;
        return str;
    }
}

char* strright(char* result , char* str, int length, int space)
{
    if(space > length)
    {
        sprintf(result, "%*s%s", space-length, "", str);
        return result;
    }
    else
    {
        result = str;
        return str;
    }
}

int chartoint(char c)
{
    return c - '0';
}
