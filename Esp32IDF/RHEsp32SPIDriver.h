// RHSPIDriver.h
// Author: Mike McCauley (mikem@airspayce.com)
// Copyright (C) 2014 Mike McCauley
// $Id: RHSPIDriver.h,v 1.14 2019/09/06 04:40:40 mikem Exp $

#ifndef RHEsp32SPIDriver_h
#define RHEsp32SPIDriver_h

// #include <RHSPIDriver.h>
// #include <RHHardwareSPI.h>
#include <RHEsp32SPI.h>
#include <RHSPIDriver.h>

class RHEsp32SPIDriver: public RHGenericSPI
{
public:
    /// Constructor
    /// \param[in] slaveSelectPin The controler pin to use to select the desired SPI device. This pin will be driven LOW
    /// during SPI communications with the SPI device that uis iused by this Driver.
    /// \param[in] spi Reference to the SPI interface to use. The default is to use a default built-in Hardware interface.
    RHEsp32SPIDriver(uint8_t slaveSelectPin, RHGenericSPI& spi);

    /// Reads a single register from the SPI device
    /// \param[in] reg Register number
    /// \return The value of the register
    uint8_t        spiRead(uint8_t reg);

    /// Writes a single byte to the SPI device
    /// \param[in] reg Register number
    /// \param[in] val The value to write
    /// \return Some devices return a status byte during the first data transfer. This byte is returned.
    ///  it may or may not be meaningfule depending on the the type of device being accessed.
    uint8_t           spiWrite(uint8_t reg, uint8_t val);

    /// Reads a number of consecutive registers from the SPI device using burst read mode
    /// \param[in] reg Register number of the first register
    /// \param[in] dest Array to write the register values to. Must be at least len bytes
    /// \param[in] len Number of bytes to read
    /// \return Some devices return a status byte during the first data transfer. This byte is returned.
    ///  it may or may not be meaningfule depending on the the type of device being accessed.
    uint8_t           spiBurstRead(uint8_t reg, uint8_t* dest, uint8_t len);

    /// Write a number of consecutive registers using burst write mode
    /// \param[in] reg Register number of the first register
    /// \param[in] src Array of new register values to write. Must be at least len bytes
    /// \param[in] len Number of bytes to write
    /// \return Some devices return a status byte during the first data transfer. This byte is returned.
    ///  it may or may not be meaningfule depending on the the type of device being accessed.
    uint8_t           spiBurstWrite(uint8_t reg, const uint8_t* src, uint8_t len);

    /// Set the SPI interrupt number
    /// If SPI transactions can occur within an interrupt, tell the low level SPI
    /// interface which interrupt is used
    /// \param[in] interruptNumber the interrupt number
    void spiUsingInterrupt(uint8_t interruptNumber);

    protected:
    /// Reference to the RHGenericSPI instance to use to transfer data with the SPI device
        spi_device_handle_t _spiDevice;
};

#endif
