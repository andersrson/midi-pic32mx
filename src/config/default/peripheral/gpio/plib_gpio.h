/*******************************************************************************
  GPIO PLIB

  Company:
    Microchip Technology Inc.

  File Name:
    plib_gpio.h

  Summary:
    GPIO PLIB Header File

  Description:
    This library provides an interface to control and interact with Parallel
    Input/Output controller (GPIO) module.

*******************************************************************************/

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

#ifndef PLIB_GPIO_H
#define PLIB_GPIO_H

#include <device.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    extern "C" {

#endif
// DOM-IGNORE-END

// *****************************************************************************
// *****************************************************************************
// Section: Data types and constants
// *****************************************************************************
// *****************************************************************************


/*** Macros for GPIO_RA0 pin ***/
#define GPIO_RA0_Set()               (LATASET = (1<<0))
#define GPIO_RA0_Clear()             (LATACLR = (1<<0))
#define GPIO_RA0_Toggle()            (LATAINV= (1<<0))
#define GPIO_RA0_OutputEnable()      (TRISACLR = (1<<0))
#define GPIO_RA0_InputEnable()       (TRISASET = (1<<0))
#define GPIO_RA0_Get()               ((PORTA >> 0) & 0x1)
#define GPIO_RA0_PIN                  GPIO_PIN_RA0

/*** Macros for GPIO_RA1 pin ***/
#define GPIO_RA1_Set()               (LATASET = (1<<1))
#define GPIO_RA1_Clear()             (LATACLR = (1<<1))
#define GPIO_RA1_Toggle()            (LATAINV= (1<<1))
#define GPIO_RA1_OutputEnable()      (TRISACLR = (1<<1))
#define GPIO_RA1_InputEnable()       (TRISASET = (1<<1))
#define GPIO_RA1_Get()               ((PORTA >> 1) & 0x1)
#define GPIO_RA1_PIN                  GPIO_PIN_RA1

/*** Macros for GPIO_RB0 pin ***/
#define GPIO_RB0_Set()               (LATBSET = (1<<0))
#define GPIO_RB0_Clear()             (LATBCLR = (1<<0))
#define GPIO_RB0_Toggle()            (LATBINV= (1<<0))
#define GPIO_RB0_OutputEnable()      (TRISBCLR = (1<<0))
#define GPIO_RB0_InputEnable()       (TRISBSET = (1<<0))
#define GPIO_RB0_Get()               ((PORTB >> 0) & 0x1)
#define GPIO_RB0_PIN                  GPIO_PIN_RB0

/*** Macros for GPIO_RB1 pin ***/
#define GPIO_RB1_Set()               (LATBSET = (1<<1))
#define GPIO_RB1_Clear()             (LATBCLR = (1<<1))
#define GPIO_RB1_Toggle()            (LATBINV= (1<<1))
#define GPIO_RB1_OutputEnable()      (TRISBCLR = (1<<1))
#define GPIO_RB1_InputEnable()       (TRISBSET = (1<<1))
#define GPIO_RB1_Get()               ((PORTB >> 1) & 0x1)
#define GPIO_RB1_PIN                  GPIO_PIN_RB1

/*** Macros for GPIO_RB2 pin ***/
#define GPIO_RB2_Set()               (LATBSET = (1<<2))
#define GPIO_RB2_Clear()             (LATBCLR = (1<<2))
#define GPIO_RB2_Toggle()            (LATBINV= (1<<2))
#define GPIO_RB2_OutputEnable()      (TRISBCLR = (1<<2))
#define GPIO_RB2_InputEnable()       (TRISBSET = (1<<2))
#define GPIO_RB2_Get()               ((PORTB >> 2) & 0x1)
#define GPIO_RB2_PIN                  GPIO_PIN_RB2

