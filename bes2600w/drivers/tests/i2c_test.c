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
#include "i2c_if.h"
#include "platform.h"

#define AHT10_ADDR 0x38 /* slave address */

static uint8_t ath10_calibrate_cmd[] = {0xE1, 0x08, 0x00};
static uint8_t ath10_measure_cmd[] = {0xAC, 0x33, 0x00};
static uint8_t ath10_reset_cmd[] = {0xBA};

/**
 * default I2C0_IOMUX_INDEX 4, SCL - PIN04, SDA - PIN05
 */
struct aht10 {
    DevHandle i2c;
    uint16_t i2c_id;
    uint16_t i2c_addr;
};

static struct aht10 priv = {
    .i2c_id = 0,
    .i2c_addr = AHT10_ADDR,
};

static void aht10_init(struct aht10 *priv)
{
    struct I2cMsg msg = {
        .addr = priv->i2c_addr,
        .flags = 0,
        .len = sizeof(ath10_calibrate_cmd),
        .buf = ath10_calibrate_cmd,
    };
    if (I2cTransfer(priv->i2c, &msg, 1) != 1) {
        LOG_E("ath10_calibrate failed");
        return;
    }
    DelayMs(40);
}

static void ath10_reset(struct aht10 *priv)
{
    struct I2cMsg msg = {
        .addr = priv->i2c_addr,
        .flags = 0,
        .len = sizeof(ath10_reset_cmd),
        .buf = ath10_reset_cmd,
    };
    if (I2cTransfer(priv->i2c, &msg, 1) != 1) {
        LOG_E("ath10_reset failed");
        return;
    }
    DelayMs(20);
}

static void aht10_read_temperature_humidity(struct aht10 *priv, double *temperature, double *humidity)
{
    uint8_t data[6] = {0};
    struct I2cMsg msgs[] = {
        {
            .addr = priv->i2c_addr,
            .flags = 0,
            .len = sizeof(ath10_measure_cmd),
            .buf = ath10_measure_cmd,
        },
        {
            .addr = priv->i2c_addr,
            .flags = I2C_FLAG_READ,
            .len = sizeof(data),
            .buf = data,
        },
    };
    if (I2cTransfer(priv->i2c, &msgs[0], 1) != 1) {
        LOG_E("ath10_measure failed");
        return;
    }
    DelayMs(80);
    if (I2cTransfer(priv->i2c, &msgs[1], 1) != 1) {
        LOG_E("ath10_measure failed");
        return;
    }
    LOG_I("rx: %02X %02X %02X %02X %02X %02X\r\n", data[0], data[1], data[2], data[3], data[4], data[5]);
    *humidity = (data[1] << 12 | data[2] << 4 | (data[3] & 0xf0) >> 4) * 100.0 / (1 << 20);
    *temperature = ((data[3] & 0xf) << 16 | data[4] << 8 | data[5]) * 200.0 / (1 << 20) - 50;
}

void i2c_test(void)
{
    priv.i2c = I2cOpen(priv.i2c_id);
    DBG_ASSERT(priv.i2c != NULL);
    ath10_reset(&priv);
    aht10_init(&priv);
    for (int i = 0; i < 30; i++) {
        double temperature = 0.0;
        double humidity = 0.0;
        aht10_read_temperature_humidity(&priv, &temperature, &humidity);
        LOG_I("aht10 temperature %.2f℃, humidity %.0f%%\r\n", temperature, humidity);
        DelayMs(1000);
    }
}