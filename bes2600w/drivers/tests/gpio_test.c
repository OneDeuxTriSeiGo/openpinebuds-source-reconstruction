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
#include "gpio_if.h"
#include "platform.h"

static uint16_t gpio02 = 2;
static uint16_t gpio04 = 4;
static uint16_t gpio05 = 5;

static void gpio_output_test(void)
{
    LOG_I("set gpio %d output", gpio02);
    GpioSetDir(gpio02, GPIO_DIR_OUT);
    LOG_I("set gpio %d low", gpio02);
    GpioWrite(gpio02, GPIO_VAL_LOW);
    DelayMs(500);
    LOG_I("set gpio %d high", gpio02);
    GpioWrite(gpio02, GPIO_VAL_HIGH);
    DelayMs(500);
}

static void gpio_input_test(void)
{
    int cnt = 0;
    LOG_I("set gpio %d input", gpio04);
    GpioSetDir(gpio04, GPIO_DIR_IN);
    while (cnt++ < 100) {
        uint16_t val = 0;
        GpioRead(gpio04, &val);
        LOG_I("gpio %d val %d", gpio04, val);
        DelayMs(100);
    }
}

static void gpio_irq_handler(uint16_t gpio, void *data)
{
    if (gpio != gpio05 || !data) {
        LOG_E("unexpected irq from gpio %d", gpio);
        return;
    }
    uint16_t val = 0;
    GpioRead(gpio, &val);
    LOG_I("arg %d, gpio %d, val %d", *(int *)data, gpio, val);
    LOG_I("disable irq");
    GpioDisableIrq(gpio);
}

static void gpio_irq_test(void)
{
    static int val = 1234;
    LOG_I("set gpio %d input", gpio05);
    GpioSetDir(gpio05, GPIO_DIR_IN);
    GpioSetIrq(gpio05, GPIO_IRQ_TRIGGER_RISING, gpio_irq_handler, &val);
    GpioEnableIrq(gpio05);
}

void gpio_test()
{
    gpio_output_test();
    gpio_input_test();
    gpio_irq_test();
}