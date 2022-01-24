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
#ifndef KFIFO_H
#define KFIFO_H 1

#include "plat_types.h"

#if defined(__cplusplus)
extern "C" {
#endif

struct kfifo {
    unsigned char *buffer;     /* the buffer holding the data */
    unsigned int size;         /* the size of the allocated buffer */
    unsigned int in;           /* data is added at offset (in % size) */
    unsigned int out;          /* data is extracted from off. (out % size) */
};

void kfifo_init(struct kfifo *k, unsigned char *buff, unsigned int len);
unsigned int kfifo_put(struct kfifo *k, unsigned char *buff, unsigned int len);
unsigned int kfifo_get(struct kfifo *k, unsigned char *buff, unsigned int len);
unsigned int kfifo_peek(struct kfifo *k, unsigned int len_want, unsigned char **buff1, unsigned char **buff2, unsigned int *len1, unsigned int *len2);
unsigned int kfifo_peek_to_buf(struct kfifo *fifo, unsigned char *buff, unsigned int len);
unsigned int kfifo_len(struct kfifo *fifo);
unsigned int kfifo_available(struct kfifo *fifo);

#if defined(__cplusplus)
}
#endif

#endif /* KFIFO_H */
