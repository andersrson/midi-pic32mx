/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Anders Runesson

  @File Name
    SyncedPinReader.c

  @Summary
    Reads MIDI bit stream, synced to start bit @ 31250bps

  @Description
 */
/* ************************************************************************** */

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */

#include "config/default/definitions.h"

#include "ZwPinReader.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */


void lPinReaderPinChanged(GPIO_PIN pin, uintptr_t context) {
    GPIO_PinInterruptDisable(pin);
    struct ZwPinReader *reader = (struct ZwPinReader*) context;
        
    if(GPIO_PinRead(pin) == 0) {
        reader->ReaderState = PINREAD_DATA_BIT;
        
        reader->ReadBits = 0;
        reader->ReadByte = 0;
        reader->TimerStart();
    } else {
        GPIO_PinInterruptEnable(pin);
    }
}

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

static volatile uint32_t lPinsState;

void ZwPinReaderOnInput(uint32_t status, uintptr_t context) {
    
    struct ZwPinReader *reader = (struct ZwPinReader*) context;
    
    __conditional_software_breakpoint(reader != NULL);
    
    reader->PortIn = GPIO_PinRead(reader->Pin);
    
    switch(reader->ReaderState) {
        case PINREAD_NEVER_READ: {
            // Should never happen
            __conditional_software_breakpoint(false);
            break;
        } case PINREAD_IDLE: {
            // Should never happen
            __conditional_software_breakpoint(false);
            break;
        } case PINREAD_START_BIT: {
            // Should never happen
            __conditional_software_breakpoint(false);
            break;
        } case PINREAD_DATA_BIT: {
            
            reader->ReadByte += reader->PortIn << (reader->ReadBits++);
            if(reader->ReadBits == 8)
                reader->ReaderState = PINREAD_STOP_BIT;
            break;
        } case PINREAD_STOP_BIT: {
            
            __conditional_software_breakpoint(reader->NextBufferIndex < configPINREADER_BUFFER_SIZE);
            
            reader->Buffer[reader->NextBufferIndex++] = reader->ReadByte;

            if(reader->NextBufferIndex == configPINREADER_BUFFER_SIZE)
                reader->NextBufferIndex = 0;
            
            reader->ReaderState = PINREAD_IDLE;
            reader->ConsecutiveIdleTicks = 0;
            
            reader->TimerStop();
            
            GPIO_PinInterruptEnable(reader->Pin);
            
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xTaskNotifyFromISR(
                xDataProcessor_Task, 
                reader->FlagId,
                eSetBits,    
                &xHigherPriorityTaskWoken);
            
            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
            break;
        }
    }
}

void ZwPinReaderInitialize(uint32_t status, uintptr_t context) {
    
    struct ZwPinReader *reader = (struct ZwPinReader*) context;
    
    reader->ReaderState = PINREAD_NEVER_READ;
    
    if(GPIO_PinRead(reader->Pin) == 0) {
        reader->ConsecutiveIdleTicks = 0;
        return;  
    } 
    
    reader->ConsecutiveIdleTicks++;
    if(reader->ConsecutiveIdleTicks > 3) {
        reader->TimerStop();
        
        reader->TimerCallbackRegister(&ZwPinReaderOnInput, context);
        GPIO_PinInterruptCallbackRegister(reader->Pin, &lPinReaderPinChanged, context);
        GPIO_PinInterruptEnable(reader->Pin);
    }
}

/* *****************************************************************************
 End of File
 */
