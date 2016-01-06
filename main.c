#include <stdio.h>
#include "delay.h"
#include "config.h"
#include <string.h>
#include "set_uart_2400.h"
#include "access_program_memory.h"


#define TOUCH_CAP_THRESHOLD 380

#define VWC_THRESHOLD_LO    200
#define VWC_THRESHOLD_HIGH  300
#define CHARGE_CURRENT      16
#define CHARGE_TIME         16
#define AD_BITRATE          1024
#define AD_REF_VOLTAGE      3.3

#define ERASE_REGISTOR_SIZE 0x400
#define ERASE_BUFFER_SIZE   0x200
#define BLOCK_FLASH_SIZE    0x600   //one block reg size
#define TOTAL_FLASH_SIZE    0xC00   //total reg size (2block)
#define FLASH_BLOCK_ADDRESS 0x9000  //first address of the flash block

#define BUF_SIZE  50

#define DEBUG

#ifdef DEBUG
#define DEBUG_PRINT_STR(x) printf(x)
#define DEBUG_PRINT(fmt, ...) printf(fmt, __VA_ARGS__)
#else
#define DEBUG_PRINT_STR(x)
#define DEBUG_PRINT(fmt, ...)
#endif


const __attribute__((section("my_memory"), space (prog), address (FLASH_BLOCK_ADDRESS) )) unsigned int _flash_datas[96*8]; //test write
const __attribute__((section("my_memory"), space (prog), address (FLASH_BLOCK_ADDRESS+BLOCK_FLASH_SIZE) )) unsigned int _flash_datas2[96*8]; //test write

/******************************************************************************
 * Soil moisture sensor prototype
 *  connection
 *      pin2: touch sensor
 *      pin3: soil moisture sensto
 *      pin4: UART TX
 *      pin26: full color led
 *      pin25: full color led
 *      pin24: full color led
 *  operation
 *      1.Initialize CTMU,RTC,UART
 *      2.check touch switch, ON=>go to 3, ODD=>go to 1
 *      3.check VWC sensor, not enough:red, good:green, too much:blue
 *      4.Go to deep sleep
 *      5.wake after 0.5[s] => back to 1
 *
 *
 *****************************************************************************/

void SYSTEM_Initialize (void);
void get_command(char *buf);
void flash_set(void);
void Flash_Erase(unsigned int page, unsigned int offset);
void Flash_Write(unsigned int page, unsigned int offset, unsigned int val);
unsigned int Flash_Read(unsigned int page, unsigned int offset);
int Flash_Bulk_Write_With_Erase(unsigned int page, unsigned int offset, unsigned int val);
void Flash_Init(void);

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


    int i=0;
    unsigned int  val;

    unsigned int page   = __builtin_tblpage(_flash_datas);
    unsigned int offset = __builtin_tbloffset(_flash_datas);
    Flash_Init();
    
    while(1){
        
        i++;
        
        Flash_Erase(page, offset);
        int j;
        //for(j=0;j<64*8*2;j++){
          //  Flash_Write(page, offset+2*j,0xABCD);

        //}
        Flash_Write(page, offset,0xABCD);
        Flash_Write(page, offset+0x1FE,0xABCD);
        Flash_Write(page, offset+0x200,0xABCD);
        Flash_Write(page, offset+0x300,0xABCD);
        Flash_Write(page, offset+0x400,0xABCD);
        Flash_Write(page, offset+0x500,0xABCD);
        Flash_Write(page, offset+0x5FE,0xABCD);

        Flash_Bulk_Write_With_Erase(page, offset+0x408,0xABCD);

        for(j=0;j<64*8*2*10;j++){
            val = Flash_Read(page, offset+2*j);
            printf("%d address:0x%x,",j,offset+2*j);
            printf("data:0x%x\r\n",val);
        }
        delay_ms(1000);
    }
}

void Flash_Init(void)
{
    unsigned int page   = __builtin_tblpage(_flash_datas);
    unsigned int offset = __builtin_tbloffset(_flash_datas);
    Flash_Erase(page, offset);
    Flash_Erase(page, offset+0x400);
    Flash_Erase(page, offset+0x800);
}


