/**
 * @file Esp32.h
 * @author Phil Hilger (phil.hilger@waterlooti.com)
 * @brief 
 * @version 0.1
 * @date 2020-03-18
 * 
 * @copyright (c) 2020, WaterlooTI
 * 
 */

#ifndef __RH_ESP32_H
#define __RH_ESP32_H

#include <stdint.h>
#include <string.h>
#include <math.h>

enum
{
    INPUT,
    OUTPUT,
    INPUT_PULLUP,
    INPUT_PULLDOWN
};

enum
{
    CHANGE = 1,
    FALLING,
    RISING,
};

enum {
    LOW = 0,
    HIGH
};

#define ESP_INTR_FLAG_DEFAULT 0

#define random(min, max) (min + rand() * (max - min))

#define RFM_MOSI   GPIO_NUM_13
#define RFM_MISO   GPIO_NUM_12
#define RFM_SCK    GPIO_NUM_14
#define RFM_INT    GPIO_NUM_26
#define RFM_CS     GPIO_NUM_16
#define RFM_RST    GPIO_NUM_27
#define RFM_DIO1   GPIO_NUM_33
#define RFM_DIO2   GPIO_NUM_32

#ifdef __cplusplus
    extern "C" {
#endif

    void pinMode(unsigned char pin, unsigned char mode);
    void digitalWrite(unsigned char pin, unsigned char value);
    unsigned long millis();
    void delay(unsigned long ms);
    uint8_t digitalPinToInterrupt(uint8_t interruptPin);
    void attachInterrupt(int pin, void (*handler)(void ), int mode);
    void yield(void);
    void prepareInterrupt(void);

#ifdef __cplusplus
}
#endif

#endif // __RH_ESP32_H