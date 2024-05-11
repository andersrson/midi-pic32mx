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

#ifndef _HD44780_H    /* Guard against multiple inclusion */
#define _HD44780_H

#include <stdint.h>

/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif

#ifndef configHD44780_NUM_DISPLAYS
#error "You must define the constant configHD44780_NUM_DISPLAYS"
#endif
    
typedef uint8_t HD44780_Handle;
    
struct HD44780Config;

#define HD44780_ENTRY_SHIFT_OFF     0x00
#define HD44780_ENTRY_SHIFT_ON      0x01
#define HD44780_ENTRY_DIR_DECR      0x00
#define HD44780_ENTRY_DIR_INCR      0x02

#define HD44780_CURSOR_BLINK_OFF    0x00
#define HD44780_CURSOR_BLINK_ON     0x01
#define HD44780_CURSOR_OFF          0x00
#define HD44780_CURSOR_ON           0x02
#define HD44780_DISPLAY_OFF         0x00
#define HD44780_DISPLAY_ON          0x04

#define HD44780_SHIFT_LEFT          0x00
#define HD44780_SHIFT_RIGHT         0x04
#define HD44780_SHIFT_CURSOR        0x00
#define HD44780_SHIFT_DISPLAY       0x08

#define HD44780_FUNCTION_8DOT       0x00
#define HD44780_FUNCTION_10DOT      0x04
#define HD44780_FUNCTION_1LINE      0x00
#define HD44780_FUNCTION_2LINE      0x08
#define HD44780_FUNCTION_4BIT       0x00
#define HD44780_FUNCTION_8BIT       0x10

#define HD44780_SCROLL_RIGHT        0x1E
#define HD44780_SCROLL_LEFT         0x18

#define HD44780_CMD_CLRSCR          0x01 
#define HD44780_CMD_HOME            0x02 
#define HD44780_CMD_ENTRY           0x04
#define HD44780_CMD_DISPLAY         0x08
#define HD44780_CMD_SHIFT           0x10
#define HD44780_CMD_FUNCTION        0x20

#define HD44780_FLAG_RS             0x01
#define HD44780_FLAG_R_NW           0x02
#define HD44780_FLAG_EN             0x04
#define HD44780_FLAG_LED            0x08

#define HD44780_CMD_ON              (HD44780_FLAG_LED | HD44780_FLAG_EN)
#define HD44780_CMD_OFF             (HD44780_FLAG_LED)
#define HD44780_DATA_ON             (HD44780_FLAG_LED | HD44780_FLAG_EN| HD44780_FLAG_RS)
#define HD44780_DATA_OFF            (HD44780_FLAG_LED | HD44780_FLAG_RS)

#define HD44780_1602_LINE1          0x00
#define HD44780_1602_LINE2          0x10

#define HD44780_2002_LINE1          0x00
#define HD44780_2002_LINE2          0x10

#define HD44780_2004_LINE1          0x00
#define HD44780_2004_LINE2          0x40
#define HD44780_2004_LINE3          0x14
#define HD44780_2004_LINE4          0x54

typedef bool (*HD44780SendData)(uint8_t InstanceId, void* sendCtx, uint8_t* data, uint8_t len);

struct HD44780Instance;

struct HD44780Instance* HD44780AllocInstance(uint8_t displayId, void* sendContext, HD44780SendData func);
bool HD44780Initialize(struct HD44780Instance* instance);
void HD44780SetConfig(struct HD44780Instance* instance);

bool HD44780PrintString(struct HD44780Instance* instance, const char* string);
bool HD44780ClearScreen(struct HD44780Instance* instance);
bool HD44780Home(struct HD44780Instance* instance);

#ifdef __cplusplus
}
#endif

#endif /* _HD44780_H */

/* *****************************************************************************
 End of File
 */
