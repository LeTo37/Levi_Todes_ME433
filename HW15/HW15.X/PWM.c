#include"PWM.h"
 
 
void PWM_init(void){
    //Open pins for OC1 and OC2
    RPA0Rbits.RPA0R = 0b0101; //Set A0 as OC1 (PIN 2)
//    RPB8Rbits.RPB8R = 0b0101; //Set B8 as OC2 (PIN 17)
     
    //***************TIMER 2 PWM********************
    T2CONbits.TCKPS = 0; //Prescaler N=1
    // period = (PR2+1) * N * 20.83 ns
    PR2 = 2399;//With prescaler of 1 give a 20 kHz signal
    TMR2 = 0;                       //reset the timer  
    T2CONbits.ON = 1;               //Turn on the timer
    //**********************************************
    
    //***************TIMER 3 Interrupt**************
    T3CONbits.TCKPS = 6; //Prescaler N=64
    // period = (PR3+1) * N * 20.83 ns
    PR3 = 7500;//With prescaler of 1 give a 100 Hz signal
    TMR3 = 0;                       //reset the timer  
    T3CONbits.ON = 1;               //Turn on the timer
    IPC3bits.T3IP = 5;       //priority
    IPC3bits.T3IS = 0;       //sub-priority       
    IFS0bits.T3IF = 0;       //int flag
    IEC0bits.T3IE = 1;       //enable int
    //**********************************************
     
    //*************PHASE DIRECTIONS******************
    TRISBbits.TRISB2 = 0;//set as output
    LATBbits.LATB2 = 0; //LOW to begin with  
//    TRISBbits.TRISB13 = 0;//output
//    LATBbits.LATB13 = 0; //LOW to begin with
      
    //OC1
    OC1CONbits.OCM = 0b110;     //PWM mode without fault pin; other OC1CON bits are defaults
    OC1RS = 0;              //0% duty Cycle
    OC1R = 0;               //0% duty Cycle
    OC1CONbits.OCTSEL = 0;      //Use timer 2
    OC1CONbits.ON = 1;          // turn on OC1
     
    //OC2
//    OC2CONbits.OCM = 0b110;     //PWM mode without fault pin; other OC2CON bits are defaults
//    OC2RS = 0;              //0% duty Cycle
//    OC2R = 0;               //0% duty Cycle
//    OC2CONbits.OCTSEL = 0;      //Use timer 2
//    OC2CONbits.ON = 1;          // turn on OC2
}