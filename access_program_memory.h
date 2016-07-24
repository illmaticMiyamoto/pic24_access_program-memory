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
 * Flash_Init(void);
 *
 * Initialize registor to erase data.
 ******************************************************************************/
void Flash_Init(void);
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

