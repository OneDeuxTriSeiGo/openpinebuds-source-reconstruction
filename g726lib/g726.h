#ifndef __G726_H
#define __G726_H

extern void g726_Encode(unsigned char *speech,char *bitstream);
extern void g726_Decode(char *bitstream,unsigned char *speech);

#endif
