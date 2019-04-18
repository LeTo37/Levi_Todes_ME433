#ifndef I2C
#define I2C
// Header file for i2c_master_noint.c
// helps implement use I2C2 as a master without using interrupts

#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

#define I2C_ADDRESS 0b0100010
#define IODIR 0x00  //I/O direction register
#define OLAT 0x0A //Output Latch register
#define GPIO 0x09 //GPIO register

void i2c_master_setup(void);              // set up I2C 1 as a master, at 400 kHz

void i2c_master_start(void);              // send a START signal
void i2c_master_restart(void);            // send a RESTART signal
void i2c_master_send(unsigned char byte); // send a byte (either an address or data)
unsigned char i2c_master_recv(void);      // receive a byte of data
void i2c_master_ack(int val);             // send an ACK (0) or NACK (1)
void i2c_master_stop(void);               // send a stop
void setExpander(char pin, char level);   // write to Expander
char getExpander(void);                   //read expander
void initExpander(void);                  //initialize expander

#endif