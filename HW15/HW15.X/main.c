#include "Setup.h"      //PIC
#include "ili9341.h"    //LCD
#include "PWM.h"    //PWM

#include <math.h>
#include <stdio.h>

int count = 0;
char message[100];

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
    __builtin_enable_interrupts();

    SPI1_init();
    LCD_init();
    PWM_init();

    LCD_clearScreen(ILI9341_WHITE);

    //start fresh
    _CP0_SET_COUNT(0);

    unsigned char red[240];
    unsigned char blue[240];
    unsigned char green[240];
    unsigned char i;
    for (i = 0; i < 240; i++) {
       red[i] = i;
       if (i < 110){
           blue[i] = i;
       }
       else{
           blue[i] = 240 -i;
       }
       green[i] = 240 -i;
    }
    
    LCD_plot_high_three(red,60,ILI9341_RED);
    
    LCD_plot_high_three(blue,180,ILI9341_BLUE);
    
    LCD_plot_high_three(green,280,ILI9341_GREEN);

    
    while (1) {
        //        start fresh
        

    }

}

void __ISR(_TIMER_3_VECTOR, IPL5SOFT) Timer3ISR(void) {
    count++;
    if (count == 100) {
        count = 0;
        LATBbits.LATB2 = !LATBbits.LATB2;
        OC1RS = 0;
    }
    OC1RS += 24;
    IFS0bits.T3IF = 0;
}