/*
 * mpu6050.c
 *
 *  Created on: Aug 2, 2017
 *      Author: tuan
 */

#include "../sub_include.h"
#include "mpu6050.h"

extern void i2c_config(void){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_I2C1);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

    GPIOPinConfigure(GPIO_PA6_I2C1SCL);
    GPIOPinConfigure(GPIO_PA7_I2C1SDA);

    GPIOPinTypeI2CSCL(GPIO_PORTA_BASE, GPIO_PIN_6);
    GPIOPinTypeI2C(GPIO_PORTA_BASE, GPIO_PIN_7);
    I2CMasterInitExpClk(I2C1_BASE, SysCtlClockGet(), true);


}

static void I2C_readbuffer(uint8_t regAddress, uint8_t *pData, uint8_t length)
{
    I2CMasterSlaveAddrSet(I2C1_BASE, ADDRESS_DEVICE, false);
    I2CMasterDataPut(I2C1_BASE, regAddress);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterSlaveAddrSet(I2C1_BASE, ADDRESS_DEVICE, true);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_START);
    while(I2CMasterBusy(I2C1_BASE));
    pData[0]=I2CMasterDataGet(I2C1_BASE);

    uint8_t index=1;
    for(index=1;index<length-1;index++)
    {
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_CONT);
        while(I2CMasterBusy(I2C1_BASE));
        pData[index]=I2CMasterDataGet(I2C1_BASE);
    }

    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_RECEIVE_FINISH);
    while(I2CMasterBusy(I2C1_BASE));
    pData[index]=I2CMasterDataGet(I2C1_BASE);
}


static void I2c_Read_Char(uint8_t regAddress, uint8_t *pData)
{
    I2CMasterSlaveAddrSet(I2C1_BASE, ADDRESS_DEVICE, false);
    I2CMasterDataPut(I2C1_BASE, regAddress);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_SEND);
    while(I2CMasterBusy(I2C1_BASE));

    I2CMasterSlaveAddrSet(I2C1_BASE, ADDRESS_DEVICE, true);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_SINGLE_RECEIVE);
    while(I2CMasterBusy(I2C1_BASE));
    *pData=I2CMasterDataGet(I2C1_BASE);
}

static void I2c_Write_Buffer(uint8_t regAddress, uint8_t *pData, uint8_t length)
{
    I2CMasterSlaveAddrSet(I2C1_BASE, ADDRESS_DEVICE, false);
    I2CMasterDataPut(I2C1_BASE, regAddress);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_START);
    while(I2CMasterBusy(I2C1_BASE));

    uint8_t index=0;
    for(index=0;index<length-1;index++)
    {
        I2CMasterDataPut(I2C1_BASE, pData[index]);
        I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_CONT);
        while(I2CMasterBusy(I2C1_BASE));
    }

    I2CMasterDataPut(I2C1_BASE, pData[length-1]);
    I2CMasterControl(I2C1_BASE, I2C_MASTER_CMD_BURST_SEND_FINISH);
    while(I2CMasterBusy(I2C1_BASE));
}



