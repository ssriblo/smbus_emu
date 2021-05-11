#include <gpiod.h>
#include "gpio_utility.h"

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

/**** GLOBALS ****/
struct gpiod_line *sda_line;
struct gpiod_line *scl_line;
struct gpiod_line *switch_line;
struct gpiod_line *led_line;
struct gpiod_chip *chip;

int pins_setup_chip(int sda, int scl){
    int ret = 0;
    char *chipname = "gpiochip0";
//	chip = gpiod_chip_open_by_name(chipname);
	chip = gpiod_chip_open("/dev/gpiochip0");
	if (!chip) {
		perror("Open chip failed\n");
        ret = -1;
	}
    return ret;
}

int pins_setup_gpio(int switch_pin, int led_pin){
    // Call after pins_setup_chip() !!
	int ret = 0;
    switch_line = gpiod_chip_get_line(chip, switch_pin);
	if (!switch_line) {
		perror("Get switch_line failed\n");
		goto close_chip;
	}

    led_line = gpiod_chip_get_line(chip, led_pin);
	if (!led_line) {
		perror("Get led_line failed\n");
		goto close_chip;
	}

	ret = gpiod_line_request_input(sda_line, CONSUMER);
	if (ret < 0) {
		perror("Request sda_line as input failed\n");
		goto release_line;
    }

	ret = gpiod_line_request_input(led_line, CONSUMER);
	if (ret < 0) {
		perror("Request led_line as input failed\n");
		goto release_line;
    }
    
    release_line:
        gpiod_line_release(switch_line);
        gpiod_line_release(led_line);
    close_chip:
        gpiod_chip_close(chip);        
        ret = -1;
    return ret;
}

int pins_setup_i2c(int sda, int scl){
    // Call after pins_setup_chip() !!
	int ret = 0;
	sda_line = gpiod_chip_get_line(chip, sda);
	if (!sda_line) {
		perror("Get sda_line failed\n");
		goto close_chip;
	}
	scl_line = gpiod_chip_get_line(chip, scl);
	if (!scl_line) {
		perror("Get scl_line failed\n");
		goto close_chip;
	}
	ret = gpiod_line_request_input(sda_line, CONSUMER);
	if (ret < 0) {
		perror("Request sda_line as input failed\n");
		goto release_line;
	}
	ret = gpiod_line_request_input(scl_line, CONSUMER);
	if (ret < 0) {
		perror("Request scl_line as input failed\n");
		goto release_line;
	}
    release_line:
        gpiod_line_release(sda_line);
        gpiod_line_release(scl_line);
    close_chip:
        gpiod_chip_close(chip);
        ret = -1;
    return ret;
}

int digitalRead(int pin){
    struct gpiod_line *line;
    if(SDA_PIN == pin){
        line = sda_line;
    }else{
        line = scl_line;
    }
    int val = gpiod_line_get_value(line);
    return val;
}

void digitalWrite(int pin, int level){   
    struct gpiod_line *line;
    if(SDA_PIN == pin){
        line = sda_line;
    }else{
        line = scl_line;
    }
	gpiod_line_set_value(line, level);
}

void pinMode(int pin, int mode, int value){
/* for SDA line only  */

    if (INPUT == mode) {
        gpiod_line_set_direction_input(sda_line);

    }else {
        gpiod_line_set_direction_output(sda_line, value);
    }

}

void delayMicroseconds(int delay_us){
    // 	delayMicroseconds((1e6/I2C_FREQ)/2);
    usleep(delay_us);
}


