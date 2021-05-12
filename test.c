#include <gpiod.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "soft_i2c.h"
#include "gpio_utility.h"



int main (int argc, char **argv)
{
    printf("I2C Bus Emu started \n");
    pins_setup_chip();
    pins_setup_gpio(SWITCH_PIN, LED_PIN);
    pins_setup_i2c(SDA_PIN, SCL_PIN);

}
