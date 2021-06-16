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
static struct gpiod_line *bat_sel_line;
static struct gpiod_line *board_on_line;
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

void stopAll(void){
    gpiod_line_release(sda_line);
    gpiod_line_release(scl_line);
    gpiod_chip_close(chip);
}

int pins_setup_chip(void){
    int ret = 0;
    char *chipname = "gpiochip0";
//	chip = gpiod_chip_open_by_name(chipname);
	chip = gpiod_chip_open("/dev/gpiochip0");
	if (!chip) {
		printf("Open chip failed\n");
        ret = -1;
	}
    return ret;
}

int pins_setup_gpio(int bat_sel_pin, int board_on_pin){
    // Call after pins_setup_chip() !!
	int ret = 0;
    bat_sel_line = gpiod_chip_get_line(chip, bat_sel_pin);
	if (!bat_sel_line) {
		printf("Get bat_sel_line failed\n");
		goto close_chip;
	}

    board_on_line = gpiod_chip_get_line(chip, board_on_pin);
	if (!board_on_line) {
		printf("Get board_on_line failed\n");
		goto close_chip;
	}

    printf("setOutput(bat_sel_line) \n");
    ret = setOutput(bat_sel_line, 0, BAT_SEL_DEFAULT_VAL);
	if (ret < 0) {
		printf("Request bat_sel_pin as Output failed\n");
		goto release_line;
    }

    printf("setOutput(board_on_line) \n");
    ret = setOutput(board_on_line, 0, BOARD_ON_DEFAULT_VAL);
	if (ret < 0) {
		printf("Request board_on_line as Output failed\n");
		goto release_line;
    }
    printf("gpio setup done\n");
    return ret;

    release_line:
        gpiod_line_release(bat_sel_line);
        gpiod_line_release(board_on_line);
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
		printf("Get sda_line failed\n");
		goto close_chip;
	}
	scl_line = gpiod_chip_get_line(chip, scl);
	if (!scl_line) {
		printf("Get scl_line failed\n");
		goto close_chip;
	}
    ret = setInput(sda_line);
	if (ret < 0) {
		printf("Request sda_line as input failed\n");
		goto release_line;
	}
    ret = setInput(scl_line);
	if (ret < 0) {
		printf("Request scl_line as input failed\n");
		goto release_line;
	}
    printf("i2c setup done\n");
    return ret;

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
    switch (pin)
    {
    case SDA_PIN:
        line = sda_line;
        break;
    case SCL_PIN:
        line = scl_line;
        break;
    case BAT_SEL_PIN:
        line = bat_sel_line;
        break;
    case BOARD_ON_PIN:
        line = board_on_line;
        break;   
    default:
        break;
    }
	int p = gpiod_line_offset(line);
    if (DEBUG1) printf("digitalWrite() pin=%d level=%d\n", p, level);
    gpiod_line_set_value(line, level);
}

void pinMode(int pin, int newDirection, int value){
    struct gpiod_line *line;
    if(SDA_PIN == pin){
        line = sda_line;
    }else{
        line = scl_line;
    }
    int direction = gpiod_line_direction(line);
    //if(newDirection != direction) {
        gpiod_line_release(line);
    //}  

    if (GPIOD_LINE_DIRECTION_INPUT == newDirection) {
        setInput(line);
    }else {
        setOutput(line, GPIOD_LINE_REQUEST_FLAG_OPEN_DRAIN, value);
    }
}

int setInput(struct gpiod_line *line){
    int ret;
    // ?? GPIOD_LINE_REQUEST_FLAG_ACTIVE_LOW ??
    ret = gpiod_line_request_input_flags(line, CONSUMER, 0);
    return ret;
}

int setOutput(struct gpiod_line *line, int flags, int value){
    int ret;
    // ?? GPIOD_LINE_REQUEST_FLAG_ACTIVE_LOW ??
    int pin = gpiod_line_offset(line);
    if (DEBUG1) printf("setOutput() pin= %d flag= %d\n", pin, flags);
    ret = gpiod_line_request_output_flags(line, CONSUMER, flags, value);
    if(ret < 0){
        printf("setOutput() FAILED \n");
    }
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

