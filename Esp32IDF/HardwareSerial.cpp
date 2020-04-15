/**
 * @file HardwareSerial.cpp
 * @author Phil Hilger (phil.hilger@waterlooti.com)
 * @brief
 * @version 0.1
 * @date 2020-03-24
 *
 * @copyright (c) 2020, WaterlooTI
 *
 */

#include "HardwareSerial.h"
#include "esp_log.h"
#include "stdio.h"
#include "esp_err.h"
#include "soc/uart_struct.h"
#include "RHEsp32.h"

static const char *TAG = "LORA";

HardwareSerial::HardwareSerial() {

}

HardwareSerial::HardwareSerial(uart_port_t uart_num, const uart_config_t *config, gpio_num_t tx, gpio_num_t rx)
: _uart_num(uart_num) {
  ESP_ERROR_CHECK(uart_param_config(uart_num, config));
  ESP_ERROR_CHECK(uart_set_pin(uart_num, tx, rx, -1, -1));
  ESP_ERROR_CHECK(uart_driver_install(uart_num, _uart_buffer_size, _uart_buffer_size, 10, &_uart_queue, 0));
}

int HardwareSerial::available() {
  int length = 0;
  ESP_ERROR_CHECK(uart_get_buffered_data_len(_uart_num, (size_t *)&length));
  return length;
}

int HardwareSerial::waitAvailable() {
  while (!available())
    vTaskDelay(2);
  ;
  return available();
}

int HardwareSerial::waitAvailableTimeout(unsigned long timeout) {
  unsigned long start = millis();
  while (!available() && millis()-start < timeout) {
    vTaskDelay(2);
  }
  return available();
}

int HardwareSerial::read(void) {
  int length = 0;
  uint8_t data;
  ESP_ERROR_CHECK(uart_get_buffered_data_len(_uart_num, (size_t *)&length));
  if (!length) {
    return -1;
  }
  length = uart_read_bytes(_uart_num, &data, 1, 100);
  if (!length) {
    return -1;
  }
  return data;
}

int HardwareSerial::write(const char ch) {
  uart_write_bytes(_uart_num, &ch, 1);
  return ch;
}

void HardwareSerial::print(const char c) { print("%c", c); }

void HardwareSerial::print(uint8_t value, uint8_t type = DEC) {
  switch (type) {
  case HEX:
    print("%02x", value);
    break;
  case DEC:
    print("%d", value);
    break;
  }
}

void HardwareSerial::println(uint8_t value, uint8_t type = DEC) {
  print(value, type);
}

void HardwareSerial::print(const char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  debugPrint(fmt, arg);
  va_end(arg);
}

void HardwareSerial::printf(const char *fmt...) {
  va_list arg;
  va_start(arg, fmt);
  debugPrint(fmt, arg);
  va_end(arg);
}

void HardwareSerial::println(const char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  debugPrint(fmt, arg);
  va_end(arg);
}

void HardwareSerial::printlnf(const char *fmt, ...) {
  va_list arg;
  va_start(arg, fmt);
  debugPrint(fmt, arg);
  va_end(arg);
}

void HardwareSerial::debugPrint(const char *fmt, va_list arg) {
    char buffer[1024] = {0};
    vsnprintf(buffer,1023,fmt, arg);
    ESP_LOGD(TAG, "%s", buffer);
}


