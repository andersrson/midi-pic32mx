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

#include "config/default/driver/driver.h"
#include "config/default/driver/usart/drv_usart.h"

#include "app.h"
#include "ZwUartReader.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/* 
 * 

#define configUSARTREADER_COUNT                   (2U)
#define configUSARTREADER_BUFFER_SIZE             (256U)

 */


uint8_t nextToInitialize = 0;

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



/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

DRV_HANDLE ZwUartInitialize(SYS_MODULE_INDEX moduleIndex) {
    if(nextToInitialize == configUSARTREADER_COUNT)
        return 0;
    
    struct ZwUsartInput* input = &appData.UsartInputs[nextToInitialize++];
    
    DRV_HANDLE usartHandle = 0;
    usartHandle = DRV_USART_Open(moduleIndex, DRV_IO_INTENT_READ);
    
    input->driver = usartHandle;
    
    DRV_USART_BufferEventHandlerSet(usartHandle, ZwUartReadCallback, (uintptr_t) input);
    
    DRV_USART_ReadBufferAdd(
        usartHandle,
        input->InputBuffer,
        configUSARTREADER_BUFFER_SIZE,
        &input->bufferHandle
    );
    
    
    if(input->bufferHandle == DRV_USART_BUFFER_HANDLE_INVALID)
        input->State = ZW_USART_STATE_ERR_INIT;
    
    return usartHandle;
}

void ZwUartReadCallback(DRV_USART_BUFFER_EVENT event, DRV_USART_BUFFER_HANDLE bufferHandle, uintptr_t context) {

    struct ZwUsartInput* input = (struct ZwUsartInput*)context;
    
    switch(event) {
        case DRV_USART_BUFFER_EVENT_COMPLETE: 
            break;
        case DRV_USART_BUFFER_EVENT_ERROR:
            input->State = ZW_USART_STATE_ERR; 
            input->Error = DRV_USART_ErrorGet(input->bufferHandle);
            break;
        case DRV_USART_BUFFER_EVENT_PENDING:
            break;
        case DRV_USART_BUFFER_EVENT_HANDLE_EXPIRED:
            input->State = ZW_USART_STATE_ERR_HANDLE_EXPIRED; 
            input->Error = DRV_USART_ErrorGet(input->bufferHandle);
            break;
        case DRV_USART_BUFFER_EVENT_HANDLE_INVALID:
            input->State = ZW_USART_STATE_ERR_HANDLE_INVALID; 
            input->Error = DRV_USART_ErrorGet(input->bufferHandle);
            break;
    }
    
}
// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */



/* *****************************************************************************
 End of File
 */
