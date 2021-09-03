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
#include "platform.h"
#include "spi_if.h"

/* W25QXX SPI Flash */
#define SPI_FLASH_PageSize 256
#define SPI_FLASH_PerWritePageSize 256
#define W25X_WriteEnable 0x06
#define W25X_WriteDisable 0x04
#define W25X_ReadStatusReg 0x05
#define W25X_WriteStatusReg 0x01
#define W25X_ReadData 0x03
#define W25X_FastReadData 0x0B
#define W25X_FastReadDual 0x3B
#define W25X_PageProgram 0x02
#define W25X_BlockErase 0xD8
#define W25X_SectorErase 0x20
#define W25X_ChipErase 0xC7
#define W25X_PowerDown 0xB9
#define W25X_ReleasePowerDown 0xAB
#define W25X_DeviceID 0xAB
#define W25X_ManufactureDeviceID 0x90
#define W25X_JedecDeviceID 0x9F
#define WIP_Flag 0x01 /* Write In Progress (WIP) flag */
#define Dummy_Byte 0xFF

#define FLASH_WriteAddress 0x00000
#define FLASH_ReadAddress FLASH_WriteAddress
#define FLASH_SectorToErase FLASH_WriteAddress

static int spi_transfer(DevHandle dev, uint8_t *txbuf, uint8_t *rxbuf, uint32_t len)
{
    struct SpiMsg msg = {
        .wbuf = txbuf,
        .rbuf = rxbuf,
        .len = len,
        .delayUs = 10,
    };
    return SpiTransfer(dev, &msg, 1);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WriteEnable
* Description    : Enables the write access to the FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WriteEnable(DevHandle dev)
{
    uint8_t cmd = W25X_WriteEnable;
    uint8_t data;
    spi_transfer(dev, &cmd, &data, 1);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_WaitForWriteEnd
* Description    : Polls the status of the Write In Progress (WIP) flag in the
*                  FLASH's status  register  and  loop  until write  opertaion
*                  has completed.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_WaitForWriteEnd(DevHandle dev)
{
    uint8_t cmd[2] = {W25X_ReadStatusReg, Dummy_Byte};
    uint8_t data[2] = {0};
    do {
        spi_transfer(dev, cmd, data, 2);
    } while ((data[1] & WIP_Flag) == 1); /* Write in progress */
}

void SPI_Flash_PowerDown(DevHandle dev)
{
    uint8_t cmd = W25X_PowerDown;
    uint8_t data = 0;
    spi_transfer(dev, &cmd, &data, 1);
}

void SPI_Flash_WAKEUP(DevHandle dev)
{
    uint8_t cmd = W25X_ReleasePowerDown;
    uint8_t data = 0;
    spi_transfer(dev, &cmd, &data, 1);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_SectorErase
* Description    : Erases the specified FLASH sector.
* Input          : SectorAddr: address of the sector to erase.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_SectorErase(DevHandle dev, uint32_t SectorAddr)
{
    /* Send write enable instruction */
    SPI_FLASH_WriteEnable(dev);
    SPI_FLASH_WaitForWriteEnd(dev);
    /* Sector Erase */
    uint8_t cmd[4] = {W25X_SectorErase, (SectorAddr & 0xFF0000) >> 16,
                      (SectorAddr & 0xFF00) >> 8, SectorAddr & 0xFF};
    uint8_t data[4];
    spi_transfer(dev, cmd, data, sizeof(cmd));
    /* Wait the end of Flash writing */
    SPI_FLASH_WaitForWriteEnd(dev);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BulkErase
* Description    : Erases the entire FLASH.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BulkErase(DevHandle dev)
{
    /* Send write enable instruction */
    SPI_FLASH_WriteEnable(dev);
    /* Bulk Erase */
    uint8_t cmd = W25X_ChipErase;
    uint8_t data = 0;
    spi_transfer(dev, &cmd, &data, 1);
    /* Wait the end of Flash writing */
    SPI_FLASH_WaitForWriteEnd(dev);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_PageWrite
* Description    : Writes more than one byte to the FLASH with a single WRITE
*                  cycle(Page WRITE sequence). The number of byte can't exceed
*                  the FLASH page size.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH,
*                    must be equal or less than "SPI_FLASH_PageSize" value.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_PageWrite(DevHandle dev, const uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    /* Enable the write access to the FLASH */
    SPI_FLASH_WriteEnable(dev);
    if (NumByteToWrite > SPI_FLASH_PerWritePageSize) {
        NumByteToWrite = SPI_FLASH_PerWritePageSize;
    }
    uint8_t cmd[NumByteToWrite + 4];
    uint8_t data[NumByteToWrite + 4];
    cmd[0] = W25X_PageProgram;
    cmd[1] = (WriteAddr & 0xFF0000) >> 16;
    cmd[2] = (WriteAddr & 0xFF00) >> 8;
    cmd[3] = WriteAddr & 0xFF;
    memcpy(&cmd[4], pBuffer, NumByteToWrite);
    spi_transfer(dev, cmd, data, NumByteToWrite + 4);
    /* Wait the end of Flash writing */
    SPI_FLASH_WaitForWriteEnd(dev);
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferWrite
* Description    : Writes block of data to the FLASH. In this function, the
*                  number of WRITE cycles are reduced, using Page WRITE sequence.
* Input          : - pBuffer : pointer to the buffer  containing the data to be
*                    written to the FLASH.
*                  - WriteAddr : FLASH's internal address to write to.
*                  - NumByteToWrite : number of bytes to write to the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferWrite(DevHandle dev, const uint8_t *pBuffer, uint32_t WriteAddr, uint16_t NumByteToWrite)
{
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0, temp = 0;

    Addr = WriteAddr % SPI_FLASH_PageSize;
    count = SPI_FLASH_PageSize - Addr;
    NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
    NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

    if (Addr == 0) /* WriteAddr is SPI_FLASH_PageSize aligned  */
    {
        if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
        {
            SPI_FLASH_PageWrite(dev, pBuffer, WriteAddr, NumByteToWrite);
        } else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            while (NumOfPage--) {
                SPI_FLASH_PageWrite(dev, pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr += SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            SPI_FLASH_PageWrite(dev, pBuffer, WriteAddr, NumOfSingle);
        }
    } else /* WriteAddr is not SPI_FLASH_PageSize aligned  */
    {
        if (NumOfPage == 0) /* NumByteToWrite < SPI_FLASH_PageSize */
        {
            if (NumOfSingle > count) /* (NumByteToWrite + WriteAddr) > SPI_FLASH_PageSize */
            {
                temp = NumOfSingle - count;

                SPI_FLASH_PageWrite(dev, pBuffer, WriteAddr, count);
                WriteAddr += count;
                pBuffer += count;

                SPI_FLASH_PageWrite(dev, pBuffer, WriteAddr, temp);
            } else {
                SPI_FLASH_PageWrite(dev, pBuffer, WriteAddr, NumByteToWrite);
            }
        } else /* NumByteToWrite > SPI_FLASH_PageSize */
        {
            NumByteToWrite -= count;
            NumOfPage = NumByteToWrite / SPI_FLASH_PageSize;
            NumOfSingle = NumByteToWrite % SPI_FLASH_PageSize;

            SPI_FLASH_PageWrite(dev, pBuffer, WriteAddr, count);
            WriteAddr += count;
            pBuffer += count;

            while (NumOfPage--) {
                SPI_FLASH_PageWrite(dev, pBuffer, WriteAddr, SPI_FLASH_PageSize);
                WriteAddr += SPI_FLASH_PageSize;
                pBuffer += SPI_FLASH_PageSize;
            }

            if (NumOfSingle != 0) {
                SPI_FLASH_PageWrite(dev, pBuffer, WriteAddr, NumOfSingle);
            }
        }
    }
}

/*******************************************************************************
* Function Name  : SPI_FLASH_BufferRead
* Description    : Reads a block of data from the FLASH.
* Input          : - pBuffer : pointer to the buffer that receives the data read
*                    from the FLASH.
*                  - ReadAddr : FLASH's internal address to read from.
*                  - NumByteToRead : number of bytes to read from the FLASH.
* Output         : None
* Return         : None
*******************************************************************************/
void SPI_FLASH_BufferRead(DevHandle dev, uint8_t *pBuffer, uint32_t ReadAddr, uint16_t NumByteToRead)
{
    uint8_t cmd[NumByteToRead + 4];
    uint8_t data[NumByteToRead + 4];
    cmd[0] = W25X_ReadData;
    cmd[1] = (ReadAddr & 0xFF0000) >> 16;
    cmd[2] = (ReadAddr & 0xFF00) >> 8;
    cmd[3] = ReadAddr & 0xFF;
    spi_transfer(dev, cmd, data, NumByteToRead + 4);
    memcpy(pBuffer, &data[4], NumByteToRead);
}

uint32_t SPI_FLASH_ReadJedecID(DevHandle dev)
{
    uint8_t cmd[4] = {W25X_JedecDeviceID, Dummy_Byte, Dummy_Byte, Dummy_Byte};
    uint8_t data[4];
    spi_transfer(dev, cmd, data, 4);
    return (data[1] << 16) | (data[2] << 8) | data[3];
}

uint32_t SPI_FLASH_ReadDeviceID(DevHandle dev)
{
    uint8_t cmd[5] = {W25X_DeviceID, Dummy_Byte, Dummy_Byte, Dummy_Byte, Dummy_Byte};
    uint8_t data[5];
    spi_transfer(dev, cmd, data, 5);
    return data[4];
}

uint32_t SPI_FLASH_ReadManufactureID(DevHandle dev)
{
    uint8_t cmd[6] = {W25X_ManufactureDeviceID, 0, 0, 0, 0, 0};
    uint8_t data[6];
    spi_transfer(dev, cmd, data, 6);
    return (uint32_t)(data[4] << 8) | data[5];
}

void spi_test(void)
{
    LOG_I("enter");
    struct SpiDevInfo info = {0, 0};
    DevHandle dev = SpiOpen(&info);
    if (dev == NULL) {
        LOG_E("SpiOpen failed");
        return;
    }
    SPI_Flash_WAKEUP(dev);
    uint32_t id = SPI_FLASH_ReadManufactureID(dev);
    LOG_I("ManufactureID=0x%X", id);
    id = SPI_FLASH_ReadDeviceID(dev);
    LOG_I("DeviceID=0x%X", id);
    id = SPI_FLASH_ReadJedecID(dev);
    LOG_I("JedecID=0x%X", id);

#define BUFSIZE 300 // smaller or bigger than SPI_FLASH_PageSize
    uint8_t txbuf[BUFSIZE] = "This is w25qxx flash test.";
    uint8_t rxbuf[BUFSIZE];
    /* Erase SPI FLASH Sector to write on */
    SPI_FLASH_SectorErase(dev, FLASH_SectorToErase);
    LOG_I("SPI_FLASH_SectorErase");

    SPI_FLASH_BufferWrite(dev, txbuf, FLASH_WriteAddress, BUFSIZE);
    LOG_I("tx: %s", txbuf);

    SPI_FLASH_BufferRead(dev, rxbuf, FLASH_ReadAddress, BUFSIZE);
    LOG_I("rx: %s", rxbuf);

    if (memcmp(txbuf, rxbuf, BUFSIZE) == 0) {
        LOG_I("W25Qxx Test Passed!");
    } else {
        LOG_I("W25Qxx Test Failed!");
    }
    SPI_Flash_PowerDown(dev);
    SpiClose(dev);
    LOG_I("leave");
}