int Flash_Bulk_Write_With_Erase(unsigned int page, unsigned int offset, unsigned int val) {
    unsigned int i=0;
    unsigned int buffer[ERASE_BUFFER_SIZE];
    unsigned char erase_block;
    
    //buffer[i + head] = values[i];
    if(offset%2==1)
    {
        //error
        return -1;
    }
    else if(offset<FLASH_BLOCK_ADDRESS)
    {
        //error
        return -1;
    }else if(offset<FLASH_BLOCK_ADDRESS+ERASE_REGISTOR_SIZE)
    {
        //erase 1st
        erase_block = 0;
    }else if(offset<FLASH_BLOCK_ADDRESS+2*ERASE_REGISTOR_SIZE)
    {
        //erase 2nd
        erase_block = 1;
    }else if(offset<FLASH_BLOCK_ADDRESS+3*ERASE_REGISTOR_SIZE)
    {
        //erase 3rd
        erase_block = 2;
    }else
    {
        //error
        return -1;
    };

    DEBUG_PRINT("block:%d\r\n",erase_block);

    //rescue to buffer
    TBLPAG = page;
    for (i=0; i< ERASE_BUFFER_SIZE; i++) {
        buffer[i] = Flash_Read(page, FLASH_BLOCK_ADDRESS+erase_block*ERASE_REGISTOR_SIZE + 2*i);
    }

    

    //write data to buffer
    buffer[(offset-FLASH_BLOCK_ADDRESS-erase_block*ERASE_REGISTOR_SIZE)/2] =val;

    for (i=0; i< ERASE_BUFFER_SIZE; i++)DEBUG_PRINT("buf:0x%x\r\n",buffer[i]) ;

    //erase flash rom
    Flash_Erase( page, FLASH_BLOCK_ADDRESS+erase_block*ERASE_REGISTOR_SIZE);

    //write to registor
    /*
    NVMCON = 0x4003;
    TBLPAG = page;
    for (i=0; i<ERASE_BUFFER_SIZE; i++) {
        __builtin_tblwtl(FLASH_BLOCK_ADDRESS+erase_block*ERASE_REGISTOR_SIZE + 2*i, buffer[i]);
        __builtin_tblwth(FLASH_BLOCK_ADDRESS+erase_block*ERASE_REGISTOR_SIZE + 2*i, 0xFF);
    }
    asm volatile ("disi #5");
    __builtin_write_NVM();
    while(NVMCONbits.WR);
    */
     for (i=0; i<ERASE_BUFFER_SIZE; i++)  Flash_Write(page,  FLASH_BLOCK_ADDRESS+erase_block*ERASE_REGISTOR_SIZE + 2*i, buffer[i]);

    return 0;
}


void Flash_Erase(unsigned int page, unsigned int offset)
{
    while(NVMCONbits.WR);
    TBLPAG = page;
    __builtin_tblwtl(offset, 0x0000);
    NVMCON = 0x4042;
    asm volatile ("disi #5");
    __builtin_write_NVM();
    while(NVMCONbits.WR);
}

unsigned int Flash_Read(unsigned int page, unsigned int offset)
{
    unsigned int val;
    while(NVMCONbits.WR);
    TBLPAG = page;
    val = __builtin_tblrdl(offset);
    return val;
}


void Flash_Write(unsigned int page, unsigned int offset, unsigned int val)
{
    while(NVMCONbits.WR);

    NVMCON = 0x4003;
    TBLPAG = page;
    __builtin_tblwtl(offset, val);
    __builtin_tblwth(offset, 0xFF);
    asm volatile ("disi #5");
    __builtin_write_NVM();

    while(NVMCONbits.WR);
}

void SYSTEM_Initialize (void)
{

    TRISBbits.TRISB2 = 1;   //P6: set to Input  for UART Rx
    TRISBbits.TRISB3 = 0;   //P7: set to Output for UART Tx

}

/*********************************************************************************************
 * get serial from command
 ********************************************************************************************/
void get_command(char *buf){
    unsigned char i = 0;
    char cmd;

    for(i =0;i<BUF_SIZE;i++){
        cmd = uart_get_char_2400();
        buf[i] = cmd;
        if ((cmd == '\n')||(cmd=='\r')){
            break;
        }
    }
}

