
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;

void DelayMS(int ms);

void printfBytes(const char* label,const u8* data, int len);
void printHex(const u8* d, int len);

#ifndef min
#define min(_a,_b) ((_a) < (_b) ? (_a) : (_b))
#endif

inline int LE16(const u8* d)
{
    return d[0] | (d[1] << 8);
}

inline u32 BE32(const u8* d)
{
    return (d[0] << 24) | (d[1] << 16) | (d[2] << 8) | d[3];
}

inline void BE32(u32 n, u8* d)
{
    d[0] = (u8)(n >> 24);
    d[1] = (u8)(n >> 16);
    d[2] = (u8)(n >> 8);
    d[3] = (u8)n;
}

inline void BE16(u32 n, u8* d)
{
    d[0] = (u8)(n >> 8);
    d[1] = (u8)n;
}
