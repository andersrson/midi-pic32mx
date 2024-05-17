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
#include <stdio.h>

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
    appData.readMidi1State = READ_MIDI_STATE_INIT;
    
    appData.lastReadStackSize = 0;
    appData.largestTaskStackSize = 0;
    
    appData.tmr2Ticks = 0;
}

void updateTaskStackSizeStats(TaskHandle_t task) {
    uint32_t size = uxTaskGetStackHighWaterMark(task);
    if(size > appData.largestTaskStackSize) {
        appData.largestTaskStackSize = size;
        appData.taskName = pcTaskGetName(task);
    }
}

typedef enum  {
    IDLE,
    START_BIT,
    DATA_BIT, 
    STOP_BIT
} MidiInputState;

MidiInputState midiPort1State;
uint8_t midiPortIn = 0;
uint8_t readBits; // How many bits have been read
uint8_t readByte; // temp storage byte currently being deserialized
uint8_t currentByteIndex; // where will next deserialized byte be saved
uint8_t midiBytes[4] = { 0, 0, 0, 0 }; // array of deserialized bytes
uint8_t subsequentOnes = 0; // counter to reset currentByteIndex

void timerCallback(uint32_t status, uintptr_t context) {
    GPIO_RA1_Toggle();
    midiPortIn = GPIO_RB5_Get();
    
    switch(midiPort1State) {
        case IDLE: {
            if(midiPortIn == 0) {
                midiPort1State = DATA_BIT;
                readBits = 0;
                readByte = 0;
                
            } else {
                // Reset after a few idle cycles, before we have actual interpretation of MIDI messages
                if(midiPortIn == 1)
                    subsequentOnes++;
                
                if(subsequentOnes > 5) {
                    currentByteIndex = 0;
                }
            }
            break;
        } case START_BIT: {
            midiPort1State = DATA_BIT;
            break;
        } case DATA_BIT: {
            readByte += midiPortIn << (readBits++);
            
            if(readBits == 8)
                midiPort1State = STOP_BIT;
            break;
        } case STOP_BIT: {
            GPIO_RA0_Clear();
            
            // Check if status byte
            if((readByte & 0x80) > 0) {
                currentByteIndex = 0;
                
                // We know we will overwrite byte 1 & 2, but not necessarily the last 2
                midiBytes[2] = 0;
                midiBytes[3] = 0;
            }
            
            midiBytes[currentByteIndex++] = readByte;
            midiPort1State = IDLE;
            subsequentOnes = 0;
            
            break;
        }
    }
}

void APP_READ_MIDI_Task(void) {
    switch(appData.readMidi1State) {
        case READ_MIDI_STATE_INIT: {
            midiPort1State = IDLE;
            
            TMR2_CallbackRegister(&timerCallback, 1);
            TMR2_Start();
            appData.readMidi1State = READ_MIDI_STATE_READY;
            break;
        } case READ_MIDI_STATE_INIT_ERR: {
            
            break;
        } case READ_MIDI_STATE_READY: {
            
            vTaskDelay(timer10);
            break;
        } case READ_MIDI_STATE_READING: {
            
            break;
        }
    }
    
    updateTaskStackSizeStats(xREAD_MIDI_Task);
}

/******************************************************************************
  Function:
    void APP_Tasks ( void )

  Remarks:
    See prototype in app.h.
 */

void APP_Task ( void ) {

    /* Check the application's current state. */
    switch ( appData.state ) {
        /* Application's initial state. */
        case APP_STATE_INIT: {

            GPIO_RA1_OutputEnable();
            GPIO_RA1_Clear();
            
            bool appInitialized = true;
            
            if (appInitialized)  {
                appData.state = APP_STATE_SERVICE_TASKS;
            }
            
            break;
        }

        case APP_STATE_SERVICE_TASKS: {
            vTaskDelay(timer500);
            
            //GPIO_RA1_Toggle();
            
            break;
        }

        /* The default state should never be executed. */
        default: {
            /* TODO: Handle error in application's state machine. */
            break;
        }
    }
    
    updateTaskStackSizeStats(xAPP_Task);
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
            
            GPIO_RA0_OutputEnable();
            GPIO_RA0_Clear();
            
            if(DRV_I2C_Status(sysObj.drvI2C0) == SYS_STATUS_READY) {
                appData.i2cHandle = DRV_I2C_Open(sysObj.drvI2C0, DRV_IO_INTENT_EXCLUSIVE);
                if(appData.i2cHandle == DRV_HANDLE_INVALID) {
                    appData.display1State = DISPLAY_STATE_INIT_ERR;
                    break;
                }
            }
            
            appData.lcd = HD44780AllocInstance(0x27, &appData.i2cHandle, &i2cSend);
            
            if(appData.lcd == NULL) {
                appData.display1State = DISPLAY_STATE_INIT_ERR;
                break;
            }
            
            if(i2cCheckError(HD44780Initialize(appData.lcd)) == false) {
                appData.display1State = DISPLAY_STATE_INIT_ERR;
                break;
            }
            
            HD44780SetSize(appData.lcd, 4, 20, true);
            
            if(i2cCheckError(HD44780PrintString(appData.lcd, appSTRINGS_DISPLAY_READY)) == false) {
                appData.display1State = DISPLAY_STATE_WRITE_ERR;
                break;
            }
            
            if(i2cCheckError(HD44780GoTo(appData.lcd, 0, 0)) == false) {
                appData.display1State = DISPLAY_STATE_WRITE_ERR;
                break;
            }
            
            if(i2cCheckError(HD44780PrintString(appData.lcd, appSTRINGS_APP_INITIALIZING)) == false) {
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
            
            if(appData.largestTaskStackSize > appData.lastReadStackSize) {        
                snprintf(appData.displayMessageBuffer, 21, "%s %d", appData.taskName, appData.largestTaskStackSize);
                
                i2cCheckError(HD44780GoTo(appData.lcd, 2, 0));
                i2cCheckError(HD44780PrintString(appData.lcd, appData.displayMessageBuffer));
                appData.lastReadStackSize = appData.largestTaskStackSize;
            }
            
            snprintf(appData.displayMessageBuffer, 21, "[%x] [%x] [%x]   ", midiBytes[0], midiBytes[1], midiBytes[2]);
            i2cCheckError(HD44780GoTo(appData.lcd, 3, 0));
            i2cCheckError(HD44780PrintString(appData.lcd, appData.displayMessageBuffer));

            
            vTaskDelay(timer500);
            
            //GPIO_RA0_Toggle();
            vTaskDelay(timer500);
            
            break;
        }
    }
    
    updateTaskStackSizeStats(xI2C_Task);
    vTaskDelay(timer50);
}

/*******************************************************************************
 End of File
 */
