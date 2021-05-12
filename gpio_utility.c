#include <gpiod.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "soft_i2c.h"
#include "gpio_utility.h"

#ifndef	CONSUMER
#define	CONSUMER	"Consumer"
#endif

/**** GLOBALS ****/
static struct gpiod_line *sda_line;
static struct gpiod_line *scl_line;
static struct gpiod_line *switch_line;
static struct gpiod_line *led_line;
static struct gpiod_chip *chip;

/***********************************************************
 * Version: 0.1
 * 
 * ToDo:
 * PullUp Resistor ???
 * 
************************************************************/


void gpiod_chip_close(struct gpiod_chip *chip){
	printf("CHIP CLOSED \n");
}

int pins_setup_chip(void){
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

    ret = setOutput(switch_line, SWITCH_DEFAULT_VAL);
	if (ret < 0) {
		perror("Request switch_pin as input failed\n");
		goto release_line;
    }

    ret = setOutput(led_line, LED_DEFAULT_VAL);
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
    ret = setInput(sda_line);
	if (ret < 0) {
		perror("Request sda_line as input failed\n");
		goto release_line;
	}
    ret = setInput(scl_line);
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
    struct gpiod_line *line;
    if(SDA_PIN == pin){
        line = sda_line;
    }else{
        line = scl_line;
    }
    if (INPUT == mode) {
        setInput(line);
    }else {
        setOutput(line, value);
    }
}

int setInput(struct gpiod_line *line){
    int ret;
    // ?? GPIOD_LINE_REQUEST_FLAG_ACTIVE_LOW ??
    ret = gpiod_line_request_input_flags(line, CONSUMER, 
        GPIOD_LINE_REQUEST_FLAG_OPEN_DRAIN | 
        GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP);
    return ret;
}

int setOutput(struct gpiod_line *line, int value){
    int ret;
    // ?? GPIOD_LINE_REQUEST_FLAG_ACTIVE_LOW ??
    gpiod_line_request_output_flags(line, CONSUMER, 
        GPIOD_LINE_REQUEST_FLAG_OPEN_DRAIN |
        GPIOD_LINE_REQUEST_FLAG_BIAS_PULL_UP, 
        value);
    return ret;
}

void delayMicroseconds(int delay_us){
    // 	delayMicroseconds((1e6/I2C_FREQ)/2);
    usleep(delay_us);
}

void delayMs(int delay_ms){
    for(int i=0; i<1000; i++){
        usleep(1000);
    }
}

