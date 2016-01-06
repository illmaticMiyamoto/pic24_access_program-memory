#include <stdio.h>
#include "delay.h"
#include "config.h"
#include "set_uart_2400.h"
#include "access_program_memory.h"

/******************************************************************************
 * 
 *
 *
 *****************************************************************************/

void SYSTEM_Initialize (void);


int main(void)
{
    /*init configulation bit*/
    config_init();
    /*INIT SYSTEM*/
    SYSTEM_Initialize();
    /*init UART*/
    uart_init_2400();

    delay_ms(2000);
    
    printf("test_hello\r\n");



    Flash_Init();
    
    while(1){
    }
}


void SYSTEM_Initialize (void)
{
    TRISBbits.TRISB2 = 1;   //P6: set to Input  for UART Rx
    TRISBbits.TRISB3 = 0;   //P7: set to Output for UART Tx
}


