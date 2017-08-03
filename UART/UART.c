/*
 * UART.c
 *
 *  Created on: Aug 2, 2017
 *      Author: tuan
 */
#include "../sub_include.h"
#include "UART.h"

static void UART_ISR(void)
{
    UARTIntClear(UART0_BASE,UARTIntStatus(UART0_BASE,true));
    int32_t k;
    k=UARTCharGet(UART0_BASE);
    UARTCharPut(UART0_BASE, k);
}

extern void config_UART(void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinConfigure(GPIO_PA0_U0RX);
    GPIOPinConfigure(GPIO_PA1_U0TX);
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    UARTClockSourceSet(UART0_BASE, UART_CLOCK_SYSTEM);

    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE |UART_CONFIG_PAR_NONE);
    UARTIntRegister(UART0_BASE,&UART_ISR);
    IntEnable(INT_UART0);
    UARTIntEnable(UART0_BASE, UART_INT_RX | UART_INT_RT);

}



