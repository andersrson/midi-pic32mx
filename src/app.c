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
    appData.availableHeapBytes = 0;
    appData.lastReadAvailableHeapBytes = 0;
}

void updateTaskStackSizeStats(TaskHandle_t task) {
    uint32_t size = uxTaskGetStackHighWaterMark(task);
    
    if(size > appData.largestTaskStackSize) {
        appData.largestTaskStackSize = size;
        appData.taskName = pcTaskGetName(task);
    }
    
    uint32_t heap = xPortGetFreeHeapSize();
    if(heap > appData.availableHeapBytes)
        appData.availableHeapBytes = heap;
}

void APP_PinReaderTask(void) {
    switch(appData.readMidi1State) {
        case READ_MIDI_STATE_INIT: {
            
            struct PinReader_t *reader = (struct PinReader_t*) &appData.PinReader[0];
            reader->Pin = GPIO_PIN_RB5;
            reader->ConsecutiveIdleTicks = 0;
            reader->TimerStart = &TMR2_Start;
            reader->TimerStop = &TMR2_Stop;
            reader->TimerCallbackRegister = &TMR2_CallbackRegister;
    
            uintptr_t pr0 = (uintptr_t) reader;
            
            TMR2_CallbackRegister(&PinReaderInitialize, pr0);
            TMR2_Start();
            
            appData.readMidi1State = READ_MIDI_STATE_READY;
            break;
        } case READ_MIDI_STATE_INIT_ERR: {
            
            break;
        } case READ_MIDI_STATE_READY: {
            
            vTaskDelay(timer500);
            break;
        } case READ_MIDI_STATE_READING: {
            
            break;
        }
    }
    
    updateTaskStackSizeStats(xPinReader_Task);
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
            
            if(     appData.largestTaskStackSize > appData.lastReadStackSize ||
                    appData.availableHeapBytes > appData.lastReadAvailableHeapBytes) {        
                
                snprintf(appData.displayMessageBuffer, 21, "%s %d %d", 
                        appData.taskName, 
                        appData.largestTaskStackSize,
                        appData.availableHeapBytes);
                
//                snprintf(appData.displayMessageBuffer, 21, "%s %d", 
//                        appData.taskName, 
//                        appData.largestTaskStackSize
//                        //,appData.availableHeapBytes
//                        );
                
                i2cCheckError(HD44780GoTo(appData.lcd, 2, 0));
                i2cCheckError(HD44780PrintString(appData.lcd, appData.displayMessageBuffer));
                appData.lastReadStackSize = appData.largestTaskStackSize;
            }
            
            struct PinReader_t *reader = (struct PinReader_t*) &appData.PinReader[0];
            
            __conditional_software_breakpoint(reader != NULL);
            
            while(reader->ReaderState != PINREAD_IDLE) {
                vTaskDelay(timer1);
            }
            
            uint16_t lastByteIndex = 0;
            
            PINREAD_LAST_WRITTEN(lastByteIndex, reader);
            
            __conditional_software_breakpoint(lastByteIndex < configPINREADER_BUFFER_SIZE);
            
            uint8_t byte0 = reader->Buffer[lastByteIndex];
            while(!MIDI_IS_STATUS_BYTE(byte0)) {
                PINREAD_PREV_BYTE(lastByteIndex);
                __conditional_software_breakpoint(lastByteIndex < configPINREADER_BUFFER_SIZE);
                byte0 = reader->Buffer[lastByteIndex];
            }
            
            uint8_t byte1 = 0;
            uint8_t byte2 = 0;
            const char* byte0Str;
            char byte1Str[4] = "   ";
            
            if(MIDI_IS_SYSEX(byte0)) {
                MIDI_GET_SYSEX_STRING_SHORT(byte0, byte0Str);
                
                if(MIDI_IS_SYSEX_SONG_SEL(byte0)) {
                    PINREAD_NEXT_BYTE(lastByteIndex);
                    byte1 = reader->Buffer[lastByteIndex];
            
                    snprintf(appData.displayMessageBuffer, 21, "[%s] [%x]       ", byte0Str, byte1);
                } else if(MIDI_IS_SYSEX_SONG_POS(byte0)) {
                    PINREAD_NEXT_BYTE(lastByteIndex);
                    byte1 = reader->Buffer[lastByteIndex];
                    PINREAD_NEXT_BYTE(lastByteIndex);
                    byte2 = reader->Buffer[lastByteIndex];

                    snprintf(appData.displayMessageBuffer, 21, "[%s] [%x] [%x]", byte0Str, byte1, byte2);
                } else 
                    snprintf(appData.displayMessageBuffer, 21, "[%s]     ", byte0Str);
            } else if(MIDI_IS_STATUS_BYTE(byte0)) {
                MIDI_GET_STATUS_SHORT_STRING(byte0, byte0Str);
                PINREAD_NEXT_BYTE(lastByteIndex);
                byte1 = reader->Buffer[lastByteIndex];
                    
                if(MIDI_IS_NOTE(byte0)) {
                    MIDI_GET_NOTE_STRING(byte1, byte1Str);
                    snprintf(appData.displayMessageBuffer, 21, "[%s] [%s]   ", byte0Str, byte1Str);
                } else 
                    snprintf(appData.displayMessageBuffer, 21, "[%s] [%x]   ", byte0Str, byte1);
                
            } else {
                PINREAD_NEXT_BYTE(lastByteIndex);
                byte1 = reader->Buffer[lastByteIndex];
                PINREAD_NEXT_BYTE(lastByteIndex);
                byte2 = reader->Buffer[lastByteIndex];
                snprintf(appData.displayMessageBuffer, 21, "[%x] [%x] [%x]   ", byte0, byte1, byte2);
            }
            
            i2cCheckError(HD44780GoTo(appData.lcd, 3, 0));
            i2cCheckError(HD44780PrintString(appData.lcd, appData.displayMessageBuffer));
            
            GPIO_RA0_Toggle();
            vTaskDelay(timer100);
            
            break;
        }
    }
    
    updateTaskStackSizeStats(xI2C_Task);
    vTaskDelay(timer50);
}

/*******************************************************************************
 End of File
 */
