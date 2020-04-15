/**
 * @file RHEsp32SPI.cpp
 * @author Phil Hilger (phil.hilger@waterlooti.com)
 * @brief
 * @version 0.1
 * @date 2020-03-23
 *
 * @copyright (c) 2020, WaterlooTI
 *
 */

#include "RHEsp32SPI.h"

RHEsp32SPI::RHEsp32SPI(int csPin, HardwareSPI &spi): _csPin(csPin), _spi(spi) {
  _deviceConfig.mode = 0;
  _deviceConfig.clock_speed_hz = SPI_MASTER_FREQ_8M;
  _deviceConfig.spics_io_num = csPin;
  _deviceConfig.queue_size = 1;
  _deviceConfig.flags = 0;
  _deviceConfig.pre_cb = NULL;
}

void RHEsp32SPI::begin() {
  _spi.begin(_deviceConfig,_spiDevice);
}

void RHEsp32SPI::end() {
  spi_bus_remove_device(_spiDevice);
  _spi.end();
}

spi_device_handle_t RHEsp32SPI::getHandle() {
  return _spiDevice;
}
