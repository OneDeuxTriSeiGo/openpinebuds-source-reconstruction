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
#include "pwm.h"
#include <stdlib.h>
#include <stdio.h>
#include "device_resource_if.h"
#include "hdf_device_desc.h"
#include "hal_trace.h"
#include "hal/hal_timer.h"
#include "hal_iomux.h"
#include "hal_iomux_best2003.h"
#include "pwm_core.h"

#define PWM_MAX_FUNCTION 8
#define UNTIL_NAN0SECONDS 100000000
#define PERCENT 100
#define DEC_TEN 10
#define PIN_GROUP_NUM 8

static uint32_t g_pwmFunction[PWM_MAX_FUNCTION] = {
    HAL_IOMUX_FUNC_PWM0,
    HAL_IOMUX_FUNC_PWM1,
    HAL_IOMUX_FUNC_PWM2,
    HAL_IOMUX_FUNC_PWM3,
    HAL_IOMUX_FUNC_PWM4,
    HAL_IOMUX_FUNC_PWM5,
    HAL_IOMUX_FUNC_PWM6,
    HAL_IOMUX_FUNC_PWM7,
};

static int32_t PwmDevSetConfig(struct PwmDev *pwm, struct PwmConfig *config);
static int32_t PwmDevOpen(struct PwmDev *pwm);
static int32_t PwmDevClose(struct PwmDev *pwm);

struct PwmMethod g_pwmmethod = {
    .setConfig = PwmDevSetConfig,
    .open = PwmDevOpen,
    .close = PwmDevClose,
};

static int InitPwmDevice(struct PwmDev *host)
{
    if (host == NULL) {
        printf("%s: invaild parameter\r\n", __func__);
        return HDF_ERR_INVALID_PARAM;
    }

    struct PwmDevice *pwmDevice;
    pwmDevice = (struct PwmDevice *)host->priv;

    if (pwmDevice == NULL) {
        printf("pwmDevice is NULL\r\n");
        return HDF_FAILURE;
    }

    struct PwmResource *resource = &pwmDevice->resource;
    if (resource == NULL) {
        printf("resource is NULL\r\n");
        return HDF_FAILURE;
    }

    struct HAL_IOMUX_PIN_FUNCTION_MAP pinmuxPwm[] = {
        {0, 0, HAL_IOMUX_PIN_VOLTAGE_VIO, HAL_IOMUX_PIN_NOPULL},
    };

    pinmuxPwm->pin = resource->pwm_pin;
    pinmuxPwm->function = g_pwmFunction[resource->pwm_id];

    hal_iomux_init(pinmuxPwm, ARRAY_SIZE(pinmuxPwm));

    return HDF_SUCCESS;
}

static uint32_t GetPwmDeviceResource(
    struct PwmDevice *device, const struct DeviceResourceNode *resourceNode)
{
    uint32_t tempPin = 0;

    if (device == NULL || resourceNode == NULL) {
        printf("resource or device is NULL\r\n");
        return HDF_FAILURE;
    }

    struct PwmResource *resource = &device->resource;
    struct DeviceResourceIface *dri = NULL;

    dri = DeviceResourceGetIfaceInstance(HDF_CONFIG_SOURCE);

    if (dri == NULL || dri->GetUint32 == NULL) {
        printf("DeviceResourceIface is invalid\r\n");
        return HDF_FAILURE;
    }

    if (dri->GetUint32(resourceNode, "pwm_pin", &tempPin, 0) != HDF_SUCCESS) {
        printf("read pwm_pin fail\r\n");
        return HDF_FAILURE;
    }
    resource->pwm_pin = ((tempPin / DEC_TEN) * PIN_GROUP_NUM) + (tempPin % DEC_TEN);

