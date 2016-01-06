/* 
 * File:   access_program_memory.h
 * Author: Naoy_Miyamoto
 *
 * Created on 2016/01/06, 21:51
 */

#ifndef ACCESS_PROGRAM_MEMORY_H
#define	ACCESS_PROGRAM_MEMORY_H

#ifdef	__cplusplus
extern "C" {
#endif

#define ERASE_REGISTOR_SIZE 0x400
#define ERASE_BUFFER_SIZE   0x200
#define BLOCK_FLASH_SIZE    0x600   //one block reg size
#define TOTAL_FLASH_SIZE    0xC00   //total reg size (2block)
#define FLASH_BLOCK_ADDRESS 0x9000  //first address of the flash block
#define MEMORY_SIZE         0x600   //[word]


/******************************************************************************
 * void Flash_Erase(unsigned int page, unsigned int offset);
 *
 * Erase memory 0x400 registor size.
 *  -Turn all data to 0xFF (Initial data = 0x00)
 * ex)   Erase 0x9000~0x9400 : Flash_Erase((page,0x9600);
 *  also Erase 0x9000~0x9400 : Flash_Erase((page,0x9700);
 ******************************************************************************/
void Flash_Erase(unsigned int page, unsigned int offset);

/******************************************************************************
 * unsigned int Flash_Read(unsigned int page, unsigned int offset);
 * Read data;
 ******************************************************************************/
unsigned int Flash_Read(unsigned int page, unsigned int offset);

/******************************************************************************
 * void Flash_Write(unsigned int page, unsigned int offset, unsigned int val);
 *
 * Write the data to regisor.
 * It is impossible to overwrite data.
 ******************************************************************************/
 void Flash_Write(unsigned int page, unsigned int offset, unsigned int val);

/******************************************************************************
 * Flash_Init(void);
 *
 * Initialize registor to erase data.
 ******************************************************************************/
void Flash_Init(void);

/******************************************************************************
 * Flash_Bulk_Write_With_Erase(unsigned int page, unsigned int offset, unsigned int val);
 *
 * Write the data to registor.
 *  -Read all data and stored to the BUFFER.
 *  -Write the new data to the buffer.
 *  -Erase 0x400 size block registor.
 *  -Write all data to the program memory.
 *
 * error -> return -1
 ******************************************************************************/
int Flash_Bulk_Write_With_Erase(unsigned int page, unsigned int offset, unsigned int val);


/******************************************************************************
 * unsigned int Read_Memory(unsigned int address);
 * address: integer from 0 to 0x600-1
 ******************************************************************************/
unsigned int Read_Memory(unsigned int address);
/******************************************************************************
 * unsigned int Write_Memory(unsigned int address, unsigned int data);
 ******************************************************************************/
unsigned int Write_Memory(unsigned int address, unsigned int data);

#ifdef	__cplusplus
}
#endif

#endif	/* ACCESS_PROGRAM_MEMORY_H */

