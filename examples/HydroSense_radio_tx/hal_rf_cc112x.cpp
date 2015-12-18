#include <stdint.h>

#include "components/common/hal_rf.h"
#include "components/devices/cc112x/cc112x_spi.h"

uint8_t halRfWriteReg(uint16_t addr, uint8_t data)
{
  return cc112xSpiWriteReg(addr, &data, 1);
}
