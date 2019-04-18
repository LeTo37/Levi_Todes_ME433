#ifndef SPI
#define SPI

#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

#define CS LATBbits.LATB7  //CS pin

unsigned char spi_io(unsigned char o);
void initSPI1(void);
void setVoltage(char channel, int voltage);

#endif