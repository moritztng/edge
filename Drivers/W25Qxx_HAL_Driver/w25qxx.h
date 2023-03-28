#ifndef _W25QXX_H
#define _W25QXX_H

#include <stdint.h>

typedef enum
{
	W25Q10 = 1,
	W25Q20,
	W25Q40,
	W25Q80,
	W25Q16,
	W25Q32,
	W25Q64,
	W25Q128,
	W25Q256,
	W25Q512,

} W25QXX_ID_t;

typedef struct
{
	W25QXX_ID_t ID;
	uint8_t UniqID[8];
	uint16_t PageSize;
	uint32_t PageCount;
	uint32_t SectorSize;
	uint32_t SectorCount;
	uint32_t BlockSize;
	uint32_t BlockCount;
	uint32_t CapacityInKiloByte;
	uint8_t StatusRegister1;
	uint8_t StatusRegister2;
	uint8_t StatusRegister3;
	uint8_t Lock;

} w25qxx_t;

extern w25qxx_t w25qxx;

uint8_t W25qxx_Init(void);
void W25qxx_Read(uint8_t *buffer, uint32_t address, uint32_t size);
void W25qxx_EnableWrite();
void W25qxx_WritePage(uint32_t page_address, uint8_t *p_data, uint16_t size);
void W25qxx_WaitWriteEnd();
void W25qxx_EraseSector(uint32_t sector_number);

#endif
