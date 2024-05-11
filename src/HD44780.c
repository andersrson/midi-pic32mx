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

#include <stdbool.h>

// FreeRTOS - task related headers
#include "definitions.h"

#include "HD44780.h"

const static TickType_t delay50ms = (50 / portTICK_PERIOD_MS);
const static TickType_t delay5ms = (10 / portTICK_PERIOD_MS);

struct HD44780Config {
    uint8_t Rows;
    uint8_t Cols;
    uint8_t Flags;
};

struct HD44780Instance {
    void* SendContext;
    uint8_t InstanceId;
    HD44780SendData SendFunction;
    struct HD44780Config Config;
};


struct HD44780Instance instances[configHD44780_NUM_DISPLAYS];
static uint8_t nextUnallocatedInstance = 0;

static uint8_t byte_upper, byte_lower;
static uint8_t i2c_cmd_bytes[4];
static uint8_t i2c_data_bytes[4];

struct HD44780Instance* HD44780AllocInstance(uint8_t displayId, void* sendContext, HD44780SendData func) {
    if(nextUnallocatedInstance >= configHD44780_NUM_DISPLAYS)
        return NULL;
    
    instances[nextUnallocatedInstance].InstanceId = displayId;
    instances[nextUnallocatedInstance].SendContext = sendContext;
    instances[nextUnallocatedInstance].SendFunction = func;
    
    return &instances[nextUnallocatedInstance++];
}

bool LocalHD44780SendCommand(struct HD44780Instance* instance, uint8_t cmd) {
    
    byte_lower = (cmd << 4) & 0xf0;
    byte_upper = cmd & 0xf0;
    i2c_cmd_bytes[0] = byte_upper | HD44780_CMD_ON;
    i2c_cmd_bytes[1] = byte_upper | HD44780_CMD_OFF;
    i2c_cmd_bytes[2] = byte_lower | HD44780_CMD_ON;
    i2c_cmd_bytes[3] = byte_lower | HD44780_CMD_OFF;
    
    return instance->SendFunction(
        instance->InstanceId, 
        instance->SendContext,
        i2c_cmd_bytes,
        4
    );
    //return DRV_I2C_WriteTransfer(sysObj.i2cHandle, 0x27, i2c_bytes, 4);
}

bool LocalHD44780SendData(struct HD44780Instance* instance, const char* data) {
    
    const char* next = data;
    
    while(*next != '\0') {
        char nextByte = *next++;
        
        byte_lower = (nextByte << 4) & 0xf0;
        byte_upper = nextByte & 0xf0;
        i2c_data_bytes[0] = byte_upper | HD44780_DATA_ON;
        i2c_data_bytes[1] = byte_upper | HD44780_DATA_OFF;
        i2c_data_bytes[2] = byte_lower | HD44780_DATA_ON;
        i2c_data_bytes[3] = byte_lower | HD44780_DATA_OFF;
        
        bool send = instance->SendFunction(
            instance->InstanceId, 
            instance->SendContext,
            i2c_data_bytes,
            4
        );
        if(send == false)
            return false;
       
    }
    
    return true;
}

bool HD44780Initialize(struct HD44780Instance* instance) {
    
    // Must wait 40 ms after reaching 2.7V
    vTaskDelay(delay50ms);
    
    if(LocalHD44780SendCommand(instance, HD44780_CMD_HOME) == false)
        return false;
    
    // Wait min 4.1ms
    vTaskDelay(delay5ms);
    
    if(LocalHD44780SendCommand(instance, HD44780_CMD_HOME) == false)
        return false;
    
    // Wait min 100us
    vTaskDelay(delay5ms);    
    
    if(LocalHD44780SendCommand(instance, HD44780_CMD_HOME) == false)
        return false;
    
    vTaskDelay(delay5ms);    
    
    // No more waiting
    if(LocalHD44780SendCommand(instance, HD44780_CMD_FUNCTION | HD44780_FUNCTION_4BIT | HD44780_FUNCTION_2LINE | HD44780_FUNCTION_8DOT) == false)
        return false;
    
    if(LocalHD44780SendCommand(instance, HD44780_CMD_DISPLAY | HD44780_DISPLAY_ON | HD44780_CURSOR_ON | HD44780_CURSOR_BLINK_ON) == false)
        return false;
    
    if(LocalHD44780SendCommand(instance, HD44780_CMD_ENTRY | HD44780_ENTRY_SHIFT_OFF | HD44780_ENTRY_DIR_INCR) == false)
        return false;
    
    //if(HD44780SendCommand(HD44780_CMD_SHIFT | HD44780_SHIFT_RIGHT | HD44780_SHIFT_CURSOR) == false)
    //    return false;
    
    if(LocalHD44780SendCommand(instance, HD44780_CMD_CLRSCR) == false)
        return false;
    
    if(LocalHD44780SendCommand(instance, HD44780_CMD_HOME) == false)
        return false;
    
    vTaskDelay(delay5ms);
    
    
    return true;
}

void HD44780SetConfig(struct HD44780Instance* instance) {
    
}

bool HD44780PrintString(struct HD44780Instance* instance, const char* string) {
    return LocalHD44780SendData(instance, string);
}

bool HD44780ClearScreen(struct HD44780Instance* instance) {
    return LocalHD44780SendCommand(instance, HD44780_CMD_CLRSCR);
}

bool HD44780Home(struct HD44780Instance* instance) {
    return LocalHD44780SendCommand(instance, HD44780_CMD_HOME);
}
