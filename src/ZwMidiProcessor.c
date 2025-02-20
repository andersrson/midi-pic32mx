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

#include <assert.h>

#include "app.h"

#include "ZwMidiProcessor.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

#define ZW_OK 0
#define ZW_ERR 1
#define ZW_OUT_OF_BOUNDS 2

#define ZW_READER_COUNT (configPINREADER_COUNT + configUSARTREADER_COUNT)
#define ZW_MIDI_OUTBOUND_MSG_QUEUE_SIZE 16

static struct ZwMidiSource Sources[ZW_READER_COUNT];

static struct ZwMidiMessage* MessageQueue[ZW_MIDI_OUTBOUND_MSG_QUEUE_SIZE];
static uint8_t MessageQueueAddNextIndex = 0;
static uint8_t MessageQueueUnreadNextIndex = 0;

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

// *****************************************************************************


struct ZwMidiMessage* ZwMidiNextUnreadMessage() {
    
    for(uint8_t i = 0; i < ZW_MIDI_OUTBOUND_MSG_QUEUE_SIZE; i++) {
        struct ZwMidiMessage* msg = MessageQueue[MessageQueueUnreadNextIndex++];
        if(MessageQueueUnreadNextIndex >= ZW_MIDI_OUTBOUND_MSG_QUEUE_SIZE)
            MessageQueueUnreadNextIndex = 0;
        
        if(msg != NULL && msg->IsHandled == false && msg->IsComplete == true)
            return msg;
    }
    
    return NULL;
}

struct ZwMidiMessage* ZwMidiNextUnreadRealtimeMessage() {
    
    return NULL;
}

void ZwMidiProcessorInitialize() {
    for(int i = 0; i < ZW_READER_COUNT; i++) {
        struct ZwMidiSource* src = &Sources[i];
        src->FlagId = 1 << i;
        src->IncomingMessage = &src->Queue[0];
        src->LatestMessage = &src->Queue[0];
        src->QueueNextIndex = 1;
        
        for(int n = 0; n < ZW_MIDI_SOURCE_MSG_QUEUE_SIZE; n++) {
            src->Queue[n].MidiStatus = 0;
            src->Queue[n].NextIndex = 1;
            src->Queue[n].IsHandled = false;
            src->Queue[n].IsComplete = false;
            for(int k = 0; k < ZW_MIDI_DATA_BUFSIZE; k++) {
                src->Queue[n].MidiData[k] = 0;
            }
        }
    }
}

void ZwMidiProcessorRegisterReader(uint8_t flagId, uintptr_t reader, ZW_MIDI_PROCESSOR_DATA_CALLBACK cb) {
    for(int i = 0; i < ZW_READER_COUNT; i++) {
        if(Sources[i].FlagId == flagId) {
            Sources[i].GetLatestByte = cb;
            Sources[i].Source = reader;
        }
    }
}

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */
void ZwMidiProcessorTask(void) {
    
    uint32_t notifyValue = 0;
    uint32_t interruptStatus = 0;
    
    GPIO_RA1_Clear();
    GPIO_RB2_Clear();
    
    while(true) {
        //GPIO_RA1_Clear();
        //GPIO_RB2_Clear();
        
        notifyValue = xTaskNotifyWait(
                0x00,
                ULONG_MAX,
                &interruptStatus,
                portMAX_DELAY);
        
        if(notifyValue == pdFALSE) {
            // Wait timeout
            continue;
        }
        
        for(int i = 0; i < ZW_READER_COUNT; i++) {
            struct ZwMidiSource* reader = (struct ZwMidiSource*) &Sources[i];
            
            if((reader->FlagId | interruptStatus) == reader->FlagId) {
                uint8_t byte = reader->GetLatestByte(reader->Source);
                struct ZwMidiMessage* msg = NULL;
                
                __conditional_software_breakpoint(reader->IncomingMessage != NULL);
                __conditional_software_breakpoint(reader->LatestMessage != NULL);
                
                if(MIDI_IS_REALTIME(byte)) {
                    //__conditional_software_breakpoint(false);
                    // TODO: implement realtime
                    return;
                } else if(MIDI_IS_SYSEX(byte)) {
                    // all further data bytes ignored until we get a new 
                    // status byte. TODO 
                    reader->IsSysexMode = true;
                    return;
                }
                
                msg = reader->IncomingMessage;
                    
                if(MIDI_IS_STATUS_BYTE(byte)) {
                    
                    if(msg->MidiStatus != 0 || msg->MessageType != NULL) {
                        // New status byte before current message finished
                        
                        __conditional_software_breakpoint(false);
                    }
                    
                    msg->MidiStatus = byte;
                    msg->MidiData[0] = byte;
                    
                    //MIDI_GET_MSG_TYPE(byte, msg->MessageType);
                    
                    msg->MessageType = ZwMidiGetMessageData(byte);
                    
                    __conditional_software_breakpoint(msg->MessageType != NULL);
                    
                    GPIO_RB2_Toggle();
                    
                } else {
                    
                    if(reader->IsSysexMode == true) {
                        return;
                    }
                    __conditional_software_breakpoint(msg->NextIndex < ZW_MIDI_DATA_BUFSIZE);
                    if(msg->MessageType == NULL) {
                        // Invalid message - reject and return
                        __conditional_software_breakpoint(false);
                        return;
                    }
                    
                    msg->MidiData[msg->NextIndex++] = byte;
                    if(msg->NextIndex >= ZW_MIDI_DATA_BUFSIZE)
                        msg->NextIndex = 0;
                    
                }
                
                int8_t dataByteCount = msg->NextIndex - 1;
                if(dataByteCount >= msg->MessageType->MessageLength) {
                    __conditional_software_breakpoint(reader->QueueNextIndex < ZW_MIDI_SOURCE_MSG_QUEUE_SIZE);

                    msg->IsComplete = true;
                    reader->LatestMessage = reader->IncomingMessage;
                    reader->IncomingMessage = &reader->Queue[reader->QueueNextIndex++];
                    if(reader->QueueNextIndex >= ZW_MIDI_SOURCE_MSG_QUEUE_SIZE)
                        reader->QueueNextIndex = 0;

                    msg = reader->IncomingMessage;
                    msg->IsHandled = false;
                    msg->IsComplete = false;
                    msg->MessageType = NULL;
                    msg->NextIndex = 1;
                    msg->MidiStatus = 0;
                    msg->MidiData[0] = 0;
                    msg->MidiData[1] = 0;
                    msg->MidiData[2] = 0;
                    msg->MidiData[3] = 0;

                    __conditional_software_breakpoint(MessageQueueAddNextIndex < ZW_MIDI_OUTBOUND_MSG_QUEUE_SIZE);

                    MessageQueue[MessageQueueAddNextIndex++] = reader->LatestMessage;
                    if(MessageQueueAddNextIndex >= ZW_MIDI_OUTBOUND_MSG_QUEUE_SIZE)
                        MessageQueueAddNextIndex = 0;
                }
            }
        }

        //if(interruptStatus != 0) {
        //    GPIO_RA1_Toggle();
        //}
    }
}


/* *****************************************************************************
 End of File
 */
