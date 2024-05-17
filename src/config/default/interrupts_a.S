/*******************************************************************************
  System Interrupts File

  Company:
    Microchip Technology Inc.

  File Name:
    interrupt_a.S

  Summary:
    Raw ISR definitions.

  Description:
    This file maps all the interrupt vectors to their corresponding
    implementations. If a particular module interrupt is used, then its ISR
    definition can be found in corresponding PLIB source file. If a module
    interrupt is not used, then its ISR implementation is mapped to dummy
    handler.
 *******************************************************************************/

// DOM-IGNORE-BEGIN
/*******************************************************************************
* Copyright (C) 2019 Microchip Technology Inc. and its subsidiaries.
*
* Subject to your compliance with these terms, you may use Microchip software
* and any derivatives exclusively with Microchip products. It is your
* responsibility to comply with third party license terms applicable to your
* use of third party software (including open source software) that may
* accompany Microchip software.
*
* THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES, WHETHER
* EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE, INCLUDING ANY IMPLIED
* WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY, AND FITNESS FOR A
* PARTICULAR PURPOSE.
*
* IN NO EVENT WILL MICROCHIP BE LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE,
* INCIDENTAL OR CONSEQUENTIAL LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND
* WHATSOEVER RELATED TO THE SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS
* BEEN ADVISED OF THE POSSIBILITY OR THE DAMAGES ARE FORESEEABLE. TO THE
* FULLEST EXTENT ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN
* ANY WAY RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
* THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *******************************************************************************/
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Included Files
// *****************************************************************************
// *****************************************************************************

#include <xc.h>
#include "ISR_Support.h"

   .extern  TIMER_2_Handler

   .section .vector_8,code, keep
   .equ     __vector_dispatch_8, IntVectorTIMER_2_Handler
   .global  __vector_dispatch_8
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorTIMER_2_Handler

IntVectorTIMER_2_Handler:
    la    $26,  _TIMER_2_Handler
    jr    $26
    nop
    .end    IntVectorTIMER_2_Handler

   .section .TIMER_2_Handler_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _TIMER_2_Handler

_TIMER_2_Handler:
    portSAVE_CONTEXT
    la    s6,  TIMER_2_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end    _TIMER_2_Handler
   .extern  TIMER_3_Handler

   .section .vector_12,code, keep
   .equ     __vector_dispatch_12, IntVectorTIMER_3_Handler
   .global  __vector_dispatch_12
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorTIMER_3_Handler

IntVectorTIMER_3_Handler:
    la    $26,  _TIMER_3_Handler
    jr    $26
    nop
    .end    IntVectorTIMER_3_Handler

   .section .TIMER_3_Handler_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _TIMER_3_Handler

_TIMER_3_Handler:
    portSAVE_CONTEXT
    la    s6,  TIMER_3_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end    _TIMER_3_Handler
   .extern  TIMER_4_Handler

   .section .vector_16,code, keep
   .equ     __vector_dispatch_16, IntVectorTIMER_4_Handler
   .global  __vector_dispatch_16
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorTIMER_4_Handler

IntVectorTIMER_4_Handler:
    la    $26,  _TIMER_4_Handler
    jr    $26
    nop
    .end    IntVectorTIMER_4_Handler

   .section .TIMER_4_Handler_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _TIMER_4_Handler

_TIMER_4_Handler:
    portSAVE_CONTEXT
    la    s6,  TIMER_4_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end    _TIMER_4_Handler
   .extern  TIMER_5_Handler

   .section .vector_20,code, keep
   .equ     __vector_dispatch_20, IntVectorTIMER_5_Handler
   .global  __vector_dispatch_20
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorTIMER_5_Handler

IntVectorTIMER_5_Handler:
    la    $26,  _TIMER_5_Handler
    jr    $26
    nop
    .end    IntVectorTIMER_5_Handler

   .section .TIMER_5_Handler_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _TIMER_5_Handler

_TIMER_5_Handler:
    portSAVE_CONTEXT
    la    s6,  TIMER_5_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end    _TIMER_5_Handler
   .extern  I2C_1_Handler

   .section .vector_33,code, keep
   .equ     __vector_dispatch_33, IntVectorI2C_1_Handler
   .global  __vector_dispatch_33
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  IntVectorI2C_1_Handler

IntVectorI2C_1_Handler:
    la    $26,  _I2C_1_Handler
    jr    $26
    nop
    .end    IntVectorI2C_1_Handler

   .section .I2C_1_Handler_vector_text, code, keep
   .set     nomicromips
   .set     noreorder
   .set     nomips16
   .set     noat
   .ent  _I2C_1_Handler

_I2C_1_Handler:
    portSAVE_CONTEXT
    la    s6,  I2C_1_Handler
    jalr  s6
    nop
    portRESTORE_CONTEXT
    .end    _I2C_1_Handler

