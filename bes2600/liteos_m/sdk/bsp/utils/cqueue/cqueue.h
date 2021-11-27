/*
 * Copyright (c) 2021 Bestechnic (Shanghai) Co., Ltd. All rights reserved.
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
#ifndef C_QUEUE_H
#define C_QUEUE_H 1

#if defined(__cplusplus)
extern "C" {
#endif

enum {
    CQ_OK = 0,
    CQ_ERR,
};

typedef unsigned char CQItemType;

typedef struct __CQueue
{
    int read;
    int write;
    int size;
    int len;
    CQItemType *base;
} CQueue;

typedef uint16_t (*FRAME_LEN_GETTER_T)(CQueue* q);

/* Init Queue */
int InitCQueue(CQueue *Q, unsigned int size, CQItemType *buf);
/* Is Queue Empty */
int IsEmptyCQueue(CQueue *Q);
/* Filled Length Of Queue */
int LengthOfCQueue(CQueue *Q);
/* Empty Length Of Queue */
int AvailableOfCQueue(CQueue *Q);
/* Push Data Into Queue (Tail) */
int EnCQueue(CQueue *Q, CQItemType *e, unsigned int len);
/* Push Data Into Queue (Tail) */
int EnCQueue_AI(CQueue *Q, CQItemType *e, unsigned int len, FRAME_LEN_GETTER_T getter);
/* Push Data Into Front Of Queue */
int EnCQueueFront(CQueue *Q, CQItemType *e, unsigned int len);
/* Pop Data Data From Queue (Front) */
int DeCQueue(CQueue *Q, CQItemType *e, unsigned int len);
/* Peek But Not Pop Data From Queue (Front) */
int PeekCQueue(CQueue *Q, unsigned int len_want, CQItemType **e1, unsigned int *len1, CQItemType **e2, unsigned int *len2);
/* Peek data to buf e, But Not Pop Data From Queue (Front) */
int PeekCQueueToBuf(CQueue *Q, CQItemType *e, unsigned int len);
int PullCQueue(CQueue *Q, CQItemType *e, unsigned int len);
/* Dump Queue */
int DumpCQueue(CQueue *Q);

void ResetCQueue(CQueue *Q);

unsigned int GetCQueueReadOffset(CQueue *Q);
unsigned int GetCQueueWriteOffset(CQueue *Q);
int PeekCQueueToBufWithOffset(CQueue *Q, CQItemType *e, unsigned int len_want, unsigned int offset);

#if defined(__cplusplus)
}
#endif

#endif /* C_QUEUE_H */
