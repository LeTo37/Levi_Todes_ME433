#include "Setup.h"      //PIC
#include "ili9341.h"    //LCD

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
    LCD_drawButton(155, 135, 30, 50, ILI9341_MAGENTA, ILI9341_CYAN, "I+ +");
    LCD_drawButton(155, 200, 30, 50, ILI9341_MAGENTA, ILI9341_CYAN, "I- -");
    char message[100];
    double fps = 0;
    unsigned short X = 0, Y = 0;
    int i = 0;

    int Z = 0;
    unsigned char Zplus = 0, Zminus = 0;

    //start fresh
    _CP0_SET_COUNT(0);

    while (1) {
        //        start fresh
        _CP0_SET_COUNT(0);

        LCD_drawPixel(120, 160, ILI9341_ORANGE);

        touch_read(&X, &Y, &Z);
        sprintf(message, "Xraw: %4d", (X));
        LCD_drawString(message, 28, 22, ILI9341_WHITE, ILI9341_BLACK);
        sprintf(message, "Yraw: %4d", (Y));
        LCD_drawString(message, 28, 42, ILI9341_WHITE, ILI9341_BLACK);
        sprintf(message, "Z: %4d", (Z));
        LCD_drawString(message, 28, 62, ILI9341_WHITE, ILI9341_BLACK);
        sprintf(message, "I= %3d", (i));
        LCD_drawString(message, 130, 163, ILI9341_GREENYELLOW, ILI9341_BLACK);
        X = X*240/4095;
        Y = (4095-Y)*320/4095;
        if (Z > 1000) {
            sprintf(message, "X: %4d", (X));
            LCD_drawString(message, 28, 142, ILI9341_PURPLE, ILI9341_LIGHTGREY);
            sprintf(message, "Y: %4d", (Y));
            LCD_drawString(message, 28, 162, ILI9341_PURPLE, ILI9341_LIGHTGREY);
        }
        else {
            sprintf(message, "X: %4d", (0));
            LCD_drawString(message, 28, 142, ILI9341_PURPLE, ILI9341_LIGHTGREY);
            sprintf(message, "Y: %4d", (0));
            LCD_drawString(message, 28, 162, ILI9341_PURPLE, ILI9341_LIGHTGREY);
        }

        if (Zplus && Z < 1000) {
            i++;
            Zplus = 0;
        }
        if (Zminus && Z < 1000) {
            i--;
            Zminus = 0;
        }

        if (X > 130 && X < 170) {
            if (Y > 110 && Y < 160) {
                if (Z > 1000) {
                    Zplus = 1;
                }
            } else if (Y > 175 && Y < 225) {
                if (Z > 1000) {
                    Zminus = 1;
                }
            }
        }



        //        sprintf(message, "Zprev: %4d", (Zprev));
        //        LCD_drawString(message, 28, 162, ILI9341_WHITE, ILI9341_BLACK);

        fps = 1.0 / (_CP0_GET_COUNT() / 24000000.0);
        sprintf(message, "FPS: %5.2f", fps);
        LCD_drawString(message, 168, 42, ILI9341_WHITE, ILI9341_BLACK);
        while (_CP0_GET_COUNT() < (2400000)) {
            ;
        }
        //        LATAbits.LATA4 = !LATAbits.LATA4;
    }

}
