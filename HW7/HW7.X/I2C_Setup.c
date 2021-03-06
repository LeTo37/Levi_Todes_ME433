#include "I2C_Setup.h"
#include "imu.h"

// I2C Master utilities, 100 kHz, using polling rather than interrupts
// The functions must be callled in the correct order as per the I2C protocol
// Change I2C2 to the I2C channel you are using
// I2C pins need pull-up resistors, 2k-10k

void i2c_master_setup(void) {
    ANSELBbits.ANSB2 = 0; //make B2 and B3 digital
    ANSELBbits.ANSB3 = 0;
    I2C2BRG = 53; // I2CBRG = [1/(2*Fsck) - PGD]*Pbclk - 2 
    //Fsck = 400kHz, Pbclk = 48MHz
    // look up PGD for your PIC32 -104ns
    I2C2CONbits.ON = 1; // turn on the I2C2 module
}

// Start a transmission on the I2C bus

void i2c_master_start(void) {
    I2C2CONbits.SEN = 1; // send the start bit
    while (I2C2CONbits.SEN) {
        ;
    } // wait for the start bit to be sent
}

void i2c_master_restart(void) {
    I2C2CONbits.RSEN = 1; // send a restart 
    while (I2C2CONbits.RSEN) {
        ;
    } // wait for the restart to clear
}

void i2c_master_send(unsigned char byte) { // send a byte to slave
    I2C2TRN = byte; // if an address, bit 0 = 0 for write, 1 for read
    while (I2C2STATbits.TRSTAT) {
        ;
    } // wait for the transmission to finish
    if (I2C2STATbits.ACKSTAT) { // if this is high, slave has not acknowledged
        // ("I2C2 Master: failed to receive ACK\r\n");
    }
}

unsigned char i2c_master_recv(void) { // receive a byte from the slave
    I2C2CONbits.RCEN = 1; // start receiving data
    while (!I2C2STATbits.RBF) {
        ;
    } // wait to receive the data
    return I2C2RCV; // read and return the data
}

void i2c_master_ack(int val) { // sends ACK = 0 (slave should send another byte)
    // or NACK = 1 (no more bytes requested from slave)
    I2C2CONbits.ACKDT = val; // store ACK/NACK in ACKDT
    I2C2CONbits.ACKEN = 1; // send ACKDT
    while (I2C2CONbits.ACKEN) {
        ;
    } // wait for ACK/NACK to be sent
}

void i2c_master_stop(void) { // send a STOP:
    I2C2CONbits.PEN = 1; // comm is complete and master relinquishes bus
    while (I2C2CONbits.PEN) {
        ;
    } // wait for STOP to complete
}

void i2c_write(unsigned char address, unsigned char reg, unsigned char data) { //write to expander
    //    LATAbits.LATA4 = 1;
    i2c_master_start();
    i2c_master_send(0 | (address << 1)); //write
    i2c_master_send(reg);
    i2c_master_send(data);
//    i2c_master_ack(1);
    i2c_master_stop();
}

char i2c_read_multiple(unsigned char address, unsigned char reg, unsigned char * data, int length) { //read expander
    //first send register to read from
    i2c_master_start();
    i2c_master_send((address << 1) | 0); //write
    i2c_master_send(reg);
    i2c_master_restart(); //restart
    //now read from register
    i2c_master_send((address << 1) | 1); //read
    int i;
    for (i = 0; i < length; i++) {
        data[i] = i2c_master_recv();
        if (i < (length - 1)) {
            i2c_master_ack(0);
        } else {
            i2c_master_ack(1);
        }
    }
    i2c_master_stop();
}