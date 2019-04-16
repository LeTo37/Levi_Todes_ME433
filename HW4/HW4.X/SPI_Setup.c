#include "SPI_Setup.h"

// send a byte via spi and return the response

unsigned char spi_io(unsigned char o) {
    SPI1BUF = o;
    while (!SPI1STATbits.SPIRBF) { // wait to receive the byte
        ;
    }
    return SPI1BUF;
}

void initSPI1() {
    //CS Pin to B7 - digital out
    TRISBbits.TRISB7 = 0;
    CS = 1; //start high

    //Setup B8 for SDO1
    RPB8Rbits.RPB8R = 0b0011;

    //SPI setup
    SPI1CON = 0; // turn off the spi module and reset it
    SPI1BUF; // clear the rx buffer by reading from it
    SPI1BRG = 0x3; // baud rate to 10MHz [SPI1BRG = (80000000/(2*desired))-1]
    SPI1STATbits.SPIROV = 0; // clear the overflow bit
    SPI1CONbits.CKE = 1; // data changes when clock goes from hi to lo (since CKP is 0)
    SPI1CONbits.MSTEN = 1; // master operation
    SPI1CONbits.ON = 1; // turn on spi 1


}

void setVoltage(char channel, int voltage) {
    unsigned short message = 0;
    //    unsigned char v = (unsigned char) voltage;
    message = message | (channel << 15);
    message = message | (0x7 << 12);
    message = message | voltage;

    CS = 0;
    spi_io((message & 0xFF00) >> 8);
    spi_io(message & 0x00FF);
    CS = 1;
}