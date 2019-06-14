#include"PWM.h"

void PWM_init(void) {
    //Open pins for OC1 and OC2
    RPB15Rbits.RPB15R = 0b0101; //Set B15 as OC1 (PIN 2)
    RPA4Rbits.RPA4R = 0b0101; //Set A4 as OC4 (PIN 17)

    //    //***************TIMER 2 PWM********************
    //    T2CONbits.TCKPS = 0; //Prescaler N=1
    //    // period = (PR2+1) * N * 20.83 ns
    //    PR2 = 2399; //With prescaler of 1 give a 20 kHz signal
    //    TMR2 = 0; //reset the timer  
    //    T2CONbits.ON = 1; //Turn on the timer
    //    //**********************************************

    //***************TIMER 3 PWM********************
    T3CONbits.TCKPS = 0; //Prescaler N=1
    // period = (PR2+1) * N * 20.83 ns
    PR3 = 2399; //With prescaler of 1 give a 20 kHz signal
    TMR3 = 0; //reset the timer  
    T3CONbits.ON = 1; //Turn on the timer
    //********************

    //    //***************TIMER 3 Interrupt**************
    //    T3CONbits.TCKPS = 6; //Prescaler N=64
    //    // period = (PR3+1) * N * 20.83 ns
    //    PR3 = 7500; //With prescaler of 1 give a 100 Hz signal
    //    TMR3 = 0; //reset the timer  
    //    T3CONbits.ON = 1; //Turn on the timer
    //    IPC3bits.T3IP = 5; //priority
    //    IPC3bits.T3IS = 0; //sub-priority       
    //    IFS0bits.T3IF = 0; //int flag
    //    IEC0bits.T3IE = 1; //enable int
    //    //**********************************************

    //*************PHASE DIRECTIONS******************
    TRISAbits.TRISA10 = 0; //set as output
    DIR1 = 0; //LOW to begin with  
    TRISAbits.TRISA7 = 0; //output
    DIR2 = 0; //LOW to begin with

    TRISBbits.TRISB4 = 1; // USER button (random place for it)

    //OC1
    OC1CONbits.OCM = 0b110; //PWM mode without fault pin; other OC1CON bits are defaults
    OC1RS = 0; //0% duty Cycle
    OC1R = 0; //0% duty Cycle
    OC1CONbits.OCTSEL = 1; //Use timer 3
    OC1CONbits.ON = 1; // turn on OC1

    //OC4
    OC4CONbits.OCM = 0b110; //PWM mode without fault pin; other OC2CON bits are defaults
    OC4RS = 0; //0% duty Cycle
    OC4R = 0; //0% duty Cycle
    OC4CONbits.OCTSEL = 1; //Use timer 3
    OC4CONbits.ON = 1; // turn on OC2
}

void setDutyRight(float percent, unsigned char dir) //0 = forward, 1 = back
{
    int duty;
    if (dir == 0) {
        DIR1 = 0;
        percent = percent / 100;
        duty = (int) (percent * 2400);
        OC1RS = duty;
    } else {
        DIR1 = 1;
        percent = percent / 100;
        //        duty = (int) (2400 - (percent * 2400));
        duty = (int) (percent * 2400);
        OC1RS = duty;
    }
}

void setDutyLeft(float percent, unsigned char dir) //0 = forward, 1 = back
{
    int duty;
    if (dir == 0) {
        DIR2 = 1;
        percent = percent / 100;
        //        duty = (int) (2400 - (percent * 2400));
        duty = (int) (percent * 2400);
        OC4RS = duty;
    } else {
        DIR2 = 0;
        percent = percent / 100;
        duty = (int) (percent * 2400);
        OC4RS = duty;

    }
}

void CannonInit(void) {
    //setup cannons
    TRISBbits.TRISB2 = 0; // output
    TRISBbits.TRISB3 = 0; // output
    LATBbits.LATB2 = 0;
    LATBbits.LATB3 = 0;
}

void ShootLeftCannon(void) {
    _CP0_SET_COUNT(0);
    LATBbits.LATB2 = 1;
    while (_CP0_GET_COUNT() < 2500000) {

    }
    LATBbits.LATB2 = 0;
}

void ShootRightCannon(void) {
    _CP0_SET_COUNT(0);
    LATBbits.LATB2 = 1;
    while (_CP0_GET_COUNT() < 2500000) {

    }
    LATBbits.LATB2 = 0;
}