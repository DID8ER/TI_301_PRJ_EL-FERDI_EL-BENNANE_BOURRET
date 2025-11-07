#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"

static char *getID(int i)
{
    static char buffer[10];
    char temp[10];
    int index = 0;
    while (i >= 0)
    {
        temp[index++] = 'A' + (i % 26);
        i = (i / 26) - 1;
    }
    for (int j = 0; j < index; j++)
    {
        buffer[j] = temp[index - j - 1];
    }
    buffer[index] = '\0';
    return buffer;
}



char *getId(int num)
{
    static char buf[8];
    int i = 0;
    if (num <= 0) {
        buf[0] = 'A';
        buf[1] = '\0';
        return buf;
    }
    while (num > 0 && i < (int)sizeof(buf) - 1) {
        buf[i++] = (char)('A' + (num - 1) % 26);
        num = (num - 1) / 26;
    }
    buf[i] = '\0';
    for (int j = 0; j < i / 2; ++j) {
        char t = buf[j];
        buf[j] = buf[i - 1 - j];
        buf[i - 1 - j] = t;
    }
    return buf;
}