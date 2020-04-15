/**
 * @file Esp32.cpp
 * @author Phil Hilger (phil.hilger@waterlooti.com)
 * @brief
 * @version 0.1
 * @date 2020-03-18
 *
 * @copyright (c) 2020, WaterlooTI
 *
 */

#include "driver/gpio.h"
#include "esp_err.h"
#include "freertos/FreeRTOS.h"
#include "sys/time.h"
#include "freertos/task.h"
#include "esp_log.h"

#include "RHEsp32.h"

static const char *TAG = "Esp32";

struct timeval RHStartTime;

// static void (*isr_handler)(void) = NULL;
// static intr_handle_t interruptHandle;

void pinMode(uint8_t pin, uint8_t mode)
{
  // gpio_pad_select_gpio(pin);
  // gpio_set_direction(pin, mode == OUTPUT ? GPIO_MODE_OUTPUT : GPIO_MODE_INPUT);
  // return;
  gpio_config_t config = {.pin_bit_mask = (1ULL << pin),
                          .mode = GPIO_MODE_DEF_OUTPUT,
                          .intr_type = GPIO_PIN_INTR_DISABLE,
                          .pull_up_en = GPIO_PULLUP_DISABLE,
                          .pull_down_en = GPIO_PULLDOWN_DISABLE};

  switch (mode)
  {
  case INPUT:
    config.mode = GPIO_MODE_INPUT;
    break;
  case INPUT_PULLUP:
    config.mode = GPIO_MODE_INPUT;
    config.pull_up_en = GPIO_PULLUP_ENABLE;
    break;
  case INPUT_PULLDOWN:
    config.mode = GPIO_MODE_INPUT;
    config.pull_down_en = GPIO_PULLDOWN_ENABLE;
    break;
  case OUTPUT:
    config.mode = GPIO_MODE_OUTPUT;
    break;
  default:
    break;
  }
  ESP_ERROR_CHECK(gpio_config(&config));
}

void digitalWrite(uint8_t pin, uint8_t value) {
  ESP_ERROR_CHECK(gpio_set_level(pin, value));
}

unsigned long millis() {
  // Declare a variable to store current time
  struct timeval RHCurrentTime;
  // Get current time
  gettimeofday(&RHCurrentTime, NULL);
  // Calculate the difference between our start time and the end time
  unsigned long difference =
      ((RHCurrentTime.tv_sec - RHStartTime.tv_sec) * 1000);
  difference += ((RHCurrentTime.tv_usec - RHStartTime.tv_usec) / 1000);
  // Return the calculated value
  return difference;
}

void delay(unsigned long ms) {
  // Implement Delay function
  vTaskDelay(ms / portTICK_RATE_MS);
}

uint8_t digitalPinToInterrupt(uint8_t interruptPin) { return interruptPin; }

// static void IRAM_ATTR gpioInterruptHandler(void *param) {
//   ESP_LOGD(TAG, "interruption");
//   if (isr_handler) {
//     isr_handler();
//     ESP_LOGD(TAG, "handled");
//   }
// }

// void yield(void) {
//    int irq = SPI.spiRead(RH95_REG_12);
//    lora_write_reg(REG_IRQ_FLAGS, irq);
//    if((irq & IRQ_RX_DONE_MASK) == 0) return 0;
//    if(irq & IRQ_PAYLOAD_CRC_ERROR_MASK) return 0;


//   // gpioInterruptHandler(NULL);
// }

void attachInterrupt(int pin, void (*handler)(void), int mode) {
  ESP_LOGD(TAG, "Interrupt added on %d", pin);
  gpio_config_t config = {.pin_bit_mask = (1ULL << pin),
                          .mode = GPIO_MODE_INPUT,
                          .pull_up_en = GPIO_PULLUP_DISABLE,
                          .pull_down_en = GPIO_PULLDOWN_DISABLE,
                          .intr_type = GPIO_INTR_POSEDGE};
  switch (mode) {
  case CHANGE:
    config.intr_type = GPIO_INTR_ANYEDGE;
    break;
  case RISING:
    config.intr_type = GPIO_INTR_POSEDGE;
    config.pull_up_en = GPIO_PULLUP_ENABLE;
    break;
  case FALLING:
    config.intr_type = GPIO_INTR_NEGEDGE;
    // config.pull_up_en = GPIO_PULLUP_ENABLE;
    break;
  default:
    break;
  }
  // esp_intr_alloc(ETS_GPIO_INTR_SOURCE, 0, (void *)handler, NULL, &interruptHandle);
  ESP_ERROR_CHECK(gpio_config(&config));
  // isr_handler = handler;
  ESP_ERROR_CHECK(gpio_isr_handler_remove(pin));
  ESP_ERROR_CHECK(gpio_isr_handler_add(pin, (gpio_isr_t) handler, (void *)pin));
}

void prepareInterrupt(void)
{
  ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT));
}
