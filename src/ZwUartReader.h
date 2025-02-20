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

#ifndef _ZWUARTREADER_H    /* Guard against multiple inclusion */
#define _ZWUARTREADER_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include <stddef.h>

#include "config/default/driver/driver.h"
#include "config/default/driver/usart/drv_usart.h"

#include "configuration.h"

#include "definitions.h"

#include "app.h"

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

enum ZW_USART_STATE {
    ZW_USART_STATE_INIT = 0,
    ZW_USART_STATE_READY,
    ZW_USART_STATE_ERR_INIT,
    ZW_USART_STATE_READING,
    ZW_USART_STATE_READ_COMPLETE,
    ZW_USART_STATE_ERR,
    ZW_USART_STATE_ERR_HANDLE_INVALID,
    ZW_USART_STATE_ERR_HANDLE_EXPIRED,
};

struct ZwUsartInput {
    
    uint8_t FlagId; 
    
    DRV_HANDLE driver;
    uint8_t InputBuffer[configUSARTREADER_BUFFER_SIZE];
    uint8_t NextIndex;
    uint8_t NextUnread;
    
    DRV_USART_BUFFER_HANDLE bufferHandle;
    enum ZW_USART_STATE State;
    DRV_USART_ERROR Error;
};

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

struct ZwUsartInput* ZwUartInitialize(struct ZwUsartInput* input, SYS_MODULE_INDEX moduleIndex, uint8_t flagId);

uint8_t ZwUartReaderGetNextUnread(uintptr_t reader);

void ZwUartReadCallback(DRV_USART_BUFFER_EVENT event, DRV_USART_BUFFER_HANDLE bufferHandle, uintptr_t context);
//void ZwUartReadCallback(uintptr_t context);

    // *****************************************************************************
    /**
      @Function
        int ExampleFunctionName ( int param1, int param2 ) 

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


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _ZWUARTREADER_H */

/* *****************************************************************************
 End of File
 */
