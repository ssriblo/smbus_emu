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
    pins_setup_gpio(BAT_SEL, BOARD_ON);
//    pins_setup_i2c(SDA_PIN, SCL_PIN);
    while(true){
        digitalWrite(BAT_SEL, LOW);
        sleep(1);
        digitalWrite(BOARD_ON, HIGH);
        sleep(1);
        digitalWrite(BAT_SEL, HIGH);
        sleep(1);
        digitalWrite(BOARD_ON, LOW);
        sleep(1);
    }

}