/*** Macros for GPIO_RB3 pin ***/
#define GPIO_RB3_Set()               (LATBSET = (1<<3))
#define GPIO_RB3_Clear()             (LATBCLR = (1<<3))
#define GPIO_RB3_Toggle()            (LATBINV= (1<<3))
#define GPIO_RB3_OutputEnable()      (TRISBCLR = (1<<3))
#define GPIO_RB3_InputEnable()       (TRISBSET = (1<<3))
#define GPIO_RB3_Get()               ((PORTB >> 3) & 0x1)
#define GPIO_RB3_PIN                  GPIO_PIN_RB3

/*** Macros for GPIO_RA2 pin ***/
#define GPIO_RA2_Set()               (LATASET = (1<<2))
#define GPIO_RA2_Clear()             (LATACLR = (1<<2))
#define GPIO_RA2_Toggle()            (LATAINV= (1<<2))
#define GPIO_RA2_OutputEnable()      (TRISACLR = (1<<2))
#define GPIO_RA2_InputEnable()       (TRISASET = (1<<2))
#define GPIO_RA2_Get()               ((PORTA >> 2) & 0x1)
#define GPIO_RA2_PIN                  GPIO_PIN_RA2

/*** Macros for GPIO_RA3 pin ***/
#define GPIO_RA3_Set()               (LATASET = (1<<3))
#define GPIO_RA3_Clear()             (LATACLR = (1<<3))
#define GPIO_RA3_Toggle()            (LATAINV= (1<<3))
#define GPIO_RA3_OutputEnable()      (TRISACLR = (1<<3))
#define GPIO_RA3_InputEnable()       (TRISASET = (1<<3))
#define GPIO_RA3_Get()               ((PORTA >> 3) & 0x1)
#define GPIO_RA3_PIN                  GPIO_PIN_RA3

/*** Macros for GPIO_RB4 pin ***/
#define GPIO_RB4_Set()               (LATBSET = (1<<4))
#define GPIO_RB4_Clear()             (LATBCLR = (1<<4))
#define GPIO_RB4_Toggle()            (LATBINV= (1<<4))
#define GPIO_RB4_OutputEnable()      (TRISBCLR = (1<<4))
#define GPIO_RB4_InputEnable()       (TRISBSET = (1<<4))
#define GPIO_RB4_Get()               ((PORTB >> 4) & 0x1)
#define GPIO_RB4_PIN                  GPIO_PIN_RB4

/*** Macros for GPIO_RA4 pin ***/
#define GPIO_RA4_Set()               (LATASET = (1<<4))
#define GPIO_RA4_Clear()             (LATACLR = (1<<4))
#define GPIO_RA4_Toggle()            (LATAINV= (1<<4))
#define GPIO_RA4_OutputEnable()      (TRISACLR = (1<<4))
#define GPIO_RA4_InputEnable()       (TRISASET = (1<<4))
#define GPIO_RA4_Get()               ((PORTA >> 4) & 0x1)
#define GPIO_RA4_PIN                  GPIO_PIN_RA4

/*** Macros for GPIO_RB5 pin ***/
#define GPIO_RB5_Set()               (LATBSET = (1<<5))
#define GPIO_RB5_Clear()             (LATBCLR = (1<<5))
#define GPIO_RB5_Toggle()            (LATBINV= (1<<5))
#define GPIO_RB5_OutputEnable()      (TRISBCLR = (1<<5))
#define GPIO_RB5_InputEnable()       (TRISBSET = (1<<5))
#define GPIO_RB5_Get()               ((PORTB >> 5) & 0x1)
#define GPIO_RB5_PIN                  GPIO_PIN_RB5

/*** Macros for GPIO_RB7 pin ***/
#define GPIO_RB7_Set()               (LATBSET = (1<<7))
#define GPIO_RB7_Clear()             (LATBCLR = (1<<7))
#define GPIO_RB7_Toggle()            (LATBINV= (1<<7))
#define GPIO_RB7_OutputEnable()      (TRISBCLR = (1<<7))
#define GPIO_RB7_InputEnable()       (TRISBSET = (1<<7))
#define GPIO_RB7_Get()               ((PORTB >> 7) & 0x1)
#define GPIO_RB7_PIN                  GPIO_PIN_RB7