    if (dri->GetUint32(resourceNode, "pwm_id", &resource->pwm_id, 0) != HDF_SUCCESS) {
        printf("read pwm_id fail\r\n");
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

static int32_t AttachPwmDevice(struct PwmDev *host, struct HdfDeviceObject *device)
{
    int32_t ret;

    struct PwmDevice *pwmDevice = NULL;
    if (device->property == NULL || host == NULL || device == NULL) {
        printf("%s: param is NULL\r\n", __func__);
        return HDF_FAILURE;
    }

    pwmDevice = (struct PwmDevice *)malloc(sizeof(struct PwmDevice));
    if (pwmDevice == NULL) {
        printf("%s: malloc pwmDevice error\r\n", __func__);
        return HDF_ERR_MALLOC_FAIL;
    }

    ret = GetPwmDeviceResource(pwmDevice, device->property);
    if (ret != HDF_SUCCESS) {
        (void)free(pwmDevice);
        return HDF_FAILURE;
    }

    host->priv = pwmDevice;
    host->num = pwmDevice->resource.pwm_id;

    return InitPwmDevice(host);
}
/* HdfDriverEntry method definitions */
static int32_t PwmDriverBind(struct HdfDeviceObject *device);
static int32_t PwmDriverInit(struct HdfDeviceObject *device);
static void PwmDriverRelease(struct HdfDeviceObject *device);

/* HdfDriverEntry definitions */
struct HdfDriverEntry g_pwmDriverEntry = {
    .moduleVersion = 1,
    .moduleName = "HDF_PLATFORM_PWM",
    .Bind = PwmDriverBind,
    .Init = PwmDriverInit,
    .Release = PwmDriverRelease,
};

// Initialize HdfDriverEntry
HDF_INIT(g_pwmDriverEntry);

static int32_t PwmDriverBind(struct HdfDeviceObject *device)
{
    static struct PwmDev devService;

    if (device == NULL) {
        printf("hdfDevice object is null!\r\n");
        return HDF_FAILURE;
    }

    device->service = &devService.service;
    devService.device = device;
    printf("Enter %s\r\n", __func__);

    return HDF_SUCCESS;
}

static int32_t PwmDriverInit(struct HdfDeviceObject *device)
{
    int32_t ret;
    struct PwmDev *host = NULL;

    if (device == NULL) {
        printf("%s: device is NULL\r\n", __func__);
        return HDF_ERR_INVALID_OBJECT;
    }

    printf("Enter %s:\r\n", __func__);

    host = (struct PwmDev *)malloc(sizeof(struct PwmDev));
    if (host == NULL) {
        printf("%s: host is NULL\r\n", __func__);
        return HDF_FAILURE;
    }

    ret = AttachPwmDevice(host, device);
    if (ret != HDF_SUCCESS) {
        free(host);
        printf("%s:attach error\r\n", __func__);
        return HDF_FAILURE;
    }

    host->method = &g_pwmmethod;
    ret = PwmDeviceAdd(device, host);
    if (ret != HDF_SUCCESS) {
        PwmDeviceRemove(device, host);
        free(host);
        return HDF_FAILURE;
    }

    printf("PwmDriverInit success!\r\n");
    return HDF_SUCCESS;
}

static void PwmDriverRelease(struct HdfDeviceObject *device)
{
    struct PwmDev *host = NULL;

    if (device->service == NULL || device == NULL) {
        printf("device is null\r\n");
        return;
    }

    host = (struct PwmDev *)device->service;
    if (host != NULL) {
        host->method = NULL;
        host->device = NULL;
        free(host);
        host = NULL;
    }

    device->service = NULL;

    printf("PwmDriverRelease finish!!\r\n");

    return;
}

static int32_t PwmDevSetConfig(struct PwmDev *pwm, struct PwmConfig *config)
{
    struct PwmDevice *prvPwm = NULL;
    struct HAL_PWM_CFG_T *pwmCfg = NULL;
    enum HAL_PWM_ID_T pwmId;

    if (pwm == NULL || config == NULL || (config->period > UNTIL_NAN0SECONDS)) {
        printf("%s\r\n", __FUNCTION__);
        return HDF_FAILURE;
    }

    prvPwm = (struct PwmDevice *)PwmGetPriv(pwm);
    if (prvPwm == NULL) {
        return HDF_FAILURE;
    }
    pwmCfg = &prvPwm->pwm_cfg;
    pwmCfg->freq = UNTIL_NAN0SECONDS / config->period;
    pwmCfg->ratio = config->duty * PERCENT / config->period;
    pwmCfg->inv = false;
    pwmCfg->sleep_on = false;

    pwmId = prvPwm->resource.pwm_id;

    if (config->status == PWM_ENABLE_STATUS) {
        hal_pwm_enable(pwmId, pwmCfg);
    } else {
        hal_pwm_disable(pwmId);
    }

    return HDF_SUCCESS;
}

static int32_t PwmDevOpen(struct PwmDev *pwm)
{
    if (pwm == NULL) {
        printf("%s\r\n", __FUNCTION__);
        return HDF_FAILURE;
    }

    return HDF_SUCCESS;
}

static int32_t PwmDevClose(struct PwmDev *pwm)
{
    struct PwmDevice *prvPwm = NULL;
    enum HAL_PWM_ID_T pwmId = 0;

    if (pwm == NULL) {
        printf("%s\r\n", __FUNCTION__);
        return HDF_FAILURE;
    }

    prvPwm = (struct PwmDevice *)PwmGetPriv(pwm);
    if (prvPwm == NULL) {
        printf("%s\r\n", __FUNCTION__);
        return HDF_FAILURE;
    }

    pwmId = prvPwm->resource.pwm_id;

    hal_pwm_disable(pwmId);

    return HDF_SUCCESS;
}
