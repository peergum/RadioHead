// HardwareSPI.cpp
// Author: Phil Hilger (phil@peergum.com)
// Copyright (C) 2020 Phil Hilger

// #include "RHEsp32SPI.h"
#include "HardwareSPI.h"
#include <esp_log.h>

static const char *TAG = "SPI";
HardwareSPI::HardwareSPI(gpio_num_t csPin, spi_host_device_t spiHost,
                         gpio_num_t mosi, gpio_num_t miso, gpio_num_t sck,
                         gpio_num_t rst)
    : _csPin(csPin), _spiHost(spiHost), _mosi(mosi), _miso(miso), _sck(sck),
      _rst(rst) {}

// void HardwareSPI::begin(Frequency frequency, BitOrder bitOrder, DataMode
// dataMode) {
void HardwareSPI::begin() {
  _busConfig.mosi_io_num = _mosi;
  _busConfig.miso_io_num = _miso;
  _busConfig.sclk_io_num = _sck;
  _busConfig.quadwp_io_num = -1;
  _busConfig.quadhd_io_num = -1;
  _busConfig.max_transfer_sz = 0;

  _deviceConfig.mode = 0;
  _deviceConfig.clock_speed_hz = 9000000;
  _deviceConfig.spics_io_num = _csPin;
  _deviceConfig.queue_size = 1;
  _deviceConfig.flags = 0;
  _deviceConfig.pre_cb = NULL;

  ESP_ERROR_CHECK(spi_bus_initialize(_spiHost, &_busConfig, 0));
  gpio_set_direction(_csPin, GPIO_MODE_OUTPUT);
  gpio_set_level(_csPin, HIGH);
  gpio_set_direction(_rst, GPIO_MODE_OUTPUT);
  gpio_set_level(_rst, HIGH);
  ESP_ERROR_CHECK(spi_bus_add_device(_spiHost, &_deviceConfig, &_spiDevice));
  gpio_set_level(_rst, LOW);
  delay(1);
  gpio_set_level(_rst, HIGH);
  delay(10);
}

void HardwareSPI::end() { ESP_ERROR_CHECK(spi_bus_free(_spiHost)); }

// void HardwareSPI::beginTransaction() {}

// void HardwareSPI::endTransaction() {}

uint8_t HardwareSPI::transfer(uint8_t reg) {
  uint8_t val;

  spi_transaction_t t = {.flags = SPI_TRANS_USE_TXDATA | SPI_TRANS_USE_RXDATA,
                         .length = 8 * 2,
                         //  .rxlength = 8,
                         .tx_data = {reg}};
  if (!(reg & SPI_WRITE_MASK)) {
    t.rxlength = 8;
  }
  // gpio_set_level(_csPin, LOW);
  esp_err_t status = spi_device_transmit(_spiDevice, &t); // Transmit!
  assert(status == ESP_OK);
  // gpio_set_level(_csPin, HIGH);

  val = t.rx_data[0];
  // ESP_LOGD(TAG, "spi %s: %02x (%c) = %02x", reg >= SPI_WRITE_MASK ? "write" :
  // "read",
  //          reg & ~SPI_WRITE_MASK, reg & ~SPI_WRITE_MASK, val);
  return val;
}

uint8_t HardwareSPI::spiRead(uint8_t reg) {
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
  // ESP_LOGD(TAG, "spi read: %02x = %02x", reg, val);
  return val;
}

uint8_t HardwareSPI::spiWrite(uint8_t reg, uint8_t val) {
  uint8_t status = 0;
  uint8_t out[2] = {(uint8_t)(reg | SPI_WRITE_MASK), val};
  uint8_t in[2];
  spi_transaction_t t = {
      .flags = 0, .length = 8 * sizeof(out), .tx_buffer = out, .rx_buffer = in};
  gpio_set_level(_csPin, LOW);

  status = spi_device_polling_transmit(_spiDevice, &t); // Transmit!
  assert(status == ESP_OK);
  gpio_set_level(_csPin, HIGH);
  // ESP_LOGD(TAG, "spi write: %02x = %02x", reg, val);
  return status;
}

uint8_t HardwareSPI::spiBurstRead(uint8_t reg, uint8_t *dest, uint8_t len) {
  uint8_t status = 0;
  ATOMIC_BLOCK_START;
  beginTransaction();
  digitalWrite(_csPin, LOW);
  status = transfer(
      reg &
      ~0x80); // Send the start address with the write mask off
  while (len--)
    *dest++ = transfer(0);
  digitalWrite(_csPin, HIGH);
  endTransaction();
  ATOMIC_BLOCK_END;
  return status;
}

uint8_t HardwareSPI::spiBurstWrite(uint8_t reg, const uint8_t *src,
                                   uint8_t len) {
    uint8_t status = 0;
    // Serial.println("spiBurstWrite");
    ATOMIC_BLOCK_START;
    beginTransaction();
    digitalWrite(_csPin, LOW);
    status = transfer(reg | 0x80); // Send the start address with the write mask on
    while (len--)
	transfer(*src++);
    digitalWrite(_csPin, HIGH);
    endTransaction();
    ATOMIC_BLOCK_END;
    return status;
}

void HardwareSPI::beginTransaction() {
  spi_device_acquire_bus(_spiDevice, pdMS_TO_TICKS(1000));
}

void HardwareSPI::endTransaction() { spi_device_release_bus(_spiDevice); }
void HardwareSPI::spiUsingInterrupt(uint8_t interruptNumber) {
  // usingInterrupt(interruptNumber);
}