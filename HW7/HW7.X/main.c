#include "Setup.h"      //PIC
#include "ili9341.h"    //LCD
#include "I2C_Setup.h"  //I2C
#include "imu.h"        //IMU

#include <math.h>
#include <stdio.h>

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
    //Make B4 an input (USER)
    TRISBbits.TRISB4 = 1;
    //Make A4 an output - initially low (GREEN LED)
    TRISAbits.TRISA4 = 0;
    LATAbits.LATA4 = 0;
    __builtin_enable_interrupts();

    SPI1_init();
    LCD_init();

    LCD_clearScreen(ILI9341_DARKGREEN);

    IMU_init();
    //        LATAbits.LATA4 = 1;
    short data[7];

    float gyroX,gyroY,gyroZ;
    char message[100];
    double fps = 0;

    //start fresh
    _CP0_SET_COUNT(0);

    while (1) {
        //        start fresh
        _CP0_SET_COUNT(0);
        IMU_read(data);
        gyroX = ((float)(data[5]))*(100.0/32768.0);
        gyroY = ((float)(data[4]))*(100.0/32768.0);
//        gyroZ = ((float)(data[6]))*(100.0/32768.0);
        
        sprintf(message, "X: %5.2f", (gyroX));
        LCD_drawString(message, 28, 22, ILI9341_WHITE, ILI9341_BLACK);
        sprintf(message, "Y: %5.2f", (gyroY));
        LCD_drawString(message, 28, 42, ILI9341_WHITE, ILI9341_BLACK);
//        sprintf(message, "Z: %5.2f", (gyroZ));
//        LCD_drawString(message, 28, 62, ILI9341_WHITE, ILI9341_BLACK);

        LCD_drawProgBar_h(gyroY, 120, 160, 100, ILI9341_PINK, ILI9341_BLUE);
        LCD_drawProgBar_v(gyroX, 120, 160, 100, ILI9341_PINK, ILI9341_BLUE);
        //        LCD_drawProgBar(number, 120, 160, 100, ILI9341_PINK, ILI9341_BLUE);
        
        fps = 1.0 / (_CP0_GET_COUNT() / 24000000.0);
        sprintf(message, "FPS: %5.2f", fps);
        LCD_drawString(message, 168, 42, ILI9341_WHITE, ILI9341_BLACK);
        while (_CP0_GET_COUNT() < (2400000)) {
            ;
        }
        //        LATAbits.LATA4 = !LATAbits.LATA4;
    }

}
