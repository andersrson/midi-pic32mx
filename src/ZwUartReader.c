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

#include <xc.h>

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

struct ZwUsartInput* ZwUartInitialize(struct ZwUsartInput* input, SYS_MODULE_INDEX moduleIndex, uint8_t flagId) {
    if(input == NULL)
        return 0;
    
    input->FlagId = flagId;
    
    DRV_HANDLE usartHandle = 0;
    usartHandle = DRV_USART_Open(moduleIndex, DRV_IO_INTENT_READ);
    input->driver = usartHandle;
    
    if(usartHandle == DRV_HANDLE_INVALID) {
        input->State = ZW_USART_STATE_ERR_INIT;
        return NULL;
    }
    
    input->NextUnread = 0;
    
    DRV_USART_BufferEventHandlerSet(usartHandle, ZwUartReadCallback, (uintptr_t) input);
    
    DRV_USART_ReadBufferAdd(
        usartHandle,
        &input->InputBuffer[input->NextIndex],
        1, // configUSARTREADER_BUFFER_SIZE,
        &input->bufferHandle
    );
    
    if(input->bufferHandle == DRV_USART_BUFFER_HANDLE_INVALID)
        input->State = ZW_USART_STATE_ERR_INIT;
    else
        input->State = ZW_USART_STATE_READY;
    
    return input;
}

uint8_t ZwUartReaderGetNextUnread(uintptr_t reader) {
    struct ZwUsartInput* input = (struct ZwUsartInput*)reader;
    uint8_t value = input->InputBuffer[input->NextUnread++];
    
    if(input->NextUnread >= configUSARTREADER_BUFFER_SIZE)
        input->NextUnread = 0;
    
    return value;
}

void ZwUartReadCallback(DRV_USART_BUFFER_EVENT event, DRV_USART_BUFFER_HANDLE bufferHandle, uintptr_t context) {

    struct ZwUsartInput* input = (struct ZwUsartInput*)context;
    
    switch(event) {
        case DRV_USART_BUFFER_EVENT_COMPLETE: 
            input->State = ZW_USART_STATE_READ_COMPLETE;
            input->Error = DRV_USART_ERROR_NONE;
            
            input->NextIndex++;
            if(input->NextIndex >= configUSARTREADER_BUFFER_SIZE)
                input->NextIndex = 0;
            
            DRV_USART_ReadBufferAdd(
                input->driver,
                &input->InputBuffer[input->NextIndex],
                1, // configUSARTREADER_BUFFER_SIZE,
                &input->bufferHandle
            );
            
            if(input->bufferHandle == DRV_USART_BUFFER_HANDLE_INVALID) {
                input->State = ZW_USART_STATE_ERR_INIT;
                input->Error = DRV_USART_ErrorGet(input->bufferHandle);
            } else
                input->State = ZW_USART_STATE_READY;
            
            BaseType_t xHigherPriorityTaskWoken = pdFALSE;
            xTaskNotifyFromISR(
                xDataProcessor_Task, 
                input->FlagId,
                eSetBits,    
                &xHigherPriorityTaskWoken);
            
            portEND_SWITCHING_ISR(xHigherPriorityTaskWoken);
            
            break;
        case DRV_USART_BUFFER_EVENT_ERROR:
            input->State = ZW_USART_STATE_ERR; 
            input->Error = DRV_USART_ErrorGet(input->bufferHandle);
            break;
        case DRV_USART_BUFFER_EVENT_PENDING:
            input->State = ZW_USART_STATE_READING;
            input->Error = DRV_USART_ERROR_NONE;
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
