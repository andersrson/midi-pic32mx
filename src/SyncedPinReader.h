/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.h

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

    /*  A brief description of a section can be given directly below the section
        banner.
     */


    /* ************************************************************************** */
    /** Descriptive Constant Name

      @Summary
        Brief one-line summary of the constant.
    
      @Description
        Full description, explaining the purpose and usage of the constant.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
      @Remarks
        Any additional remarks
     */

    // *****************************************************************************
    // *****************************************************************************
    // Section: Data Types
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

// Callback function for starting the timer
typedef void (*PINREAD_TIMER_START_FUNCTION)(void);

// Callback function for stopping the timer
typedef void (*PINREAD_TIMER_STOP_FUNCTION)(void);

// Callback function for registering the pin reader's callback function with the timer.
typedef void (*PINREAD_TIMER_CALLBACK_REGISTER_FUNCTION)( TMR_CALLBACK callback_fn, uintptr_t context );

typedef enum  {
    PINREAD_NEVER_READ,
    PINREAD_IDLE,
    PINREAD_START_BIT,
    PINREAD_DATA_BIT, 
    PINREAD_STOP_BIT
} PinReaderState;

struct PinReader_t {
    
    GPIO_PIN Pin;
    
    PinReaderState ReaderState;
    uint8_t ReadBits;
    uint8_t ReadByte;
    uint8_t CurrentByteIndex;
    uint8_t ConsecutiveIdleTicks;
    
    PINREAD_TIMER_START_FUNCTION TimerStart;
    PINREAD_TIMER_STOP_FUNCTION TimerStop;
    PINREAD_TIMER_CALLBACK_REGISTER_FUNCTION TimerCallbackRegister;
    
    uint8_t Buffer[configPINREADER_BUFFER_SIZE];
};

/**
 * PINREAD_lastByte must be large enough to hold the PinReader buffer size. E.g. if
 * configPINREADER_BUFFER_SIZE is greater than 255, PINREAD_lastByte must be at 
 * least uint16_t. 
 * @param PINREAD_lastByte integer Index to last written byte
 * @param PINREAD_preader Pointer to PinReader struct
 */
#define PINREAD_LAST_WRITTEN(PINREAD_lastByte, PINREAD_preader) (PINREAD_lastByte = (PINREAD_preader->CurrentByteIndex - 1) % configPINREADER_BUFFER_SIZE)

#define PINREAD_NEXT_BYTE(PINREAD_index) (PINREAD_index = (PINREAD_index + 1) % configPINREADER_BUFFER_SIZE)
#define PINREAD_PREV_BYTE(PINREAD_index) (PINREAD_index = (PINREAD_index - 1) % configPINREADER_BUFFER_SIZE)

    // *****************************************************************************

    /** Descriptive Data Type Name

      @Summary
        Brief one-line summary of the data type.
    
      @Description
        Full description, explaining the purpose and usage of the data type.
        <p>
        Additional description in consecutive paragraphs separated by HTML 
        paragraph breaks, as necessary.
        <p>
        Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

      @Remarks
        Any additional remarks
        <p>
        Describe enumeration elements and structure and union members above each 
        element or member.
     */
    
    


    // *****************************************************************************
    // *****************************************************************************
    // Section: Interface Functions
    // *****************************************************************************
    // *****************************************************************************

    /*  A brief description of a section can be given directly below the section
        banner.
     */

    // *****************************************************************************

    /**
      @Function
        void PinReaderOnInput ( uint32_t status, uintptr_t context ) 

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
void PinReaderOnInput(uint32_t status, uintptr_t context);

/**
 * @Function
 *      void pinReaderInitialize(uint32_t status, uintptr_t context)
 * @Summary
 *      Initialize a PinReader to start reading from an input pin.
 *      This function is first set as the timer callback and the 
 *      timer is started. In order to avoid reading partial bytes the
 *      function will wait until 3 consecutive idle cycles have been 
 *      detected. At that point it will replace itself with the pin reader's 
 *      callback.
 *      
 * @param status uint32_t Timer callback value, not used
 * @param context uintptr_t Pointer to pin reader struct
 */
void PinReaderInitialize(uint32_t status, uintptr_t context);

    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _EXAMPLE_FILE_NAME_H */

/* *****************************************************************************
 End of File
 */
