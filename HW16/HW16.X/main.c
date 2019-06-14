#include "Setup.h"      //PIC
#include "ili9341.h"    //LCD
#include "PWM.h"    //PWM
#include "ov7670.h" // CAMERA
#include "i2c_master_noint.h" // i2c_camera

#include <math.h>
#include <stdio.h>

int count = 0;
char message[100];

int shootcount = 0;

int main() {

    __builtin_disable_interrupts();
    // set the CP0 CONFIG register to indicate that kseg0 is cacheable (0x3)
    __builtin_mtc0(_CP0_CONFIG, _CP0_CONFIG_SELECT, 0xa4210583);
    // 0 data RAM access wait states
    BMXCONbits.BMXWSDRM = 0x0;
    // enable multi vector interrupts
    INTCONbits.MVEC = 0x1;
    // disable JTAG to get pins back
    DDPCONbits.JTAGEN = 0;


    ANSELA = 0;
    ANSELB = 0;
    SPI1_init();
    LCD_init();
    PWM_init();

    LCD_clearScreen(ILI9341_RED);
    i2c_master_setup();
    ov7670_setup();
    CannonInit();
    __builtin_enable_interrupts();

    int I = 0;
    char message[100];
    unsigned char d[2000]; // this is the raw camera data, both brightness and color
    unsigned char bright[1000]; // this is for just the brightness data

    //startup delay
    _CP0_SET_COUNT(0);
    while (_CP0_GET_COUNT() < 47000000) {

    }

//    int rightleft = 3;

    while (1) {
        _CP0_SET_COUNT(0);
        while (_CP0_GET_COUNT() < 4000000) {

        }
        setDutyLeft(0, 1);
        setDutyRight(0, 1);

        I++;
        sprintf(message, "I = %d   ", I);
        LCD_drawString(message, 140, 175, ILI9341_GREENYELLOW, ILI9341_BLACK);

        // vertical read
        int c = ov7670_count_vert(d);
        sprintf(message, "c = %d   ", c);
        LCD_drawString(message, 140, 195, ILI9341_GREENYELLOW, ILI9341_BLACK);

        int x = 0, x2 = 0;
        int y = 0;
        int dim = 0;
        for (x = 0; x < c / 2; x++, x2 = x2 + 2) {
            dim = d[x2] >> 3;
            bright[x] = d[x2];
            for (y = 0; y < 32; y++) {
                if (y == dim) {
                    LCD_drawPixel(x + 30, y + 250, ILI9341_BLACK);
                } else {
                    LCD_drawPixel(x + 30, y + 250, ILI9341_WHITE);
                }
            }
        }
        // at this point, bright has c/2 values in it, each value is a brightness of a pixel
        // loop through bright and calculate where the middle of the dip or bump is
        // then set the motor speed and direction to follow the line
        int i = 0;
        int sum = 0;
        int sumR = 0;
        int com = 0;
        int avg = 0;
        // find the average brightness
        for (i = 0; i < c / 2; i++) {
            sum = sum + bright[i];
        }
        avg = sum / c / 2;
        // threshold and center of mass calculation
        sum = 0;
        for (i = 0; i < c / 2; i++) {
            if (bright[i] < avg) {
                // count this pixel
                LCD_drawPixel(i, 80, ILI9341_BLUE); // visualize which pixels we're counting
                sum = sum + 255;
                sumR = sumR + 255 * i;
            } else {
                LCD_drawPixel(i, 80, ILI9341_WHITE);
                // don't count this pixel
            }
        }
        // only use com if the camera saw some data
        if (sum > 10) {
            com = sumR / sum;
        } else {
            com = c / 2 / 2;
        }
        // draw the center of mass as a bar
        for (y = 0; y < 32; y++) {
            LCD_drawPixel(com, y + 50, ILI9341_RED);
        }
        int speed = 0;
        int e = 0;
        // try to keep com at c/2/2 using the motors
        //        DIR1 = 1; // depending on your motor directions these might be different
        //        DIR2 = 1;
        // if com is less than c/2/2, then slow down the left motor, full speed right motor
        // if com is more than c/2/2, then slow down right motor, full speed left motor
        // things to play with: the slope of the line, the value that determines when the motor is not full speed
        if (com < (c / 2 / 2 - 20)) {
            e = c / 2 / 2 - com;
            setDutyRight(40, 1);
            setDutyLeft(10, 0);
//            rightleft--;
        } else if ((com > (c / 2 / 2 + 20))) {
            e = com - c / 2 / 2;
            setDutyLeft(40, 1);
            setDutyRight(10, 0);
//            rightleft++;
        } else {
            //            if (rightleft > 15) {
            //                setDutyLeft(20, 1);
            //                setDutyRight(15, 0);
            //            } else if (rightleft < -10) {
            //                setDutyLeft(15, 0);
            //                setDutyRight(20, 1);
            //            } else {
            setDutyLeft(50, 1);
            setDutyRight(50, 1);
            //            }


        }


        shootcount++;
        if (shootcount == 100) {
            ShootLeftCannon();
        }
        if (shootcount == 200) {
            ShootRightCannon();
            shootcount = 0;
        }

    }

}

