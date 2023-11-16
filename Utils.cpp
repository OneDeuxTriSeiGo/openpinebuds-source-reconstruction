

#include "mbed.h"
#include "Utils.h"

void printfBytes(const char* s, const u8* data, int len)
{
    printf("%s %d:",s,len);
    if (len > 256)
        len = 256;
    while (len-- > 0)
        printf(" %02X",*data++);
    printf("\n");
}

void printHexLine(const u8* d, int addr, int len)
{
    printf("%04X ",addr);
    int i;
    for (i = 0; i < len; i++)
        printf("%02X ",d[i]);
    for (;i < 16; i++)
        printf("   ");
    char s[16+1];
    memset(s,0,sizeof(s));
    for (i = 0; i < len; i++)
    {
        int c = d[i];
        if (c < 0x20 || c > 0x7E)
            c = '.';
        s[i] = c;
    }
    printf("%s\n",s);
}

void printHex(const u8* d, int len)
{
    int addr = 0;
    while (len)
    {
        int count = len;
        if (count > 16)
            count = 16;
        printHexLine(d+addr,addr,count);
        addr += 16;
        len -= count;
    }
}
