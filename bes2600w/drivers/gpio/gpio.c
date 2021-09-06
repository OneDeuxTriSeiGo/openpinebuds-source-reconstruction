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
#include <stdlib.h>
#include "gpio.h"
#include "hal_iomux.h"
#include "hal_iomux_best2003.h"
#include "gpio_if.h"
#include "device_resource_if.h"
#include "osal_irq.h"
#include "hal_trace.h"

#define HDF_LOG_TAG gpio_driver

struct oem_gpio_irq_handler {
    uint8_t port;
    GpioIrqFunc func;
    void *arg;
};

enum HAL_GPIO_PIN_T g_gpioPinReflectionMap[HAL_GPIO_PIN_LED_NUM] = {0};
static struct oem_gpio_irq_handler g_oemGpioIrqHandler[HAL_GPIO_PIN_LED_NUM] = {0};
static struct HAL_GPIO_IRQ_CFG_T gpio_irq_cfg[HAL_GPIO_PIN_LED_NUM] = {0};

static struct HAL_GPIO_IRQ_CFG_T hal_gpio_get_irq_config(enum HAL_GPIO_PIN_T pin)
{
    struct HAL_GPIO_IRQ_CFG_T irq_cfg;

    irq_cfg.irq_enable = gpio_irq_cfg[pin].irq_enable;
    irq_cfg.irq_debounce = gpio_irq_cfg[pin].irq_debounce;
    irq_cfg.irq_type = gpio_irq_cfg[pin].irq_type;
    irq_cfg.irq_polarity = gpio_irq_cfg[pin].irq_polarity;

    return irq_cfg;
}

static void OemGpioIrqHdl(enum HAL_GPIO_PIN_T pin)
{
    struct oem_gpio_irq_handler *hdlPointer = NULL;

    if (pin >= HAL_GPIO_PIN_LED_NUM) {
        printf("%s %d, error pin:%d", __func__, __LINE__, pin);
        return;
    }

    hdlPointer = &g_oemGpioIrqHandler[pin];
    if (g_gpioPinReflectionMap[hdlPointer->port] == pin) {
        if (hdlPointer->func) {
            hdlPointer->func(hdlPointer->port, hdlPointer->arg);
        } else {
            printf("%s %d, error hdlPointer->func == NULL", __func__, __LINE__);
        }
    } else {
        printf("%s %d, error pin:%d, hdlPointer->port:%d", __func__, __LINE__, pin, hdlPointer->port);
    }
}

/*dispatch*/
int32_t GpioDispatch(struct HdfDeviceIoClient *client, int cmdId, struct HdfSBuf *data, struct HdfSBuf *reply)
{
    int32_t result = HDF_FAILURE;

    if (client == NULL || client->device == NULL) {
        printf("%s: client or client->device is NULL", __func__);
        return result;
    }

    return result;
}

/* HdfDriverEntry method definitions */
static int32_t GpioDriverBind(struct HdfDeviceObject *device);
static int32_t GpioDriverInit(struct HdfDeviceObject *device);
static void GpioDriverRelease(struct HdfDeviceObject *device);

/* HdfDriverEntry definitions */
struct HdfDriverEntry g_GpioDriverEntry = {
    .moduleVersion = 1,
    .moduleName = "HDF_PLATFORM_GPIO",
    .Bind = GpioDriverBind,
    .Init = GpioDriverInit,
    .Release = GpioDriverRelease,
};
HDF_INIT(g_GpioDriverEntry);

/* GpioMethod method definitions */
static int32_t GpioDevWrite(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t val);
static int32_t GpioDevRead(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t *val);
static int32_t GpioDevSetDir(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t dir);
static int32_t GpioDevGetDir(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t *dir);
static int32_t GpioDevSetIrq(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t mode, GpioIrqFunc func, void *arg);
static int32_t GpioDevUnSetIrq(struct GpioCntlr *cntlr, uint16_t gpio);
static int32_t GpioDevEnableIrq(struct GpioCntlr *cntlr, uint16_t gpio);
static int32_t GpioDevDisableIrq(struct GpioCntlr *cntlr, uint16_t gpio);
/* GpioMethod definitions */
struct GpioMethod g_GpioCntlrMethod = {
    .request = NULL,
    .release = NULL,
    .write = GpioDevWrite,
    .read = GpioDevRead,
    .setDir = GpioDevSetDir,
    .getDir = GpioDevGetDir,
    .toIrq = NULL,
    .setIrq = GpioDevSetIrq,
    .unsetIrq = GpioDevUnSetIrq,
    .enableIrq = GpioDevEnableIrq,
    .disableIrq = GpioDevDisableIrq,
};

