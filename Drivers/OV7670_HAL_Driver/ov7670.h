#ifndef _OV7670_H
#define _OV7670_H

#include <stdint.h>
#include "stm32wlxx_hal.h"

void CaptureFrame(uint32_t frame_address, uint8_t scale);

#endif
