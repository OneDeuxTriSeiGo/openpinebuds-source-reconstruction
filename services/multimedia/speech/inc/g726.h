// SPDX-License-Identifier: MIT

#ifndef __G726_H
#define __G726_H

#ifdef __cplusplus
extern "C" {
#endif

extern void g726_Encode(unsigned char *speech,char *bitstream);
extern void g726_Decode(char *bitstream,unsigned char *speech);

#ifdef __cplusplus
}
#endif

#endif
