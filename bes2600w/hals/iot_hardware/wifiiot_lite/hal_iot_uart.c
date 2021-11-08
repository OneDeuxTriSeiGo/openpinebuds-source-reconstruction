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

#include "iot_errno.h"
#include "iot_uart.h"

unsigned int IoTUartInit(unsigned int id, const IotUartAttribute *param)
{
    return IOT_SUCCESS;
}

int IoTUartRead(unsigned int id, unsigned char *data, unsigned int dataLen)
{
    return dataLen;
}

int IoTUartWrite(unsigned int id, const unsigned char *data, unsigned int dataLen)
{
    return dataLen;
}

unsigned int IoTUartDeinit(unsigned int id)
{
    return IOT_SUCCESS;
}

unsigned int IoTUartSetFlowCtrl(unsigned int id, IotFlowCtrl flowCtrl)
{
    return IOT_SUCCESS;
}
