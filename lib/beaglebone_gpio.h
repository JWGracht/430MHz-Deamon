#ifndef _BEAGLEBONE_GPIO_H_
#define _BEAGLEBONE_GPIO_H_

#define GPIO1_START_ADDR 0x4804C000
#define GPIO1_END_ADDR 0x4804DFFF
#define GPIO1_SIZE (GPIO1_END_ADDR - GPIO1_START_ADDR)
#define GPIO_OE 0x134
#define GPIO_DATAIN 0x138
#define GPIO_SETDATAOUT 0x194
#define GPIO_CLEARDATAOUT 0x190


#define PIN (1<<12)


#endif
