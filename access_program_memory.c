#include <xc.h>
#include "access_program_memory.h"


const __attribute__((section("my_memory"), space (prog), address (FLASH_BLOCK_ADDRESS) )) unsigned int _flash_datas[96*8]; //test write
const __attribute__((section("my_memory"), space (prog), address (FLASH_BLOCK_ADDRESS+BLOCK_FLASH_SIZE) )) unsigned int _flash_datas2[96*8]; //test write


/*
 * prototype
 */
static void Flash_Erase(unsigned int page, unsigned int offset);
static unsigned int Flash_Read(unsigned int page, unsigned int offset);
static void Flash_Write(unsigned int page, unsigned int offset, unsigned int val);
static int Flash_Bulk_Write_With_Erase(unsigned int page, unsigned int offset, unsigned int val);



/******************************************************************************
 * void Flash_Erase(unsigned int page, unsigned int offset);
 *
 * Erase memory 0x400 registor size.
 *  -Turn all data to 0xFF (Initial data = 0x00)
 * ex)   Erase 0x9000~0x9400 : Flash_Erase((page,0x9600);
 *  also Erase 0x9000~0x9400 : Flash_Erase((page,0x9700);
 ******************************************************************************/
static void Flash_Erase(unsigned int page, unsigned int offset)
{
    while(NVMCONbits.WR);
    TBLPAG = page;
    __builtin_tblwtl(offset, 0x0000);
    NVMCON = 0x4042;
    asm volatile ("disi #5");
    __builtin_write_NVM();
    while(NVMCONbits.WR);
}

/******************************************************************************
 * unsigned int Flash_Read(unsigned int page, unsigned int offset);
 * Read data;
 ******************************************************************************/
static unsigned int Flash_Read(unsigned int page, unsigned int offset)
{
    unsigned int val;
    while(NVMCONbits.WR);
    TBLPAG = page;
    val = __builtin_tblrdl(offset);
    return val;
}

/******************************************************************************
 * void Flash_Write(unsigned int page, unsigned int offset, unsigned int val);
 *
 * Write the data to regisor.
 * It is impossible to overwrite data.
 ******************************************************************************/
static void Flash_Write(unsigned int page, unsigned int offset, unsigned int val)
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

/******************************************************************************
 * Flash_Init(void);
 *
 * Initialize registor to erase data.
 ******************************************************************************/
void Flash_Init(void)
{
    unsigned int page   = __builtin_tblpage(_flash_datas);
    unsigned int offset = __builtin_tbloffset(_flash_datas);
    Flash_Erase(page, offset);
    Flash_Erase(page, offset+ERASE_REGISTOR_SIZE);
    Flash_Erase(page, offset+2*ERASE_REGISTOR_SIZE);
}

/******************************************************************************
 * Flash_Bulk_Write_With_Erase(unsigned int page, unsigned int offset, unsigned int val);
 *
 * Write the data to registor.
 *  -Read all data and stored to the BUFFER.
 *  -Write the new data to the buffer.
 *  -Erase 0x400 size block registor.
 *  -Write all data to the program memory.
 ******************************************************************************/
int Flash_Bulk_Write_With_Erase(unsigned int page, unsigned int offset, unsigned int val)
{
    unsigned int i=0;
    unsigned int buffer[ERASE_BUFFER_SIZE];
    unsigned char erase_block;

    //buffer[i + head] = values[i];
    if(offset%2==1)return -1;
    else if(offset<FLASH_BLOCK_ADDRESS)return -1;
    else if(offset<FLASH_BLOCK_ADDRESS+ERASE_REGISTOR_SIZE)
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
    }else return -1;


    //rescue to buffer
    TBLPAG = page;
    for (i=0; i< ERASE_BUFFER_SIZE; i++) {
        buffer[i] = Flash_Read(page, FLASH_BLOCK_ADDRESS+erase_block*ERASE_REGISTOR_SIZE + 2*i);
    }

    //write data to buffer
    buffer[(offset-FLASH_BLOCK_ADDRESS-erase_block*ERASE_REGISTOR_SIZE)/2] =val;


    //erase flash rom
    Flash_Erase( page, FLASH_BLOCK_ADDRESS+erase_block*ERASE_REGISTOR_SIZE);

    for (i=0; i<ERASE_BUFFER_SIZE; i++)  Flash_Write(page,  FLASH_BLOCK_ADDRESS+erase_block*ERASE_REGISTOR_SIZE + 2*i, buffer[i]);

    return 0;
}


/******************************************************************************
 * unsigned int Read_Memory(unsigned int address);
 ******************************************************************************/
unsigned int Read_Memory(unsigned int address){
    unsigned int data;
    unsigned int page   = __builtin_tblpage(_flash_datas);

    data = Flash_Read(page, FLASH_BLOCK_ADDRESS + 2*address);

    return data;
}
/******************************************************************************
 * unsigned int Write_Memory(unsigned int address, unsigned int data);
 ******************************************************************************/
unsigned int Write_Memory(unsigned int address, unsigned int data){
    unsigned int page   = __builtin_tblpage(_flash_datas);
    int state = Flash_Bulk_Write_With_Erase(page, FLASH_BLOCK_ADDRESS + 2*address, data);
    return state;
}

