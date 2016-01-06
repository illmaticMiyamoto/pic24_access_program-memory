#include <stdio.h>
#include "delay.h"
#include "config.h"
#include "set_uart_2400.h"
#include "access_program_memory.h"

/******************************************************************************
 * samlpe code of accessing program memory
 *  -write data to program memory.
 *  -read same address and check the data.
 *****************************************************************************/

void SYSTEM_Initialize (void);


int main(void)
{
    config_init();          //initialize config
    SYSTEM_Initialize();    //initialize system
    uart_init_2400();       //initialize uart


    /*Initialize program memory. */
    Flash_Init();

    /*Write the test data */
    unsigned int data = 0xABCD;
    unsigned int address = 0;
    Write_Memory(address, data);

    /*Read the data from memory*/
    data =Read_Memory(address);
    
    printf("data:%x\r\n",data);        //pit put 0xABCD

    while(1);
}

void SYSTEM_Initialize (void)
{
    TRISBbits.TRISB2 = 1;   //P6: set to Input  for UART Rx
    TRISBbits.TRISB3 = 0;   //P7: set to Output for UART Tx
}


