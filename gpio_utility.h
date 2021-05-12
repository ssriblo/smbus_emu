#ifndef _GPIO_UTILITY_H
#define _GPIO_UTILITY_H

#include <gpiod.h>



#define INPUT   0
#define OUTPUT  1

#define LOW     0
#define HIGH    1

#define SDA_PIN         19 
#define SCL_PIN         21
#define BAT_SEL_PIN     20
#define BOARD_ON_PIN    18

#define BAT_SEL_DEFAULT_VAL     LOW
#define BOARD_ON_DEFAULT_VAL    LOW

void gpiod_chip_close(struct gpiod_chip *chip);
int pins_setup_chip(void);
int pins_setup_gpio(int switch_pin, int led_pin);
int pins_setup_i2c(int sda, int scl);
int digitalRead(int pin);
void digitalWrite(int pin, int level);
void pinMode(int pin, int mode, int value);
void delayMicroseconds(int delay_us);
void delayMs(int delay_ms);
int setInput(struct gpiod_line *line);
int setOutput(struct gpiod_line *line, int flags, int value);

#endif