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
    pins_setup_i2c(SDA_PIN, SCL_PIN);
    pins_setup_gpio(BAT_SEL_PIN, BOARD_ON_PIN);
    printf("Start Cycle\n");
    while(true){
        printf("*");
        fflush(stdout);
        digitalWrite(BAT_SEL_PIN, LOW);
        sleep(1);
//        digitalWrite(BOARD_ON_PIN, HIGH);
//        sleep(1);
        digitalWrite(BAT_SEL_PIN, HIGH);
        sleep(1);
//        digitalWrite(BOARD_ON_PIN, LOW);
//        sleep(1);
    }

}
