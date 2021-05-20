# smbus_emu
smbus emulator based on libgpiod library

Works well with I2C slave devices. 

Failed with smart battery SMBus interface. The issue - bit rate about 1 kHz, but SMBus minimum bit rate is 10 kHz. 


