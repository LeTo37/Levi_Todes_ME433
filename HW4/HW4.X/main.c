#include "Setup.h" //PIC
#include "SPI_Setup.h" //SPI

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


    //SPI
    initSPI1();
    char channel = 0;
    int voltage = 0;
    setVoltage(channel, voltage);
    //Sine wave - 10Hz - VoutA
    int i = 0;
    int sine[1000];
    for (i = 0; i < 1000; i++) {
        sine[i] = 512 + 512 * sin(i * 2 * 3.1415 * 10 / 1000 *10);
    }

    //Triangle wave - 5Hz - VoutB
    i = 0;
    int tri[1000];
    for (i = 0; i < 1000; i++) {
        tri[i] = ((2 * (512 / 3.1415)) * asin(sin(2 * (3.1415 / 0.2) *i)));
    }

    //start fresh
    _CP0_SET_COUNT(0);

    while (1) {

        if (i > 999) {
            i = 0;
        }
        //1kHz writing
        if (_CP0_GET_COUNT() > 24000) {
            setVoltage(0, sine[i]);
            setVoltage(1, tri[i]);
            i++;
            _CP0_SET_COUNT(0);
        }

    }
}