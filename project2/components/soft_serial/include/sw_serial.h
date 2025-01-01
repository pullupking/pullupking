/*
Github: junhuanchen
Copyright (c) 2018 Juwan
Licensed under the MIT license:
http://www.opensource.org/licenses/mit-license.php
*/
#ifndef Sw_SOFTWARE_SERIAL_H
#define Sw_SOFTWARE_SERIAL_H

#include <inttypes.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <freertos/portmacro.h>
#include <stdio.h>
#include <esp_private/esp_clk.h>
#include <driver/gpio.h>
#include <esp_cpu.h>

#define SW_EOF -1 

typedef struct sw_serial
{
    gpio_num_t rxPin, txPin;
    uint32_t buffSize, bitTime, rx_start_time, rx_end_time;
    bool invert, overflow;
    volatile uint32_t inPos, outPos;
    uint8_t *buffer;
} SwSerial;

SwSerial *sw_new(gpio_num_t Tx, gpio_num_t Rx, bool Inverse, int buffSize);

void sw_del(SwSerial *self);

uint32_t getCycleCount();

#define WaitBitTime(wait) \
    for (uint32_t start = getCycleCount(); getCycleCount() - start < wait;)


esp_err_t sw_enableRx(SwSerial *self, bool State);

int sw_write(SwSerial *self, uint8_t byte);

int sw_read(SwSerial *self);

esp_err_t sw_open(SwSerial *self, uint32_t baudRate);

esp_err_t sw_stop(SwSerial *self);

int sw_any(SwSerial *self);

void sw_flush(SwSerial *self);

bool sw_overflow(SwSerial *self);

int sw_peek(SwSerial *self);

#endif