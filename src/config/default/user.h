/*******************************************************************************
  User Configuration Header

  File Name:
    user.h

  Summary:
    Build-time configuration header for the user defined by this project.

  Description:
    An MPLAB Project may have multiple configurations.  This file defines the
    build-time options for a single configuration.

  Remarks:
    It only provides macro definitions for build-time configuration options

*******************************************************************************/

#ifndef USER_H
#define USER_H

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: User Configuration macros
// *****************************************************************************
// *****************************************************************************

#define configHD44780_NUM_DISPLAYS              (1U)

#define configPINREADER_COUNT                   (2U)
#define configPINREADER_BUFFER_SIZE             (256U)

#define configUSARTREADER_COUNT                   (2U)
#define configUSARTREADER_BUFFER_SIZE             (256U)
    
#define configOUTPUT_COUNT                      (4U)
#define configOUTPUT_BUFFER_SIZE                (128U)

#define configTASK_STACK_SIZE                   (1024)
    
#define configMAX_USER_PROJECTS                 (2U)
    
//DOM-IGNORE-BEGIN
#ifdef __cplusplus
}
#endif
//DOM-IGNORE-END

#endif // USER_H
/*******************************************************************************
 End of File
*/
