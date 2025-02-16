/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Anders Runesson

  @File Name
    DataProcessor.c

  @Summary
    Applies filtering, modifiers, and routing of MIDI messages.

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

#include <stdint.h>
#include <stdbool.h>
#include <limits.h>

#include "config/default/definitions.h"

#include "ZwDataProcessor.h"


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

/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */
void ZwDataProcessorTask(void) {
    uint32_t notifyValue = 0;
    //TickType_t timeout = pdMS_TO_TICKS(100);
    uint32_t interruptStatus = 0;
    
    GPIO_RA1_Clear();
    GPIO_RB2_Clear();
    
    while(true) {
        GPIO_RA1_Clear();
        GPIO_RB2_Clear();
        
        notifyValue = xTaskNotifyWait(
                0x00,
                ULONG_MAX,
                &interruptStatus,
                portMAX_DELAY);
        
        if(notifyValue < 1) {
            continue;
        }
        
        if((interruptStatus & 0x01) != 0) {
            GPIO_RA1_Set();
        }
        if((interruptStatus & 0x02) != 0) {
            GPIO_RB2_Set();
        }
        
        vTaskDelay(timer50);
        
    }
}


/* *****************************************************************************
 End of File
 */
