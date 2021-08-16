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
#ifndef CMAC_H
#define CMAC_H

#define const_Bsize 16

void aes_cal_cmac(const unsigned char* in, unsigned int length, unsigned char* out, const unsigned char* key);
//bool verify_mac(unsigned char* in, unsigned int length, unsigned char* out, unsigned char* key);

void GenerateSubkey(const unsigned char* key, unsigned char* K1, unsigned char* K2);

#endif // !CMAC_H
