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

TickType_t timer500 = (500 / portTICK_PERIOD_MS);
TickType_t timer100 = (100 / portTICK_PERIOD_MS);
TickType_t timer50 = (50 / portTICK_PERIOD_MS);
TickType_t timer10 = (10 / portTICK_PERIOD_MS);
TickType_t timer5 = (5 / portTICK_PERIOD_MS);
TickType_t timer1 = (1 / portTICK_PERIOD_MS);

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
    
    // TODO Test settings
    appData.userData.lastUsedProject = 0;
    appData.userData.currentProject = &appData.userData.projects[appData.userData.lastUsedProject];
    appData.userData.currentProject->displayFilterMidiTime = true;
    appData.userData.currentProject->displayFilterMidiNoteOff = true;
    
    struct ZwPinReader *reader = (struct ZwPinReader*) &appData.PinReader[0];
    uintptr_t up_reader = 0;

    reader->Pin = GPIO_PIN_RB5;
    reader->FlagId = 0x01;
    
    reader->ConsecutiveIdleTicks = 0;
    reader->TimerStart = &TMR2_Start;
    reader->TimerStop = &TMR2_Stop;
    reader->TimerCallbackRegister = &TMR2_CallbackRegister;

    up_reader = (uintptr_t) reader;

    TMR2_CallbackRegister(&ZwPinReaderInitialize, up_reader);

    reader = (struct ZwPinReader*) &appData.PinReader[1];
    
    reader->Pin = GPIO_PIN_RB7;
    reader->FlagId = 0x02;
    
    reader->ConsecutiveIdleTicks = 0;
    reader->TimerStart = &TMR3_Start;
    reader->TimerStop = &TMR3_Stop;
    reader->TimerCallbackRegister = &TMR3_CallbackRegister;

    up_reader = (uintptr_t) reader;

    TMR3_CallbackRegister(&ZwPinReaderInitialize, up_reader);

}

void updateTaskStackSizeStats(TaskHandle_t task) {
    
    // uxTaskGetStackHighWaterMark() returns the smallest amount of free stack 
    // space available since the task was started. 
    uint32_t size = configTASK_STACK_SIZE - uxTaskGetStackHighWaterMark(task);
    
    if(size > appData.largestTaskStackSize) {
        appData.largestTaskStackSize = size;
        appData.taskName = pcTaskGetName(task);
    }
    
    uint32_t heap = xPortGetFreeHeapSize();
    if(heap > appData.availableHeapBytes)
        appData.availableHeapBytes = heap;
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
            // TODO: Error handling
        } else if (appData.i2cErr == DRV_I2C_ERROR_BUS) {
            // TODO: Error handling
        } 
    }
    return appData.i2cErr == DRV_I2C_ERROR_NONE && input == true;
}


void printMidi(struct ZwPinReader *reader, uint8_t lcdLine, uint8_t lcdCol) {

    __conditional_software_breakpoint(reader != NULL);

    if(reader->ReaderState == PINREAD_NEVER_READ)
        return;
    
    while(reader->ReaderState != PINREAD_IDLE) {
        vTaskDelay(timer1);
    }

    uint16_t lastByteIndex = 0;
    uint16_t lastByteSafety = 0; 
    uint8_t byte0 = 0;
    //uint8_t byte0_value = 0;        // Lower nibble of first byte
    uint8_t byte1 = 0;
    uint8_t byte2 = 0;
    const char* byte0Str;
    char byte1Str[4] = "   ";
    
    PINREAD_LAST_WRITTEN(lastByteIndex, reader);
    lastByteSafety = lastByteIndex;
    
    __conditional_software_breakpoint(lastByteIndex < configPINREADER_BUFFER_SIZE);

    while(!MIDI_IS_STATUS_BYTE(byte0)) {
        PINREAD_PREV_BYTE(lastByteIndex);
        
        // If we've looped we did not find a status byte
        if(lastByteIndex == lastByteSafety)
            return;
        
        __conditional_software_breakpoint(lastByteIndex < configPINREADER_BUFFER_SIZE);
        byte0 = reader->Buffer[lastByteIndex];
    }
    //byte0_value = byte0 & MIDI_STATUS_LOWER_NIBBLE_MASK;
    
    if(MIDI_IS_SYSTEM(byte0)) {
        
        // Filter MIDI time code messages from flooding display
        if(MIDI_GET_STATUS_VALUE(byte0) == MIDI_SYSTEM_COMMON_TIME_CODE || MIDI_GET_STATUS_VALUE(byte0) == MIDI_SYSTEM_REALTIME_TIME_CLK) {
            if(appData.userData.currentProject->displayFilterMidiTime)
                return;
        }
        MIDI_GET_SYSTEM_STRING_SHORT(byte0, byte0Str);

        if(MIDI_IS_SYSTEM_SONG_SEL(byte0)) {
            PINREAD_NEXT_BYTE(lastByteIndex);
            byte1 = reader->Buffer[lastByteIndex];

            snprintf(appData.displayMessageBuffer, 21, "[%s] [%x]       ", byte0Str, byte1);
        } else if(MIDI_IS_SYSTEM_SONG_POS(byte0)) {
            PINREAD_NEXT_BYTE(lastByteIndex);
            byte1 = reader->Buffer[lastByteIndex];
            PINREAD_NEXT_BYTE(lastByteIndex);
            byte2 = reader->Buffer[lastByteIndex];

            snprintf(appData.displayMessageBuffer, 21, "[%s] [%x] [%x]", byte0Str, byte1, byte2);
        } else 
            snprintf(appData.displayMessageBuffer, 21, "[%s]     ", byte0Str);
    } else if(MIDI_IS_STATUS_BYTE(byte0)) {
        if(MIDI_IS_NOTE_OFF(byte0)) {
            if(appData.userData.currentProject->displayFilterMidiNoteOff)
                return;
        }
        
        MIDI_GET_CHANNEL_SHORT_STRING(byte0, byte0Str);
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

    i2cCheckError(HD44780GoTo(appData.lcd, lcdLine, lcdCol));
    i2cCheckError(HD44780PrintString(appData.lcd, appData.displayMessageBuffer));
            
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
                
                i2cCheckError(HD44780GoTo(appData.lcd, 0, 0));
                i2cCheckError(HD44780PrintString(appData.lcd, appData.displayMessageBuffer));
                appData.lastReadStackSize = appData.largestTaskStackSize;
            }
            
            struct ZwPinReader *reader = (struct ZwPinReader*) &appData.PinReader[0];
            printMidi(reader, 2, 0);
            reader = (struct ZwPinReader*) &appData.PinReader[1];
            printMidi(reader, 3, 0);
            
            GPIO_RA0_Toggle();
            vTaskDelay(timer10);
            
            break;
        }
    }
    
    updateTaskStackSizeStats(xI2C_Task);
    vTaskDelay(timer50);
}

/*******************************************************************************
 End of File
 */
