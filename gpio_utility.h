#ifndef _GPIO_UTILITY_H
#define _GPIO_UTILITY_H

#include <gpiod.h>



#define INPUT   0
#define OUTPUT  1

#define LOW     0
#define HIGH    1

// !!!!! TBD again!! !!!!!
#define SDA_PIN     13 
#define SCL_PIN     14
#define SWITCH_PIN  17
#define LED_PIN     18

//void pullUpDnControl(int pin);
//void pullUpDnControl(int pin);
void gpiod_chip_close(struct gpiod_chip *chip);
int pins_setup_chip(void);
int pins_setup_gpio(int switch_pin, int led_pin);
int pins_setup_i2c(int sda, int scl);
int digitalRead(int pin);
void digitalWrite(int pin, int level);
void pinMode(int pin, int mode, int value);
void delayMicroseconds(int delay_us);
void delayMs(int delay_ms);

#endif