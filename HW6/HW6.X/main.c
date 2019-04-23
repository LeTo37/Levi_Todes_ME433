#include "Setup.h" //PIC
#include "ili9341.h" //LCD

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
    //Make A4 an output - initially high (GREEN LED)
    TRISAbits.TRISA4 = 0;
    LATAbits.LATA4 = 1;
    __builtin_enable_interrupts();

    SPI1_init();
    LCD_init();

    LCD_clearScreen(ILI9341_DARKGREEN);

    char message[100];
    int number = 0;
    double fps = 0;
    //    sprintf(message, "Hello world %d!", number);
    //
    //    LCD_drawString(message, 10, 10, ILI9341_BLACK, ILI9341_DARKGREEN);
    //start fresh
    _CP0_SET_COUNT(0);

    while (1) {
        _CP0_SET_COUNT(0);
        sprintf(message, "Hello world %3d !", number);
        LCD_drawString(message, 28, 32, ILI9341_WHITE, ILI9341_BLACK);
        LCD_drawProgBar(number, 28, 52, 100, ILI9341_PINK, ILI9341_BLUE);
        fps = 1.0 / (_CP0_GET_COUNT() / 24000000.0);
        sprintf(message, "FPS: %5.2f", fps);
        LCD_drawString(message, 28, 72, ILI9341_WHITE, ILI9341_BLACK);
        number++;
        while (_CP0_GET_COUNT() < 2400000) {
            ;
        }
        if (number == 101) {
            number = 0;
        }
    }

}
