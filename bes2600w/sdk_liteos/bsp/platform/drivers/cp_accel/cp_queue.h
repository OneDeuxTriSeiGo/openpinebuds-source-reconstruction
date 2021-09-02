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
#ifndef CP_QUEUE_H
#define CP_QUEUE_H 1

#if defined(__cplusplus)
extern "C" {
#endif

#include "cqueue.h"

/* Init Queue */
int InitCpQueue(CQueue *Q, unsigned int size, CQItemType *buf);
/* Is Queue Empty */
int IsEmptyCpQueue(CQueue *Q);
/* Filled Length Of Queue */
int LengthOfCpQueue(CQueue *Q);
/* Empty Length Of Queue */
int AvailableOfCpQueue(CQueue *Q);
/* Push Data Into Queue (Tail) */
int EnCpQueue(CQueue *Q, CQItemType *e, unsigned int len);
/* Pop Data Data From Queue (Front) */
int DeCpQueue(CQueue *Q, CQItemType *e, unsigned int len);
/* Peek But Not Pop Data From Queue (Front) */
int PeekCpQueue(CQueue *Q, unsigned int len_want, CQItemType **e1, unsigned int *len1, CQItemType **e2, unsigned int *len2);

void ResetCpQueue(CQueue *Q);

#if defined(__cplusplus)
}
#endif

#endif /* CP_QUEUE_H */