/*** Macros for GPIO_RB10 pin ***/
#define GPIO_RB10_Set()               (LATBSET = (1<<10))
#define GPIO_RB10_Clear()             (LATBCLR = (1<<10))
#define GPIO_RB10_Toggle()            (LATBINV= (1<<10))
#define GPIO_RB10_OutputEnable()      (TRISBCLR = (1<<10))
#define GPIO_RB10_InputEnable()       (TRISBSET = (1<<10))
#define GPIO_RB10_Get()               ((PORTB >> 10) & 0x1)
#define GPIO_RB10_PIN                  GPIO_PIN_RB10

/*** Macros for GPIO_RB11 pin ***/
#define GPIO_RB11_Set()               (LATBSET = (1<<11))
#define GPIO_RB11_Clear()             (LATBCLR = (1<<11))
#define GPIO_RB11_Toggle()            (LATBINV= (1<<11))
#define GPIO_RB11_OutputEnable()      (TRISBCLR = (1<<11))
#define GPIO_RB11_InputEnable()       (TRISBSET = (1<<11))
#define GPIO_RB11_Get()               ((PORTB >> 11) & 0x1)
#define GPIO_RB11_PIN                  GPIO_PIN_RB11

/*** Macros for GPIO_RB13 pin ***/
#define GPIO_RB13_Set()               (LATBSET = (1<<13))
#define GPIO_RB13_Clear()             (LATBCLR = (1<<13))
#define GPIO_RB13_Toggle()            (LATBINV= (1<<13))
#define GPIO_RB13_OutputEnable()      (TRISBCLR = (1<<13))
#define GPIO_RB13_InputEnable()       (TRISBSET = (1<<13))
#define GPIO_RB13_Get()               ((PORTB >> 13) & 0x1)
#define GPIO_RB13_PIN                  GPIO_PIN_RB13

/*** Macros for GPIO_RB14 pin ***/
#define GPIO_RB14_Set()               (LATBSET = (1<<14))
#define GPIO_RB14_Clear()             (LATBCLR = (1<<14))
#define GPIO_RB14_Toggle()            (LATBINV= (1<<14))
#define GPIO_RB14_OutputEnable()      (TRISBCLR = (1<<14))
#define GPIO_RB14_InputEnable()       (TRISBSET = (1<<14))
#define GPIO_RB14_Get()               ((PORTB >> 14) & 0x1)
#define GPIO_RB14_PIN                  GPIO_PIN_RB14

/*** Macros for GPIO_RB15 pin ***/
#define GPIO_RB15_Set()               (LATBSET = (1<<15))
#define GPIO_RB15_Clear()             (LATBCLR = (1<<15))
#define GPIO_RB15_Toggle()            (LATBINV= (1<<15))
#define GPIO_RB15_OutputEnable()      (TRISBCLR = (1<<15))
#define GPIO_RB15_InputEnable()       (TRISBSET = (1<<15))
#define GPIO_RB15_Get()               ((PORTB >> 15) & 0x1)
#define GPIO_RB15_PIN                  GPIO_PIN_RB15


// *****************************************************************************
/* GPIO Port

  Summary:
    Identifies the available GPIO Ports.

  Description:
    This enumeration identifies the available GPIO Ports.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all ports are available on all devices.  Refer to the specific
    device data sheet to determine which ports are supported.
*/
#define    GPIO_PORT_A  (0U)
#define    GPIO_PORT_B  (1U)
typedef uint32_t GPIO_PORT;

