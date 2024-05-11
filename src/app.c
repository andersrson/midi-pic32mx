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

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************
#include <stdbool.h>

#include "definitions.h"

#include "app.h"
#include "HD44780.h"

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

void APP_Initialize ( void ) {
    /* Place the App state machine in its initial state. */
    appData.state = APP_STATE_INIT;
    appData.display1State = DISPLAY_STATE_INIT;
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

//TickType_t timer500 = (500 / portTICK_PERIOD_MS);
//TickType_t timer100 = (100 / portTICK_PERIOD_MS);
//TickType_t timer50 = (50 / portTICK_PERIOD_MS);


void APP_Tasks ( void ) {

    /* Check the application's current state. */
    switch ( appData.state ) {
        /* Application's initial state. */
        case APP_STATE_INIT: {
            
            GPIO_RA0_OutputEnable();
            GPIO_RA0_Clear();
            GPIO_RA1_OutputEnable();
            GPIO_RA1_Clear();
            
            GPIO_RB2_OutputEnable();
            GPIO_RB2_Clear();
            GPIO_RB3_OutputEnable();
            GPIO_RB3_Clear();
            
            GPIO_RA3_OutputEnable();
            GPIO_RA3_Clear();
            
            GPIO_RB4_OutputEnable();
            GPIO_RB4_Clear();
            
            GPIO_RA4_OutputEnable();
            GPIO_RA4_Clear();
            
            GPIO_RB5_OutputEnable();
            GPIO_RB5_Clear();
            
            GPIO_RA2_InputEnable();
            GPIO_RA2_Clear();
            
            GPIO_RB7_InputEnable();
            GPIO_RB7_Clear();
            
            bool appInitialized = true;
            
            if (appInitialized)  {
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            
            break;
        }

        case APP_STATE_SERVICE_TASKS: {
            vTaskDelay(timer500);
            
            GPIO_RB5_Toggle();
            
            break;
        }

        /* TODO: implement your application state machine.*/


        /* The default state should never be executed. */
        default: {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
}

bool i2cSend(uint8_t InstanceId, void *sendCtx, uint8_t* data, uint8_t len) {
    DRV_HANDLE hnd = *(DRV_HANDLE*)sendCtx;
    return DRV_I2C_WriteTransfer(hnd, InstanceId, data, len);
}

bool i2cCheckError(bool input) {
    appData.i2cErr = DRV_I2C_ERROR_NONE;
    if(input == false) {
        appData.i2cErr = DRV_I2C_ErrorGet(appData.i2cHandle);
        if(appData.i2cErr == DRV_I2C_ERROR_NACK) {
            GPIO_RB3_Set();
        } else if (appData.i2cErr == DRV_I2C_ERROR_BUS) {
            GPIO_RB2_Set();
        } 
    }
    return appData.i2cErr == DRV_I2C_ERROR_NONE && input == true;
}

void APP_I2C_Task(void) {
    
    switch(appData.display1State) {
        case DISPLAY_STATE_INIT: {
            GPIO_RA0_Clear();
            GPIO_RA1_Clear();
            GPIO_RB2_Clear();
            GPIO_RB3_Clear();

            if(DRV_I2C_Status(sysObj.drvI2C0) == SYS_STATUS_READY) {
                appData.i2cHandle = DRV_I2C_Open(sysObj.drvI2C0, DRV_IO_INTENT_EXCLUSIVE);
                if(appData.i2cHandle == DRV_HANDLE_INVALID) {
                    appData.display1State = DISPLAY_STATE_INIT_ERR;
                    break;
                }
            }
            
            appData.lcd = HD44780AllocInstance(0x27, &appData.i2cHandle, &i2cSend);
            
            if(appData.lcd == NULL) {
                GPIO_RA0_Set();
                GPIO_RA1_Clear();
                GPIO_RB2_Clear();
                GPIO_RB3_Clear();

                appData.display1State = DISPLAY_STATE_INIT_ERR;
                break;
            }
            
            if(i2cCheckError(HD44780Initialize(appData.lcd)) == false) {
                GPIO_RA0_Clear();
                GPIO_RA1_Set();
                GPIO_RB2_Clear();
                GPIO_RB3_Clear();

                appData.display1State = DISPLAY_STATE_INIT_ERR;
                break;
            }
            
            if(i2cCheckError(HD44780PrintString(appData.lcd, appSTRINGS_DISPLAY_READY)) == false) {
                GPIO_RA0_Clear();
                GPIO_RA1_Clear();
                GPIO_RB2_Set();
                GPIO_RB3_Clear();

                appData.display1State = DISPLAY_STATE_WRITE_ERR;
                break;
            }
            
            if(i2cCheckError(HD44780PrintString(appData.lcd, appSTRINGS_APP_INITIALIZING)) == false) {
                GPIO_RA0_Clear();
                GPIO_RA1_Clear();
                GPIO_RB2_Clear();
                GPIO_RB3_Set();

                appData.display1State = DISPLAY_STATE_WRITE_ERR;
                break;
            }
            
            appData.display1State = DISPLAY_STATE_READY;
            
            break;
            
        } case DISPLAY_STATE_INIT_ERR: {    
            if(appData.i2cHandle != DRV_HANDLE_INVALID) {
                DRV_I2C_Close(appData.i2cHandle);
                appData.i2cHandle = DRV_HANDLE_INVALID;
            }
            
            break;
        } case DISPLAY_STATE_WRITE_ERR: {
    
            if(appData.i2cHandle != DRV_HANDLE_INVALID) {
                DRV_I2C_Close(appData.i2cHandle);
                appData.i2cHandle = DRV_HANDLE_INVALID;
            }
    
            break;
        } case DISPLAY_STATE_WRITE: {
            break;
        } case DISPLAY_STATE_READY: {
            
            GPIO_RA0_Toggle();
            vTaskDelay(timer500);
            
            break;
        }
    }
    
    vTaskDelay(timer50);
}

/*******************************************************************************
 End of File
 */
