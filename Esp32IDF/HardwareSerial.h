/**
 * @file HardwareSerial.h
 * @author Phil Hilger (phil.hilger@waterlooti.com)
 * @brief 
 * @version 0.1
 * @date 2020-03-24
 * 
 * @copyright (c) 2020, WaterlooTI
 * 
 */

#include <stdarg.h>
#include <stdint.h>
#include "driver/uart.h"
#include "driver/gpio.h"

#ifndef __HARDWARESERIAL_H
#define __HARDWARESERIAL_H

enum
{
    HEX,
    DEC
};

#ifdef __cplusplus
class HardwareSerial
{
public:
    HardwareSerial();
    HardwareSerial(uart_port_t uart_num, const uart_config_t *config, gpio_num_t tx, gpio_num_t rx);
    int available(void);
    int waitAvailable(void);
    int waitAvailableTimeout(unsigned long timeout);
    int read(void);
    int write(const char ch);
    void print(char value);
    void print(uint8_t value, uint8_t type);
    void println(uint8_t value, uint8_t type);
    void print(const char *fmt, ...);
    void printf(const char *fmt, ...);
    void println(const char *fmt, ...);
    void printlnf(const char *fmt, ...);
    void debugPrint(const char *fmt, va_list arg);

    private:
        uart_port_t _uart_num;
        int _uart_buffer_size = (1024 * 2);
        QueueHandle_t _uart_queue;

};

#endif

#endif