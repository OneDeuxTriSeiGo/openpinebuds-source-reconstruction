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
#ifndef ENCRYPT_H

#include "aes.h"

unsigned char* ecb_encrypt(unsigned char* in, unsigned char* key, unsigned char* (*some_encrypt)(unsigned char* in, unsigned char* out, unsigned char* key), unsigned int* n);

unsigned char* ecb_decrypt(unsigned char* in, unsigned char* key, unsigned char* (*some_decrypt)(unsigned char* in, unsigned char* out, unsigned char* key), unsigned int* n);

#endif // !ENCRYPT_H