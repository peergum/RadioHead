/**
 * @file RHEsp32SPI.h
 * @author Phil Hilger (phil.hilger@waterlooti.com)
 * @brief 
 * @version 0.1
 * @date 2020-03-23
 * 
 * @copyright (c) 2020, WaterlooTI
 * 
 */

#ifndef __RH_ESP32_SPI_H
#define __RH_ESP32_SPI_H

#include "driver/spi_common.h"
#include "driver/spi_master.h"
#include "RHGenericSPI.h"
#include "RHSPIDriver.h"

class RHEsp32SPI : public HardwareSPI
{
public:
    RHEsp32SPI(int csPin, HardwareSPI &spi);
    void begin(void);
    // void begin(uint32_t frequency, RHGenericSPI::BitOrder bitOrder, RHGenericSPI::DataMode datamode);
    void end(void);

    void usingInterrupt(uint8_t interruptNumber);

private:
    int _csPin;
    HardwareSPI& _spi;
    spi_device_interface_config_t _deviceConfig;
    spi_device_handle_t _spiDevice;
    bool _usesInterrupt = false;
};

#endif
