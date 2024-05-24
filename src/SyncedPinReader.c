/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
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

#include "SyncedPinReader.h"

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

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

void PinReaderOnInput(uint32_t status, uintptr_t context) {
    
    struct PinReader_t *reader = (struct PinReader_t*) context;
    
    uint8_t midiPortIn = GPIO_PinRead(reader->Pin);
    
    switch(reader->ReaderState) {
        case PINREAD_NEVER_READ: {
            // Should never happen
            break;
        } case PINREAD_IDLE: {
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
            
            if(reader->CurrentByteIndex >= configPINREADER_BUFFER_SIZE)
                reader->CurrentByteIndex = 0;
            
            reader->Buffer[reader->CurrentByteIndex++] = reader->ReadByte;

            reader->ReaderState = PINREAD_IDLE;
            reader->ConsecutiveIdleTicks = 0;
            
            reader->TimerStop();
            GPIO_PinInterruptEnable(reader->Pin);
            
            break;
        }
    }
}

void PinReaderInitialize(uint32_t status, uintptr_t context) {
    
    struct PinReader_t *reader = (struct PinReader_t*) context;
    
    reader->ReaderState = PINREAD_NEVER_READ;
    
    if(GPIO_PinRead(reader->Pin) == 0) {
        reader->ConsecutiveIdleTicks = 0;
        return;  
    } 
    
    reader->ConsecutiveIdleTicks++;
    if(reader->ConsecutiveIdleTicks > 3) {
        reader->TimerStop();
        
        reader->TimerCallbackRegister(&PinReaderOnInput, context);
        GPIO_PinInterruptCallbackRegister(reader->Pin, &lPinReaderPinChanged, context);
        GPIO_PinInterruptEnable(reader->Pin);
    }
}

/* *****************************************************************************
 End of File
 */
