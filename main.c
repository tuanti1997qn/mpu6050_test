

/**
 * main.c
 */
#include "sub_include.h"

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); //80Mhz
    config_UART();
    IntMasterEnable();
    while(1);
}
