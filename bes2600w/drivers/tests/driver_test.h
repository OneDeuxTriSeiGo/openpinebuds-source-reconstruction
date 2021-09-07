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
#ifndef __DRIVER_TEST_H__
#define __DRIVER_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

void gpio_test(void);
void i2c_test(void);
void spi_test(void);
void touch_test(void);
void display_test(void);

#ifdef __cplusplus
}
#endif

#endif
