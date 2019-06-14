#ifndef PWM
#define PWM
 
#include<xc.h>           // processor SFR definitions
#include<sys/attribs.h>  // __ISR macro

#define DIR1 LATAbits.LATA10
#define DIR2 LATAbits.LATA7
#define USER PORTBbits.RB4
 
void init_PWM(void);
void setDutyRight(float perc, unsigned char dir);
void setDutyLeft(float perc, unsigned char dir);
void CannonInit(void);
void ShootLeftCannon(void);
void ShootRightCannon(void);
 
#endif