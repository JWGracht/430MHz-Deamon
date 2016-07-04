#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include "lib/beaglebone_gpio.h"

#define SERVICE_FIFO_NAME "/tmp/transmitterfifo"

int main(int argc, char *argv[]) {
    volatile void *gpio_addr = NULL;
    volatile unsigned int *gpio_oe_addr = NULL;
    volatile unsigned int *gpio_setdataout_addr = NULL;
    volatile unsigned int *gpio_cleardataout_addr = NULL;
    unsigned int reg;
    const char *pipe_msg = "\n";
  
    int fd = open("/dev/mem", O_RDWR);

    printf("Mapping %X - %X (size: %X)\n", GPIO1_START_ADDR, 
GPIO1_END_ADDR, GPIO1_SIZE);

    gpio_addr = mmap(0, GPIO1_SIZE, PROT_READ | PROT_WRITE, 
MAP_SHARED, fd, GPIO1_START_ADDR);

    gpio_oe_addr = gpio_addr + GPIO_OE;
    gpio_setdataout_addr = gpio_addr + GPIO_SETDATAOUT;
    gpio_cleardataout_addr = gpio_addr + GPIO_CLEARDATAOUT;

    if(gpio_addr == MAP_FAILED) {
        printf("Unable to map GPIO\n");
        exit(1);
    }
    printf("GPIO mapped to %p\n", gpio_addr);
    printf("GPIO OE mapped to %p\n", gpio_oe_addr);
    printf("GPIO SETDATAOUTADDR mapped to %p\n", 
gpio_setdataout_addr);
    printf("GPIO CLEARDATAOUT mapped to %p\n", 
gpio_cleardataout_addr);

    reg = *gpio_oe_addr;
    printf("GPIO1 configuration: %X\n", reg);
    reg = reg & (0xFFFFFFFF - PIN);
    *gpio_oe_addr = reg;
    printf("GPIO1 configuration: %X\n", reg);
    
    printf("Making FIFO...\n");
    unlink(SERVICE_FIFO_NAME);
    
    int result = mkfifo(SERVICE_FIFO_NAME, 0666);
    if (result == 0)
    {
      printf("New FIFO created: %s\n", CANNON_SERVICE_FIFO_NAME);
    }
    
    if (result != -1)
    {
      printf("Opened FIFO: %i\n", result);
    }
    // write to pipo so read in function will not start in end of life
    write(result, pipe_msg, strlen(pipe_msg) + 1);
    
    printf("Start program\n");
    while(1) {
      unsigned char rx_buffer[256];
      int rx_length = read(result, (void*)rx_buffer, sizeof(rx_buffer));
      
      if (rx_length < 0)
      {

      }
      else if (rx_length == 0)
      {
        // No data waiting
      }
      else
      {
        rx_buffer[rx_length] = '\0';
        
        for (i = 0; i < rx_length; i++){
          if (rx_buffer[i] == "1") {
            *gpio_setdataout_addr= PIN;
          } else {
            *gpio_cleardataout_addr = PIN;
          }
          
          sleep(0.000030);
        }
      }
    }
    (void)close(result);
    (void)close(fd);
    return 0;
}
