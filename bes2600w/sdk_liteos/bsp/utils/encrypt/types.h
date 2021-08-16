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
#ifndef _UECC_TYPES_H_
#define _UECC_TYPES_H_

#ifndef uECC_PLATFORM
    #if __AVR__
        #define uECC_PLATFORM uECC_avr
    #elif defined(__thumb2__) || defined(_M_ARMT) /* I think MSVC only supports Thumb-2 targets */
        #define uECC_PLATFORM uECC_arm_thumb2
    #elif defined(__thumb__)
        #define uECC_PLATFORM uECC_arm_thumb
    #elif defined(__arm__) || defined(_M_ARM)
        #define uECC_PLATFORM uECC_arm
    #elif defined(__aarch64__)
        #define uECC_PLATFORM uECC_arm64
    #elif defined(__i386__) || defined(_M_IX86) || defined(_X86_) || defined(__I86__)
        #define uECC_PLATFORM uECC_x86
    #elif defined(__amd64__) || defined(_M_X64)
        #define uECC_PLATFORM uECC_x86_64
    #else
        #define uECC_PLATFORM uECC_arch_other
    #endif
#endif

#ifndef uECC_ARM_USE_UMAAL
    #if (uECC_PLATFORM == uECC_arm) && (__ARM_ARCH >= 6)
        #define uECC_ARM_USE_UMAAL 1
    #elif (uECC_PLATFORM == uECC_arm_thumb2) && (__ARM_ARCH >= 6) && !__ARM_ARCH_7M__
        #define uECC_ARM_USE_UMAAL 1
    #else
        #define uECC_ARM_USE_UMAAL 0
    #endif
#endif

#ifndef uECC_WORD_SIZE
    #if uECC_PLATFORM == uECC_avr
        #define uECC_WORD_SIZE 1
    #elif (uECC_PLATFORM == uECC_x86_64 || uECC_PLATFORM == uECC_arm64)
        #define uECC_WORD_SIZE 8
    #else
        #define uECC_WORD_SIZE 4
    #endif
#endif

#if (uECC_WORD_SIZE != 1) && (uECC_WORD_SIZE != 4) && (uECC_WORD_SIZE != 8)
    #error "Unsupported value for uECC_WORD_SIZE"
#endif

#if ((uECC_PLATFORM == uECC_avr) && (uECC_WORD_SIZE != 1))
    #pragma message ("uECC_WORD_SIZE must be 1 for AVR")
    #undef uECC_WORD_SIZE
    #define uECC_WORD_SIZE 1
#endif

#if ((uECC_PLATFORM == uECC_arm || uECC_PLATFORM == uECC_arm_thumb || \
        uECC_PLATFORM ==  uECC_arm_thumb2) && \
     (uECC_WORD_SIZE != 4))
    #pragma message ("uECC_WORD_SIZE must be 4 for ARM")
    #undef uECC_WORD_SIZE
    #define uECC_WORD_SIZE 4
#endif

#if defined(__SIZEOF_INT128__) || ((__clang_major__ * 100 + __clang_minor__) >= 302)
    #define SUPPORTS_INT128 1
#else
    #define SUPPORTS_INT128 0
#endif

typedef int8_t wordcount_t;
typedef int16_t bitcount_t;
typedef int8_t cmpresult_t;

#if (uECC_WORD_SIZE == 1)

typedef uint8_t uECC_word_t;
typedef uint16_t uECC_dword_t;

#define HIGH_BIT_SET 0x80
#define uECC_WORD_BITS 8
#define uECC_WORD_BITS_SHIFT 3
#define uECC_WORD_BITS_MASK 0x07

#elif (uECC_WORD_SIZE == 4)

typedef uint32_t uECC_word_t;
typedef uint64_t uECC_dword_t;

#define HIGH_BIT_SET 0x80000000
#define uECC_WORD_BITS 32
#define uECC_WORD_BITS_SHIFT 5
#define uECC_WORD_BITS_MASK 0x01F

#elif (uECC_WORD_SIZE == 8)

typedef uint64_t uECC_word_t;
#if SUPPORTS_INT128
typedef unsigned __int128 uECC_dword_t;
#endif

#define HIGH_BIT_SET 0x8000000000000000ull
#define uECC_WORD_BITS 64
#define uECC_WORD_BITS_SHIFT 6
#define uECC_WORD_BITS_MASK 0x03F

#endif /* uECC_WORD_SIZE */

#endif /* _UECC_TYPES_H_ */
