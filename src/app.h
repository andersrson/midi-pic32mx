/*******************************************************************************
  MPLAB Harmony Application Header File

  Company:
    Microchip Technology Inc.

  File Name:
    app.h

  Summary:
    This header file provides prototypes and definitions for the application.

  Description:
    This header file provides function prototypes and data type definitions for
    the application.  Some of these are required by the system (such as the
    "APP_Initialize" and "APP_Tasks" prototypes) and some of them are only used
    internally by the application (such as the "APP_STATES" definition).  Both
    are defined here for convenience.
*******************************************************************************/

#ifndef _APP_H
#define _APP_H

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

#include "configuration.h"

#include "definitions.h"

#include "ZwMidi.h"
#include "ZwPinReader.h"
#include "ZwMidiProcessor.h"
#include "DataFilter.h"
#include "DataModifier.h"
#include "OutputProcessor.h"
#include "ZwUartReader.h"

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Type Definitions
// *****************************************************************************
// *****************************************************************************

// *****************************************************************************
/* Application states

  Summary:
    Application states enumeration

  Description:
    This enumeration defines the valid application states.  These states
    determine the behavior of the application at various times.
*/

enum APP_STATES {
    /* Application's state machine's initial state. */
    APP_STATE_INIT=0,
    APP_STATE_SERVICE_TASKS,
    /* TODO: Define states used by the application state machine. */

} ;

enum DISPLAY_STATE {
    DISPLAY_STATE_INIT = 0,
    DISPLAY_STATE_INIT_ERR,
    DISPLAY_STATE_WRITE,
    DISPLAY_STATE_WRITE_ERR,
    DISPLAY_STATE_READY,
};

enum READ_MIDI_STATE {
    READ_MIDI_STATE_INIT = 0,
    READ_MIDI_STATE_INIT_ERR,
    READ_MIDI_STATE_READY,
    READ_MIDI_STATE_READING,
};

// *****************************************************************************
/* Application Data

  Summary:
    Holds application data

  Description:
    This structure holds the application's data.

  Remarks:
    Application strings and buffers are be defined outside this structure.
 */


struct MidiTransform {
    
};

struct MidiFilter {
    
};

struct Project {
    bool displayFilterMidiTime;
    bool displayFilterMidiNoteOff;
};
        
struct DeviceConfig {
    
    
};

struct UserData{
    struct DeviceConfig devConfig;
    struct Project *currentProject;
    struct Project projects[configMAX_USER_PROJECTS];
    uint16_t lastUsedProject;
};

typedef struct {
    /* The application's current state */
    enum APP_STATES state;
    
    enum DISPLAY_STATE display1State;
    struct HD44780Instance *lcd;
    char displayMessageBuffer[128];
    
    DRV_HANDLE i2cHandle;
    DRV_I2C_ERROR i2cErr;
    
    uint32_t lastReadStackSize;
    uint32_t largestTaskStackSize;
    char *taskName;
    
    uint32_t lastReadAvailableHeapBytes;
    uint32_t availableHeapBytes;
    
    struct UserData userData;
    
    enum READ_MIDI_STATE readMidi1State;
    
    struct ZwPinReader PinReader[configPINREADER_COUNT];
    
    struct ZwUsartInput UsartInputs[configUSARTREADER_COUNT];
    
    struct OutputPin_t OutputPin[configOUTPUT_COUNT];
} APP_DATA;

extern APP_DATA appData;

extern TaskHandle_t xAPP_Task;
extern TaskHandle_t xI2C_Task;
extern TaskHandle_t xDataProcessor_Task;

extern TickType_t timer500;
extern TickType_t timer100;
extern TickType_t timer50;
extern TickType_t timer10;
extern TickType_t timer5;
extern TickType_t timer1;

// *****************************************************************************
// *****************************************************************************
// Section: Application Callback Routines
// *****************************************************************************
// *****************************************************************************
/* These routines are called by drivers when certain events occur.
*/

// *****************************************************************************
// *****************************************************************************
// Section: Application Initialization and State Machine Functions
// *****************************************************************************
// *****************************************************************************

/*******************************************************************************
  Function:
    void APP_Initialize ( void )

  Summary:
     MPLAB Harmony application initialization routine.

  Description:
    This function initializes the Harmony application.  It places the
    application in its initial state and prepares it to run so that its
    APP_Tasks function can be called.

  Precondition:
    All other system initialization routines should be called before calling
    this routine (in "SYS_Initialize").

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Initialize();
    </code>

  Remarks:
    This routine must be called from the SYS_Initialize function.
*/

void APP_Initialize ( void );


/*******************************************************************************
  Function:
    void APP_Tasks ( void )

  Summary:
    MPLAB Harmony Demo application tasks function

  Description:
    This routine is the Harmony Demo application's tasks function.  It
    defines the application's state machine and core logic.

  Precondition:
    The system and application initialization ("SYS_Initialize") should be
    called before calling this.

  Parameters:
    None.

  Returns:
    None.

  Example:
    <code>
    APP_Tasks();
    </code>

  Remarks:
    This routine must be called from SYS_Tasks() routine.
 */

void APP_PinReaderTask(void);

void APP_Task( void );

void APP_I2C_Task(void);

#define appSTRINGS_BLANK_LINE           "                    "
#define appSTRINGS_DISPLAY_READY        "Display ready"
#define appSTRINGS_APP_INITIALIZING     "App initializing.."

//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif /* _APP_H */

/*******************************************************************************
 End of File
 */

