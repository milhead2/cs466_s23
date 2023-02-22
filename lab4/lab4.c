/**
 * @brief CS466 Lab4 SPI Bit-Bang
 * 
 * Copyright (c) 2022 Washington State University.
 */

#include <stdio.h>

#include <FreeRTOS.h>
#include <task.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

#include "mGpio.h"

const uint8_t LED_PIN = 25;

void hardware_init(void)
{
    const uint LED_PIN = PICO_DEFAULT_LED_PIN;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    mGpioInit();
}
//
// gpioVerifyReadWrite()
//
// This is the main function of a task that I'm using to verify that 
// my GPIO and SPI functionality is working correctly.  It woll be retired 
// as I move on to actual GPIO-Expander Functionality.
//
void gpioVerifyReadWrite(void * notUsed)
{   
    const uint32_t queryDelayMs = 100;  // ms
    uint8_t regValue;
    uint8_t count=0;

    vTaskDelay(5000);

    while (true) 
    {
        mGpioWriteByte(IODIRB, count++);
        regValue = mGpioReadByte(IODIRB);
        printf("IODIRB: 0x%02x,  ", regValue);

        regValue = mGpioReadByte(IODIRA);
        printf("IODIRA: 0x%02x,  ", regValue);

        regValue = mGpioReadByte(IPOLA);
        printf("IPOLA: 0x%02x\n", regValue);

        vTaskDelay(queryDelayMs);
    }
}

// IODIRB: 0x00,  IODIRA: 0xff,  IPOLA: 0x00
// IODIRB: 0x01,  IODIRA: 0xff,  IPOLA: 0x00
// IODIRB: 0x02,  IODIRA: 0xff,  IPOLA: 0x00
// IODIRB: 0x03,  IODIRA: 0xff,  IPOLA: 0x00
// IODIRB: 0x04,  IODIRA: 0xff,  IPOLA: 0x00
// IODIRB: 0x05,  IODIRA: 0xff,  IPOLA: 0x00
// IODIRB: 0x06,  IODIRA: 0xff,  IPOLA: 0x00
// etc....

void heartbeat(void * notUsed)
{   
    const uint32_t heartbeatDelay = 1000;  // ms

    while (true) 
    {
        gpio_put(LED_PIN, 1);
        vTaskDelay(heartbeatDelay);
        gpio_put(LED_PIN, 0);
        vTaskDelay(heartbeatDelay);
    }
}

int main()
{
    stdio_init_all();
    printf("lab2 Hello!\n");

    hardware_init();

    xTaskCreate(heartbeat, "LED_Task", 256, NULL, 1, NULL);
    xTaskCreate(gpioVerifyReadWrite, "GPIO_Task", 256, NULL, 2, NULL);

    vTaskStartScheduler();

    while(1){};
}