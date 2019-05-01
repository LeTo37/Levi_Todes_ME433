/*******************************************************************************
  MPLAB Harmony Application Source File
  
  Company:
    Microchip Technology Inc.
  
  File Name:
    app.c

  Summary:
    This file contains the source code for the MPLAB Harmony application.

  Description:
    This file contains the source code for the MPLAB Harmony application.  It 
    implements the logic of the application's state machine and it may call 
    API routines of other MPLAB Harmony modules in the system, such as drivers,
    system services, and middleware.  However, it does not call any of the
    system interfaces (such as the "Initialize" and "Tasks" functions) of any of
    the modules in the system or make any assumptions about when those functions
    are called.  That is the responsibility of the configuration-specific system
    files.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
Copyright (c) 2013-2014 released Microchip Technology Inc.  All rights reserved.

Microchip licenses to you the right to use, modify, copy and distribute
Software only when embedded on a Microchip microcontroller or digital signal
controller that is integrated into your product or third party product
(pursuant to the sublicense terms in the accompanying license agreement).

You should refer to the license agreement accompanying this Software for
additional information regarding your rights and obligations.

SOFTWARE AND DOCUMENTATION ARE PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND,
EITHER EXPRESS OR IMPLIED, INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF
MERCHANTABILITY, TITLE, NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
IN NO EVENT SHALL MICROCHIP OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER
CONTRACT, NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR
OTHER LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE OR
CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT OF
SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
(INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.
 *******************************************************************************/
// DOM-IGNORE-END


// *****************************************************************************
// *****************************************************************************
// Section: Included Files 
// *****************************************************************************
// *****************************************************************************

#include "app.h"
#include "ili9341.h"
#include "I2C_Setup.h"
#include "imu.h"

#include <math.h>
#include <stdio.h>

// *****************************************************************************
// *****************************************************************************
// Section: Global Data Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    This structure should be initialized by the APP_Initialize function.
    
    Application strings and buffers are be defined outside this structure.
 */

APP_DATA appData;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Functions
// *****************************************************************************
// *****************************************************************************

/* TODO:  Add any necessary callback functions.
 */

// *****************************************************************************
// *****************************************************************************
// Section: Application Local Functions
// *****************************************************************************
// *****************************************************************************


/* TODO:  Add any necessary local functions.
 */


// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Initialize(void) {
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;


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
    //start fresh
    _CP0_SET_COUNT(0);

    /* TODO: Initialize your application's state machine and other
     * parameters.
     */
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Tasks(void) {

    short data[7];

    float gyroX, gyroY, gyroZ;
    char message[100];
    double fps = 0;
    /* Check the application's current state. */
    switch (appData.state) {
            /* Application's initial state. */
        case APP_STATE_INIT:
        {
            bool appInitialized = true;

            if (appInitialized) {

                appData.state = APP_STATE_SERVICE_TASKS;
            }
            break;
        }

        case APP_STATE_SERVICE_TASKS:
        {


            //        start fresh
            _CP0_SET_COUNT(0);
            IMU_read(data);
            gyroX = ((float) (data[5]))*(100.0 / 32768.0);
            gyroY = ((float) (data[4]))*(100.0 / 32768.0);
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

            break;
        }

            /* TODO: implement your application state machine.*/


            /* The default state should never be executed. */
        default:
        {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}



/*******************************************************************************
 End of File
 */
