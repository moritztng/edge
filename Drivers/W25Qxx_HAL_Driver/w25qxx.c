#include "w25qxx.h"
#include "stm32wlxx_hal.h"

w25qxx_t w25qxx;
extern SPI_HandleTypeDef hspi2;

uint8_t Spi_Send(uint8_t data)
{
	uint8_t reception;
	HAL_SPI_TransmitReceive(&hspi2, &data, &reception, 1, 100);
	return reception;
}

uint32_t W25qxx_ReadID(void)
{
	uint32_t Temp = 0, Temp0 = 0, Temp1 = 0, Temp2 = 0;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	Spi_Send(0x9F);
	Temp0 = Spi_Send(0xA5);
	Temp1 = Spi_Send(0xA5);
	Temp2 = Spi_Send(0xA5);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
	Temp = (Temp0 << 16) | (Temp1 << 8) | Temp2;
	return Temp;
}
void W25qxx_ReadUniqID(void)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	Spi_Send(0x4B);
	for (uint8_t i = 0; i < 4; i++)
		Spi_Send(0xA5);
	for (uint8_t i = 0; i < 8; i++)
		w25qxx.UniqID[i] = Spi_Send(0xA5);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}
uint8_t W25qxx_ReadStatusRegister(uint8_t SelectStatusRegister_1_2_3)
{
	uint8_t status = 0;
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	if (SelectStatusRegister_1_2_3 == 1)
	{
		Spi_Send(0x05);
		status = Spi_Send(0xA5);
		w25qxx.StatusRegister1 = status;
	}
	else if (SelectStatusRegister_1_2_3 == 2)
	{
		Spi_Send(0x35);
		status = Spi_Send(0xA5);
		w25qxx.StatusRegister2 = status;
	}
	else
	{
		Spi_Send(0x15);
		status = Spi_Send(0xA5);
		w25qxx.StatusRegister3 = status;
	}
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
	return status;
}
uint8_t W25qxx_Init(void)
{
	w25qxx.Lock = 1;
	while (HAL_GetTick() < 100)
		HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_Delay(100);
	uint32_t id;
	id = W25qxx_ReadID();
	switch (id & 0x000000FF)
	{
	case 0x20: // 	w25q512
		w25qxx.ID = W25Q512;
		w25qxx.BlockCount = 1024;
		break;
	case 0x19: // 	w25q256
		w25qxx.ID = W25Q256;
		w25qxx.BlockCount = 512;
		break;
	case 0x18: // 	w25q128
		w25qxx.ID = W25Q128;
		w25qxx.BlockCount = 256;
		break;
	case 0x17: //	w25q64
		w25qxx.ID = W25Q64;
		w25qxx.BlockCount = 128;
		break;
	case 0x16: //	w25q32
		w25qxx.ID = W25Q32;
		w25qxx.BlockCount = 64;
		break;
	case 0x15: //	w25q16
		w25qxx.ID = W25Q16;
		w25qxx.BlockCount = 32;
		break;
	case 0x14: //	w25q80
		w25qxx.ID = W25Q80;
		w25qxx.BlockCount = 16;
		break;
	case 0x13: //	w25q40
		w25qxx.ID = W25Q40;
		w25qxx.BlockCount = 8;
		break;
	case 0x12: //	w25q20
		w25qxx.ID = W25Q20;
		w25qxx.BlockCount = 4;
		break;
	case 0x11: //	w25q10
		w25qxx.ID = W25Q10;
		w25qxx.BlockCount = 2;
		break;
	default:
		w25qxx.Lock = 0;
		return 0;
	}
	w25qxx.PageSize = 256;
	w25qxx.SectorSize = 0x1000;
	w25qxx.SectorCount = w25qxx.BlockCount * 16;
	w25qxx.PageCount = (w25qxx.SectorCount * w25qxx.SectorSize) / w25qxx.PageSize;
	w25qxx.BlockSize = w25qxx.SectorSize * 16;
	w25qxx.CapacityInKiloByte = (w25qxx.SectorCount * w25qxx.SectorSize) / 1024;
	W25qxx_ReadUniqID();
	W25qxx_ReadStatusRegister(1);
	W25qxx_ReadStatusRegister(2);
	W25qxx_ReadStatusRegister(3);
	w25qxx.Lock = 0;
	return 1;
}

void W25qxx_Read(uint8_t *buffer, uint32_t address, uint32_t size)
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
    Spi_Send(0x0B);
	Spi_Send((address & 0xFF0000) >> 16);
	Spi_Send((address & 0xFF00) >> 8);
	Spi_Send(address & 0xFF);
	Spi_Send(0);
	HAL_SPI_Receive(&hspi2, buffer, size, 2000);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_Delay(1);
}
void W25qxx_EnableWrite()
{
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	Spi_Send(0x06);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
	HAL_Delay(1);
}
void W25qxx_WritePage(uint32_t page_address, uint8_t *p_data, uint16_t size)
{
	W25qxx_EnableWrite();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	Spi_Send(0x02);
	Spi_Send((page_address & 0xFF0000) >> 16);
	Spi_Send((page_address & 0xFF00) >> 8);
	Spi_Send(page_address & 0xFF);
	HAL_SPI_Transmit(&hspi2, p_data, size, 100);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}
void W25qxx_WaitWriteEnd()
{
	HAL_Delay(1);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	Spi_Send(0x05);
	uint8_t ret;
	do
	{
		ret = Spi_Send(0xA5);
		HAL_Delay(1);
	} while ((ret & 0x01) == 0x01);
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
}

void W25qxx_EraseSector(uint32_t sector_number)
{
	uint32_t sector_address = sector_number * 4096;
	W25qxx_EnableWrite();
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_RESET);
	Spi_Send(0x20);
	Spi_Send((sector_address & 0xFF0000) >> 16);
	Spi_Send((sector_address & 0xFF00) >> 8);
	Spi_Send((sector_address & 0xFF));
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET);
	W25qxx_WaitWriteEnd();
	HAL_Delay(1);
}