int InitGpioDevice(struct GpioDevice *device)
{
    struct HAL_IOMUX_PIN_FUNCTION_MAP gpio_cfg;

    gpio_cfg.pin = device->port;
    gpio_cfg.function = HAL_IOMUX_FUNC_AS_GPIO;
    gpio_cfg.volt = HAL_IOMUX_PIN_VOLTAGE_VIO;

    if ((device->config == OUTPUT_PUSH_PULL) || (device->config == OUTPUT_OPEN_DRAIN_PULL_UP) || (device->config == INPUT_PULL_UP) || (device->config == IRQ_MODE)) {
        gpio_cfg.pull_sel = HAL_IOMUX_PIN_PULLUP_ENABLE;
    } else if ((device->config == INPUT_PULL_DOWN)) {
        gpio_cfg.pull_sel = HAL_IOMUX_PIN_PULLDOWN_ENABLE;
    } else {
        gpio_cfg.pull_sel = HAL_IOMUX_PIN_NOPULL;
    }

    hal_iomux_init(&gpio_cfg, 1);

    return HDF_SUCCESS;
}

static uint32_t GetGpioDeviceResource(
    struct GpioDevice *device, const struct DeviceResourceNode *resourceNode)
{
    struct GpioResource *resource = &device->resource;
    struct DeviceResourceIface *dri = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);

    if (dri == NULL || dri->GetUint32 == NULL) {
        printf("DeviceResourceIface is invalid");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "pinNum", &resource->pinNum, 0) != HDF_SUCCESS) {
        printf("gpio config read pinNum fail");
        return HDF_FAILURE;
    }

    for (size_t i = 0; i < resource->pinNum; i++) {
        if (dri->GetUint32ArrayElem(resourceNode, "pin", i, &resource->pin, 0) != HDF_SUCCESS) {
            printf("gpio config read pin fail");
            return HDF_FAILURE;
        }

        if (dri->GetUint32ArrayElem(resourceNode, "realPin", i, &resource->realPin, 0) != HDF_SUCCESS) {
            printf("gpio config read realPin fail");
            return HDF_FAILURE;
        }

        if (dri->GetUint32ArrayElem(resourceNode, "config", i, &resource->config, 0) != HDF_SUCCESS) {
            printf("gpio config read config fail");
            return HDF_FAILURE;
        }

        uint32_t rel_pin = resource->realPin / DECIMALNUM * OCTALNUM + resource->realPin % DECIMALNUM;
        g_gpioPinReflectionMap[resource->pin] = rel_pin;
        device->config = resource->config;
        resource->pin = rel_pin;
        device->port = rel_pin;

        int32_t ret = InitGpioDevice(device);
        if (ret != HDF_SUCCESS) {
            printf("InitGpioDevice FAIL\r\n");
            return HDF_FAILURE;
        }
    }

    return HDF_SUCCESS;
}

static int32_t AttachGpioDevice(struct GpioCntlr *gpioCntlr, struct HdfDeviceObject *device)
{
    int32_t ret;

    struct GpioDevice *gpioDevice = NULL;
    if (device->property == NULL) {
        printf("%s: property is NULL", __func__);
        return HDF_FAILURE;
    }

    gpioDevice = (struct GpioDevice *)malloc(sizeof(struct GpioDevice));
    if (gpioDevice == NULL) {
        printf("%s: malloc gpioDevice error", __func__);
        return HDF_ERR_MALLOC_FAIL;
    }

    ret = GetGpioDeviceResource(gpioDevice, device->property);
    if (ret != HDF_SUCCESS) {
        (void)free(gpioDevice);
        return HDF_FAILURE;
    }

    gpioCntlr->count = gpioDevice->resource.pinNum;

    return HDF_SUCCESS;
}

