/*
 * Copyright (c) 2021 bestechnic (Shanghai) Technologies CO., LIMITED.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __LZMADECODE_H
#define __LZMADECODE_H


/* #define _LZMA_IN_CB */
/* Use callback for input data */

/* #define _LZMA_OUT_READ */
/* Use read function for output data */

/* #define _LZMA_PROB32 */
/* it's not suitable for genie project, memory usage double only */
/* It can increase speed on some 32-bit CPUs,
   but memory usage will be doubled in that case */

#ifndef UInt32
#ifdef _LZMA_UINT32_IS_ULONG
#define UInt32 unsigned long
#else
#define UInt32 unsigned int
#endif
#endif

#ifdef _LZMA_PROB32
#define CProb UInt32
#else
#define CProb unsigned short
#endif

#define LZMA_RESULT_OK 0
#define LZMA_RESULT_DATA_ERROR 1

#ifdef _LZMA_IN_CB
typedef struct _ILzmaInCallback
{
  int (*Read)(void *object, const unsigned char **buffer, unsigned int *bufferSize);
} ILzmaInCallback;
#endif

#define LZMA_BASE_SIZE 1846
#define LZMA_LIT_SIZE 768

#define LZMA_PROPERTIES_SIZE 5

typedef struct _CLzmaProperties
{
  int lc;
  int lp;
  int pb;
  #ifdef _LZMA_OUT_READ
  UInt32 DictionarySize;
  #endif
}CLzmaProperties;

int LzmaDecodeProperties(CLzmaProperties *propsRes, const unsigned char *propsData, int size);

#define LzmaGetNumProbs(Properties) (LZMA_BASE_SIZE + (LZMA_LIT_SIZE << ((Properties)->lc + (Properties)->lp)))

#define kLzmaNeedInitId (-2)

typedef struct _CLzmaDecoderState
{
  CLzmaProperties Properties;
  CProb *Probs;

  #ifdef _LZMA_IN_CB
  const unsigned char *Buffer;
  const unsigned char *BufferLim;
  #endif

  #ifdef _LZMA_OUT_READ
  unsigned char *Dictionary;
  UInt32 Range;
  UInt32 Code;
  UInt32 DictionaryPos;
  UInt32 GlobalPos;
  UInt32 DistanceLimit;
  UInt32 Reps[4];
  int State;
  int RemainLen;
  unsigned char TempDictionary[4];
  #endif
} CLzmaDecoderState;

#ifdef _LZMA_OUT_READ
#define LzmaDecoderInit(vs) { (vs)->RemainLen = kLzmaNeedInitId; }
#endif

int LzmaDecode(CLzmaDecoderState *vs,
    #ifdef _LZMA_IN_CB
    ILzmaInCallback *inCallback,
    #else
    const unsigned char *inStream, unsigned int inSize, unsigned int *inSizeProcessed,
    #endif
    unsigned char *outStream, unsigned int outSize, unsigned int *outSizeProcessed);

#endif
