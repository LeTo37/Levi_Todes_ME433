#include "Setup.h" //PIC
#include "I2C_Setup.h" //I2C

#include <math.h>

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

    //I2C
    i2c_master_setup();
    initExpander();

    //start fresh
    _CP0_SET_COUNT(0);

    while (1) {
        char read = 0;
        read = getExpander();
        if (((read>>6)&0x01) == 1) {
            setExpander(OLAT, 0x00); //LED OFF
        } else {
            setExpander(OLAT, 0x01); //LED ON
        }

    }
}