// *****************************************************************************
/* GPIO Port Pins

  Summary:
    Identifies the available GPIO port pins.

  Description:
    This enumeration identifies the available GPIO port pins.

  Remarks:
    The caller should not rely on the specific numbers assigned to any of
    these values as they may change from one processor to the next.

    Not all pins are available on all devices.  Refer to the specific
    device data sheet to determine which pins are supported.
*/
#define    GPIO_PIN_RA0  (0U)
#define    GPIO_PIN_RA1  (1U)
#define    GPIO_PIN_RA2  (2U)
#define    GPIO_PIN_RA3  (3U)
#define    GPIO_PIN_RA4  (4U)
#define    GPIO_PIN_RB0  (16U)
#define    GPIO_PIN_RB1  (17U)
#define    GPIO_PIN_RB2  (18U)
#define    GPIO_PIN_RB3  (19U)
#define    GPIO_PIN_RB4  (20U)
#define    GPIO_PIN_RB5  (21U)
#define    GPIO_PIN_RB7  (23U)
#define    GPIO_PIN_RB8  (24U)
#define    GPIO_PIN_RB9  (25U)
#define    GPIO_PIN_RB10  (26U)
#define    GPIO_PIN_RB11  (27U)
#define    GPIO_PIN_RB13  (29U)
#define    GPIO_PIN_RB14  (30U)
#define    GPIO_PIN_RB15  (31U)

    /* This element should not be used in any of the GPIO APIs.
       It will be used by other modules or application to denote that none of the GPIO Pin is used */
#define    GPIO_PIN_NONE    (-1)

typedef uint32_t GPIO_PIN;


void GPIO_Initialize(void);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on multiple pins of a port
// *****************************************************************************
// *****************************************************************************

uint32_t GPIO_PortRead(GPIO_PORT port);

void GPIO_PortWrite(GPIO_PORT port, uint32_t mask, uint32_t value);

uint32_t GPIO_PortLatchRead ( GPIO_PORT port );

void GPIO_PortSet(GPIO_PORT port, uint32_t mask);

void GPIO_PortClear(GPIO_PORT port, uint32_t mask);

void GPIO_PortToggle(GPIO_PORT port, uint32_t mask);

void GPIO_PortInputEnable(GPIO_PORT port, uint32_t mask);

void GPIO_PortOutputEnable(GPIO_PORT port, uint32_t mask);

// *****************************************************************************
// *****************************************************************************
// Section: GPIO Functions which operates on one pin at a time
// *****************************************************************************
// *****************************************************************************

static inline void GPIO_PinWrite(GPIO_PIN pin, bool value)
{
    GPIO_PortWrite((GPIO_PORT)(pin>>4), (uint32_t)(0x1) << (pin & 0xFU), (uint32_t)(value) << (pin & 0xFU));
}

static inline bool GPIO_PinRead(GPIO_PIN pin)
{
    return (bool)(((GPIO_PortRead((GPIO_PORT)(pin>>4))) >> (pin & 0xFU)) & 0x1U);
}

static inline bool GPIO_PinLatchRead(GPIO_PIN pin)
{
    return (bool)((GPIO_PortLatchRead((GPIO_PORT)(pin>>4)) >> (pin & 0xFU)) & 0x1U);
}

static inline void GPIO_PinToggle(GPIO_PIN pin)
{
    GPIO_PortToggle((GPIO_PORT)(pin>>4), 0x1UL << (pin & 0xFU));
}

static inline void GPIO_PinSet(GPIO_PIN pin)
{
    GPIO_PortSet((GPIO_PORT)(pin>>4), 0x1UL << (pin & 0xFU));
}

static inline void GPIO_PinClear(GPIO_PIN pin)
{
    GPIO_PortClear((GPIO_PORT)(pin>>4), 0x1UL << (pin & 0xFU));
}

static inline void GPIO_PinInputEnable(GPIO_PIN pin)
{
    GPIO_PortInputEnable((GPIO_PORT)(pin>>4), 0x1UL << (pin & 0xFU));
}

static inline void GPIO_PinOutputEnable(GPIO_PIN pin)
{
    GPIO_PortOutputEnable((GPIO_PORT)(pin>>4), 0x1UL << (pin & 0xFU));
}


// DOM-IGNORE-BEGIN
#ifdef __cplusplus  // Provide C++ Compatibility

    }

#endif
// DOM-IGNORE-END
#endif // PLIB_GPIO_H
