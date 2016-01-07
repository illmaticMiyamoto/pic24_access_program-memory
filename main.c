#include <stdio.h>
#include <math.h>
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
unsigned int test_rom_access(void);


int main(void)
{
    config_init();          //initialize config
    SYSTEM_Initialize();    //initialize system
    uart_init_2400();       //initialize uart

    printf("\r\n\r\n\r\n\r\n");

    /*Initialize program memory. */
    Flash_Init();

    /*test*/
    printf("Error_flag:%d",test_rom_access());

    while(1);
}

void SYSTEM_Initialize (void)
{
    TRISBbits.TRISB2 = 1;   //P6: set to Input  for UART Rx
    TRISBbits.TRISB3 = 0;   //P7: set to Output for UART Tx
}


/**
 * 
 */
unsigned int test_rom_access(void){
    /*Write the test data */
    printf("start test\r\n");
    unsigned int data ;
    unsigned int address;
    unsigned int buf[0x600];
    unsigned char error_flag = 0;

    /*Test one registor read/write*/
    printf("test1\r\n");
    int i=0;
    for(i=0;i<0x600;i++)
    {
        address = i;
        data = rand();
        buf[i] = data;
        Write_Memory(address, data);
        if(Read_Memory(address)==data);
        else{
            printf("%04x:",i);
            printf("ERROR\r\n");
            error_flag = 1;
        }
    }
    if(error_flag) printf("Error\r\n");
    else printf("No Error\r\n");


    /*recheck all registor*/
    printf("test2\r\n");
    for(i=0;i<0x600;i++){
        address = i;
        if(Read_Memory(address)==buf[i]) ;
        else{
            printf("%04x:",i);
            printf("ERROR\r\n");
            error_flag = 1;
        }
    }
    if(error_flag) printf("Error\r\n");
    else printf("No Error\r\n");

    return error_flag;
}