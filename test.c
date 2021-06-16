#include <gpiod.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "soft_i2c.h"
#include "gpio_utility.h"

int readWordSMBus(int adr, int command);

int main (int argc, char **argv)
{
    int i;
    printf("I2C Bus Emu started \n");
    pins_setup_chip();
    pins_setup_i2c(SDA_PIN, SCL_PIN);
    pins_setup_gpio(BAT_SEL_PIN, BOARD_ON_PIN);

#if 0
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
    for(i=0; i<2; i++){
        printf("***** SDA INPUT *****\n");
        pinMode(SDA_PIN, INPUT, 0);
        sleep(1);
        val = digitalRead(SDA_PIN);
        printf("SDA Input=%d\n", val);

        printf("***** SDA OUTPUT=1 *****\n");
        pinMode(SDA_PIN, OUTPUT, 1);
        sleep(1);

        printf("***** SDA OUTPUT=0 *****\n");
        pinMode(SDA_PIN, OUTPUT, 0);
        sleep(1);
    }
#endif
#if 0
    /* From i2cli.c */
	int addr;
	printf("I2C scan started\n");
	i2c_t i2c = i2c_init(SCL_PIN, SDA_PIN);
    for (addr = 0; addr < 128; addr++) {
        i2c_start(i2c);
        if (i2c_send_byte(i2c, addr << 1 | I2C_READ) == I2C_ACK)
            printf (" * Device found at %0xh\n", addr);
        i2c_stop(i2c);
	}
    printf ("** Scan finished **\n");
#endif
#if 0
    /* From i2cli.c */
	int addr;
	i2c_t i2c = i2c_init(SCL_PIN, SDA_PIN);
    for (addr = 0; addr < 128; addr++) {
        i2c_start(i2c);
        // Some devices best scanned using read, other using write
        if (i2c_send_byte(i2c, addr << 1 | I2C_WRITE) == I2C_ACK)
            printf (" * Device found at %02xh  (R: %02x, W: %02x)\n",
                addr,
                addr << 1 | 1,
                addr << 1 | 0);
        i2c_stop(i2c);
    }
#endif
//#if 0
    int val;
    val = readWordSMBus(0x0B, 0x1C);
    printf("VAL=%d\n", val);
//#endif
}

int readWordSMBus(int adr, int command){
    int val_1=0, val_2=0;
	i2c_t i2c = i2c_init(SCL_PIN, SDA_PIN);
    i2c_start(i2c);
    printf("start i2c finished\n");
    if (i2c_send_byte(i2c, adr << 1 | I2C_WRITE) == I2C_ACK){
        i2c_send_byte(i2c, command);
        i2c_start(i2c);
        i2c_send_byte(i2c, adr << 1 | I2C_READ);
        val_1 = i2c_read_byte(i2c);
        printf("val_1=%d\n", val_1);
        i2c_send_bit(i2c, I2C_ACK);
        val_2 = i2c_read_byte(i2c);
        printf("val_2=%d\n", val_2);
        i2c_send_bit(i2c, I2C_NACK);
    }
    stopAll();
    return (val_2 << 8) + val_1;
}
