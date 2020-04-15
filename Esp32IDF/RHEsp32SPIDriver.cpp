// RHSPIDriver.cpp
//
// Copyright (C) 2014 Mike McCauley
// $Id: RHSPIDriver.cpp,v 1.11 2017/11/06 00:04:08 mikem Exp $

#include <RHEsp32SPIDriver.h>

static uint8_t out[256];
static uint8_t in[256];

RHEsp32SPIDriver::RHEsp32SPIDriver(uint8_t slaveSelectPin, HardwareSPI &spi)
    : RHSPIDriver(slaveSelectPin, spi) {}

// uint8_t RHEsp32SPIDriver::transfer(uint8_t reg) { return spiRead(reg); }

uint8_t RHEsp32SPIDriver::spiRead(uint8_t reg) {
  uint8_t val;

  spi_transaction_t t = {.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA,
                         .length = 8 * 2,
                         .rxlength = 8,
                         .tx_data = {reg}};
  gpio_set_level(_csPin, LOW);
  esp_err_t status = spi_device_transmit(_spiDevice, &t); // Transmit!
  assert(status == ESP_OK);
  gpio_set_level(_csPin, HIGH);

  val = t.rx_data[0];
  ESP_LOGD(TAG, "spi read: %02x = %02x", reg, val);
  return val;
}

// uint8_t RHEsp32SPIDriver::spiWrite(uint8_t reg, uint8_t val) {
//   uint8_t status = 0;
//   spi_transaction_t t;
//   memset(&t, 0, sizeof(t));
//   t.flags = SPI_TRANS_USE_TXDATA;
//   t.length = 8 * 2;
//   t.tx_data[0] = reg | RH_SPI_WRITE_MASK;
//   t.tx_data[1] = val;
//   t.rx_buffer = NULL;
//   t.user = (void *)0;
//   status = spi_device_polling_transmit(_spiDevice, &t); // Transmit!
//   assert(status == ESP_OK);
//   return status;
// }

// uint8_t RHEsp32SPIDriver::spiBurstRead(uint8_t reg, uint8_t *dest,
//                                        uint8_t len) {
//   uint8_t status = 0;
//   spi_transaction_t t;
//   memset(&t, 0, sizeof(t));
//   t.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA;
//   t.length = 8;
//   t.tx_data[0] = reg & ~RH_SPI_WRITE_MASK;
//   t.user = (void *)0;
//   for (int i = 0; i < len; i++) {
//     status = spi_device_transmit(spi, &t); // Transmit!
//     assert(status == ESP_OK);
//     *dest++ = t.rx_data[0];
//     t.tx_data[0] = 0;
//   }
//   return status;
// }

// uint8_t RHEsp32SPIDriver::spiBurstWrite(uint8_t reg, const uint8_t *src,
//                                         uint8_t len) {
//   uint8_t status = 0;
//   spi_transaction_t t;
//   memset(&t, 0, sizeof(t));
//   t.flags = SPI_TRANS_USE_TXDATA;
//   t.length = 8;
//   t.tx_data[0] = reg | RH_SPI_WRITE_MASK;
//   t.user = (void *)0;
//   status = spi_device_transmit(spi, &t); // Transmit!
//   assert(status == ESP_OK);
//   t.flags = 0;
//   t.length = 8 * len;
//   t.tx_buffer = src;
//   status = spi_device_transmit(spi, &t); // Transmit!
//   assert(status == ESP_OK);
//   return status;
// }

void RHEsp32SPIDriver::spiUsingInterrupt(uint8_t interruptNumber) {
  _spi.usingInterrupt(interruptNumber);
}
