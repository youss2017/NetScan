#include "dynstr.h"
#define _CRT_SECURE_NO_WARNINGS
#include <string.h>
#include <stdlib.h>

STRING InitString()
{
    STRING s = {
        .nSize = 9
    };
    s.str = (char*)malloc(10);
    s.str[0] = '\0';
    return s;
}

void AppendString(STRING* s, const char* appendStr)
{
    int appendSize = strlen(appendStr);
    if (s->nSize > appendSize) {
        strcat(s->str, appendStr);
        s->nSize -= appendSize;
    }
    else {
        char* newdst = (char*)malloc(strlen(s->str) + appendSize + 1 + 25);
        strcpy(newdst, s->str);
        strcat(newdst, appendStr);
        free(s->str);
        s->str = newdst;
        s->nSize = 25;
    }
}

void FreeString(STRING* s)
{
    free(s->str);
}