static int32_t GpioDriverInit(struct HdfDeviceObject *device)
{
    int32_t ret;
    struct GpioCntlr *gpioCntlr = NULL;

    if (device == NULL) {
        printf("%s: device is NULL", __func__);
        return HDF_ERR_INVALID_OBJECT;
    }

    gpioCntlr = GpioCntlrFromDevice(device);
    if (gpioCntlr == NULL) {
        printf("GpioCntlrFromDevice fail\r\n");
        return HDF_FAILURE;
    }

    ret = AttachGpioDevice(gpioCntlr, device); // GpioCntlr add GpioDevice to priv
    if (ret != HDF_SUCCESS) {
        printf("AttachGpioDevice fail\r\n");
        return HDF_FAILURE;
    }

    gpioCntlr->ops = &g_GpioCntlrMethod; // register callback
    ret = GpioCntlrAdd(gpioCntlr);

    if (ret != HDF_SUCCESS) {
        printf("GpioCntlrAdd fail %d\r\n", gpioCntlr->start);
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

static int32_t GpioDriverBind(struct HdfDeviceObject *device)
{
    if (device == NULL) {
        printf("Sample device object is null!");
        return -1;
    }

    static struct GpioCntlr gpioCntlr;
    gpioCntlr.device = device;
    device->service = &(gpioCntlr.service);

    return HDF_SUCCESS;
}

static void GpioDriverRelease(struct HdfDeviceObject *device)
{
    struct GpioCntlr *gpioCntlr = NULL;

    if (device == NULL) {
        printf("%s: device is NULL", __func__);
        return;
    }

    gpioCntlr = GpioCntlrFromDevice(device);
    if (gpioCntlr == NULL) {
        printf("%s: host is NULL", __func__);
        return;
    }

    gpioCntlr->ops = NULL;

    free(gpioCntlr);
}

/*dev api*/
static int32_t GpioDevWrite(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t val)
{
    uint16_t hal_gpio = g_gpioPinReflectionMap[gpio];

    if ((enum HAL_GPIO_PIN_T)hal_gpio >= HAL_GPIO_PIN_LED_NUM) {
        printf("%s %d, error pin:%d", __func__, __LINE__, hal_gpio);
        return -1;
    }

    hal_gpio_pin_set_dir((enum HAL_GPIO_PIN_T)hal_gpio, HAL_GPIO_DIR_OUT, val);

    return HDF_SUCCESS;
}

static int32_t GpioDevRead(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t *val)
{
    uint16_t value;
    uint16_t hal_gpio = g_gpioPinReflectionMap[gpio];

    if ((enum HAL_GPIO_PIN_T)hal_gpio >= HAL_GPIO_PIN_LED_NUM) {
        printf("%s %d, error pin:%d", __func__, __LINE__, hal_gpio);
        return -1;
    }

    value = (uint16_t)hal_gpio_pin_get_val((enum HAL_GPIO_PIN_T)hal_gpio);
    *val = value;

    return HDF_SUCCESS;
}

static int32_t GpioDevSetDir(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t dir)
{
    uint16_t hal_gpio = g_gpioPinReflectionMap[gpio];
    if ((enum HAL_GPIO_PIN_T)hal_gpio >= HAL_GPIO_PIN_LED_NUM) {
        printf("%s %d, error pin:%d", __func__, __LINE__, hal_gpio);
        return -1;
    }

    hal_gpio_pin_set_dir((enum HAL_GPIO_PIN_T)hal_gpio, (enum HAL_GPIO_DIR_T)dir, 0);

    return HDF_SUCCESS;
}

static int32_t GpioDevGetDir(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t *dir)
{
    uint16_t value;
    uint16_t hal_gpio = g_gpioPinReflectionMap[gpio];

    if ((enum HAL_GPIO_PIN_T)hal_gpio >= HAL_GPIO_PIN_LED_NUM) {
        printf("%s %d, error pin:%d", __func__, __LINE__, gpio);
        return -1;
    }

    value = (uint16_t)hal_gpio_pin_get_dir((enum HAL_GPIO_PIN_T)hal_gpio);
    *dir = value;

    return HDF_SUCCESS;
}

static int32_t GpioDevSetIrq(struct GpioCntlr *cntlr, uint16_t gpio, uint16_t mode, GpioIrqFunc func, void *arg)
{
    enum HAL_GPIO_PIN_T pin = (enum HAL_GPIO_PIN_T)g_gpioPinReflectionMap[gpio];

    if (pin >= HAL_GPIO_PIN_LED_NUM) {
        printf("%s %d, error pin:%d", __func__, __LINE__, pin);
        return -1;
    }

    if ((mode == OSAL_IRQF_TRIGGER_RISING) || (mode == OSAL_IRQF_TRIGGER_FALLING)) {
        gpio_irq_cfg[pin].irq_type = HAL_GPIO_IRQ_TYPE_EDGE_SENSITIVE;
    } else if ((mode == OSAL_IRQF_TRIGGER_HIGH) || (mode == OSAL_IRQF_TRIGGER_LOW)) {
        gpio_irq_cfg[pin].irq_type = HAL_GPIO_IRQ_TYPE_LEVEL_SENSITIVE;
    } else {
        printf("%s %d, error mode:%d", __func__, __LINE__, mode);
        return -1;
    }

    g_oemGpioIrqHandler[pin].port = gpio;
    g_oemGpioIrqHandler[pin].func = func;
    g_oemGpioIrqHandler[pin].arg = arg;

    gpio_irq_cfg[pin].irq_polarity = mode;

    return HDF_SUCCESS;
}

static int32_t GpioDevUnSetIrq(struct GpioCntlr *cntlr, uint16_t gpio)
{
    enum HAL_GPIO_PIN_T pin = (enum HAL_GPIO_PIN_T)g_gpioPinReflectionMap[gpio];

    if (pin >= HAL_GPIO_PIN_LED_NUM) {
        printf("%s %d, error pin:%d", __func__, __LINE__, pin);
        return -1;
    }

    g_oemGpioIrqHandler[pin].func = NULL;
    g_oemGpioIrqHandler[pin].arg = NULL;

    return HDF_SUCCESS;
}

static int32_t GpioDevEnableIrq(struct GpioCntlr *cntlr, uint16_t gpio)
{
    struct HAL_GPIO_IRQ_CFG_T gpiocfg;
    uint16_t hal_gpio = (enum HAL_GPIO_PIN_T)g_gpioPinReflectionMap[gpio];

    if ((enum HAL_GPIO_PIN_T)hal_gpio >= HAL_GPIO_PIN_LED_NUM) {
        printf("%s %d, error pin:%d", __func__, __LINE__, (enum HAL_GPIO_PIN_T)hal_gpio);
        return -1;
    }

    hal_gpio_pin_set_dir((enum HAL_GPIO_PIN_T)hal_gpio, HAL_GPIO_DIR_IN, 0);

    gpiocfg.irq_enable = true;
    gpiocfg.irq_debounce = true;
    gpiocfg.irq_polarity = gpio_irq_cfg[(enum HAL_GPIO_PIN_T)hal_gpio].irq_polarity;
    gpiocfg.irq_handler = OemGpioIrqHdl;
    gpiocfg.irq_type = gpio_irq_cfg[(enum HAL_GPIO_PIN_T)hal_gpio].irq_type;
    gpio_irq_cfg[hal_gpio] = gpiocfg;

    hal_gpio_setup_irq((enum HAL_GPIO_PIN_T)hal_gpio, &gpiocfg);

    return HDF_SUCCESS;
}

static int32_t GpioDevDisableIrq(struct GpioCntlr *cntlr, uint16_t gpio)
{
    uint16_t hal_gpio = (enum HAL_GPIO_PIN_T)g_gpioPinReflectionMap[gpio];

    if ((enum HAL_GPIO_PIN_T)hal_gpio >= HAL_GPIO_PIN_LED_NUM) {
        printf("%s %d, error pin:%d", __func__, __LINE__, hal_gpio);
        return -1;
    }

    const struct HAL_GPIO_IRQ_CFG_T gpiocfg = {
        .irq_enable = false,
        .irq_debounce = false,
        .irq_polarity = HAL_GPIO_IRQ_POLARITY_LOW_FALLING,
        .irq_handler = NULL,
        .irq_type = HAL_GPIO_IRQ_TYPE_EDGE_SENSITIVE,
    };

    hal_gpio_setup_irq((enum HAL_GPIO_PIN_T)hal_gpio, &gpiocfg);

    return HDF_SUCCESS;
}
