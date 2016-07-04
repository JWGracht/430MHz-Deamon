#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include "lib/beaglebone_gpio.h"
#include "lib/SimpleGPIO.h"

#define SERVICE_FIFO_NAME "/tmp/transmitterfifo"
static int fifo_filestream = -1;

int create_fifo()
{
  int result;
  const char *pipe_msg = "\n";
  printf("Making FIFO...\n");
  unlink(SERVICE_FIFO_NAME);
  result = mkfifo(SERVICE_FIFO_NAME, 0666);
  if (result == 0)
  {
    printf("New FIFO created: %s\n", SERVICE_FIFO_NAME);
  }
  printf("Process %d opening FIFO %s\n", getpid(), SERVICE_FIFO_NAME);
  result = open(SERVICE_FIFO_NAME, O_RDWR);
  if (result != -1)
  {
    printf("Opened FIFO: %i\n", result);
  }
  // write to pipo so read in function web_server will not start in end of life if the python proces is not yet up
  write(fifo_filestream, pipe_msg, strlen(pipe_msg) + 1);
  
  return result;
}

void close_fifo()
{
  (void)close(fifo_filestream);
}

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
    fifo_filestream = create_fifo();
    
    printf("Start program\n");
    while(1) {
      unsigned char rx_buffer[16384];
      int rx_length = read(fifo_filestream, (void*)rx_buffer, sizeof(rx_buffer));
      
      if (rx_length < 0)
      {
	    printf("rx_length<0\n");
	    close_fifo();
	    fifo_filestream = create_fifo();
      }
      else if (rx_length == 0)
      {
        // No data waiting
      }
      else
      {
        rx_buffer[rx_length] = '\0';
        printf("FIFO %i bytes read : %s\n", rx_length, rx_buffer);
        
        int i;
        for (i = 0; i < rx_length; i++){
          if (rx_buffer[i] == '1') {
            *gpio_setdataout_addr= PIN;
          } else {
            *gpio_cleardataout_addr = PIN;
          }
          
          usleep(30);
        }
      }
    }
    close_fifo();
    (void)close(fd);
    return 0;
}
