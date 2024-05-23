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
}

void updateTaskStackSizeStats(TaskHandle_t task) {
    uint32_t size = uxTaskGetStackHighWaterMark(task);
    if(size > appData.largestTaskStackSize) {
        appData.largestTaskStackSize = size;
        appData.taskName = pcTaskGetName(task);
    }
}

void tmrInputRead(uint32_t status, uintptr_t context) {
    
    struct PinReader_t *reader = (struct PinReader_t*) context;
    
    uint8_t midiPortIn = GPIO_PinRead(reader->Pin);
    
    switch(reader->ReaderState) {
        case PINREAD_IDLE: {
            // Should never happen
            break;
        } case PINREAD_START_BIT: {
            // Should never happen
            break;
        } case PINREAD_DATA_BIT: {
            reader->ReadByte += midiPortIn << (reader->ReadBits++);
            
            if(reader->ReadBits == 8)
                reader->ReaderState = PINREAD_STOP_BIT;
            break;
        } case PINREAD_STOP_BIT: {
            // Check if status byte, if so put it in first byte
            if((reader->ReadByte & 0x80) > 0) {
                reader->CurrentByteIndex = 0;
                
                // We know we will overwrite byte 1 & 2, but not necessarily the last 2
                reader->Buffer[2] = 0;
                reader->Buffer[3] = 0;
            }

            // discard bytes after byte 4 for now
            if(reader->CurrentByteIndex < 4)
                reader->Buffer[reader->CurrentByteIndex++] = reader->ReadByte;

            reader->ReaderState = PINREAD_IDLE;
            reader->ConsecutiveIdleTicks = 0;
            
            reader->TimerStop();
            GPIO_PinInterruptEnable(reader->Pin);
            
            break;
        }
    }
}

void pinChangeNotification(GPIO_PIN pin, uintptr_t context) {
    GPIO_PinInterruptDisable(pin);
        
    if(GPIO_PinRead(pin) == 0) {
        struct PinReader_t *reader = (struct PinReader_t*) context;
        
        reader->ReaderState = PINREAD_DATA_BIT;
        
        reader->ReadBits = 0;
        reader->ReadByte = 0;
        reader->TimerStart();
    } else {
        GPIO_PinInterruptEnable(pin);
    }
}

void tmrInputInitialize(uint32_t status, uintptr_t context) {
    
    struct PinReader_t *reader = (struct PinReader_t*) context;
    
    if(GPIO_PinRead(reader->Pin) == 0) {
        reader->ConsecutiveIdleTicks = 0;
        return;  
    } 
    
    reader->ConsecutiveIdleTicks++;
    if(reader->ConsecutiveIdleTicks > 3) {
        reader->TimerStop();
        
        reader->TimerCallbackRegister(&tmrInputRead, context);
        GPIO_PinInterruptCallbackRegister(reader->Pin, &pinChangeNotification, context);
        GPIO_PinInterruptEnable(reader->Pin);
    }
}

void APP_READ_MIDI_Task(void) {
    switch(appData.readMidi1State) {
        case READ_MIDI_STATE_INIT: {
            struct PinReader_t *reader = (struct PinReader_t*) &appData.PinReader[0];
            
            reader->ReaderState = PINREAD_IDLE;
            reader->ConsecutiveIdleTicks = 0;
            reader->Pin = GPIO_PIN_RB5;
            reader->TimerStart = &TMR2_Start;
            reader->TimerStop = &TMR2_Stop;
            reader->TimerCallbackRegister = &TMR2_CallbackRegister;
            
            uintptr_t pr0 = (uintptr_t) reader;
            reader->TimerCallbackRegister(&tmrInputInitialize, pr0);
            reader->TimerStart();
            
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
            
            uint8_t byte0 = appData.PinReader[0].Buffer[0];
            uint8_t byte1 = appData.PinReader[0].Buffer[1];
            uint8_t byte2 = appData.PinReader[0].Buffer[2];
            
            const char* byte0Str;
            char byte1Str[4] = "   ";
            
            if(MIDI_IS_SYSEX(byte0)) {
                MIDI_GET_SYSEX_STRING_SHORT(byte0, byte0Str);
                
                if(MIDI_IS_SYSEX_SONG_SEL(byte0)) {
                    snprintf(appData.displayMessageBuffer, 21, "[%s] [%x]       ", byte0Str, byte1);
                } else if(MIDI_IS_SYSEX_SONG_POS(byte0)) {
                    snprintf(appData.displayMessageBuffer, 21, "[%s] [%x] [%x]", byte0Str, byte1, byte2);
                } else 
                    snprintf(appData.displayMessageBuffer, 21, "[%s]            ", byte0Str);
            } else if(MIDI_IS_STATUS_BYTE(byte0)) {
                MIDI_GET_STATUS_SHORT_STRING(byte0, byte0Str);
                
                if(MIDI_IS_NOTE(byte0)) {
                    MIDI_GET_NOTE_STRING(byte1, byte1Str);
                    snprintf(appData.displayMessageBuffer, 21, "[%s] [%s]       ", byte0Str, byte1Str);
                } else 
                    snprintf(appData.displayMessageBuffer, 21, "[%s] [%x]       ", byte0Str, byte1);
                
            } else {
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
