#include "I2C_Setup.h"
#include "imu.h"
#include "ili9341.h"
#include <stdio.h>

char message[100];

void IMU_init() {

    //**********I2C COMMS SETUP**********************
    i2c_master_setup();
    unsigned char who_am_i[1];
    i2c_read_multiple(IMU_ADDRESS, WHO_AM_I, who_am_i, 1);
    sprintf(message, "WHOAMI: %d", who_am_i[0]);
    LCD_drawString(message, 168, 22, ILI9341_WHITE, ILI9341_BLACK);
//    if (who_am_i[0] == 0b01101001) //WHO_AM_I is what it should be
//    {
//        LATAbits.LATA4 = 1;
//    } else {
//        LATAbits.LATA4 = 0;
//    }
    //***********************************************

    //ACCELERATION
    unsigned char setup = 0x82;
    i2c_write(IMU_ADDRESS, CTRL1_XL, setup);

    //GYRO
    setup = 0x88;
    i2c_write(IMU_ADDRESS, CTRL2_G, setup);

    //MULTIPLE REGISTER READ
    setup = 0b00000100;
    i2c_write(IMU_ADDRESS, CTRL3_C, setup);
}

void IMU_read(short *data) {
    unsigned char data_raw[14];
    int i;
    i2c_read_multiple(IMU_ADDRESS, OUT_TEMP_L, data_raw, 14);
    for (i = 0; i < 14; i += 2) {
        data[i / 2] = ((data_raw[i + 1] << 8) | data_raw[i]);
    }
//    LATAbits.LATA4 = !LATAbits.LATA4;
}