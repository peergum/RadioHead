// RHHardwareEsp32SPI.h
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2011 Mike McCauley
// Contributed by Joanna Rutkowska
// $Id: RHHardwareEsp32SPI.h,v 1.12 2020/01/05 07:02:23 mikem Exp $

#ifndef __HardwareSPI_H
#define __HardwareSPI_H

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include <string.h>
#include "RHEsp32.h"
#include <driver/gpio.h>
#include "RHGenericSPI.h"


#define SPI_WRITE_MASK 0x80

class HardwareSPI
{
public:
  HardwareSPI(gpio_num_t csPin, spi_host_device_t spiHost, gpio_num_t mosi, gpio_num_t miso, gpio_num_t sck, gpio_num_t rst);
  // void begin(Frequency frequency = Frequency1MHz, BitOrder bitOrder =
  // BitOrderMSBFirst, DataMode dataMode = DataMode0);
  void begin();
  void end();
  void beginTransaction();
  void endTransaction();
  uint8_t transfer(uint8_t data);
  uint8_t spiRead(uint8_t reg);
  uint8_t spiWrite(uint8_t reg, uint8_t val);
  uint8_t spiBurstRead(uint8_t reg, uint8_t *dest, uint8_t len);
  uint8_t spiBurstWrite(uint8_t reg, const uint8_t *src, uint8_t len);
  void spiUsingInterrupt(uint8_t interruptNumber);

protected:
  // Storage for SPI settings used in SPI transactions
  spi_device_handle_t _spiDevice;
  spi_bus_config_t _busConfig;
  spi_device_interface_config_t _deviceConfig;
  spi_device_interface_config_t *_pDeviceConfig;
  gpio_num_t _csPin;
  spi_host_device_t _spiHost;
  gpio_num_t _mosi, _miso, _sck, _rst;
};

// Built in default instance
extern HardwareSPI SPI;

#endif
