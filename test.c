#include <gpiod.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "soft_i2c.h"
#include "gpio_utility.h"

int main (int argc, char **argv)
{
    int i;
    printf("I2C Bus Emu started \n");
    pins_setup_chip();
    pins_setup_i2c(SDA_PIN, SCL_PIN);
    pins_setup_gpio(BAT_SEL_PIN, BOARD_ON_PIN);
    printf("Start Cycle\n");
    for(i=0; i<2; i++){
        printf("*");
        fflush(stdout);
        digitalWrite(BAT_SEL_PIN, LOW);
        sleep(1);
        digitalWrite(BOARD_ON_PIN, HIGH);
        sleep(1);
        digitalWrite(BAT_SEL_PIN, HIGH);
        sleep(1);
        digitalWrite(BOARD_ON_PIN, LOW);
        sleep(1);
    }
    int val = 0;
    for(i=0; i<5; i++){
        pinMode(SDA_PIN, INPUT, 0);
        sleep(1);
        val = digitalRead(SDA_PIN);
        printf("SDA Input=%d\n", val);

        pinMode(SDA_PIN, OUTPUT, 1);
        printf("SDA Output=1\n");
        sleep(1);

        pinMode(SDA_PIN, OUTPUT, 1);
        printf("SDA Output\n");
        sleep(1);
    }   
}
