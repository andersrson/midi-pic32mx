/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Anders Runesson

  @File Name
    DataProcessor.h

  @Summary
    Applies filtering, modifiers, and routing of MIDI messages.

  @Description
 */
/* ************************************************************************** */

#ifndef _ZWDATAPROCESSOR_H    /* Guard against multiple inclusion */
#define _ZWDATAPROCESSOR_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */


/* Provide C++ Compatibility */
#ifdef __cplusplus
extern "C" {
#endif


    /* ************************************************************************** */
    /* ************************************************************************** */
    /* Section: Constants                                                         */
    /* ************************************************************************** */
    /* ************************************************************************** */

#define ZW_MIDI_DATA_BUFSIZE 4
#define ZW_MIDI_SOURCE_MSG_QUEUE_SIZE 2
    
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

typedef uint8_t (*ZW_MIDI_PROCESSOR_DATA_CALLBACK)(uintptr_t reader);

struct ZwMidiMessage {
    const struct ZwMidiMessageData* MessageType;
    uint8_t MidiStatus;
    uint8_t MidiData[ZW_MIDI_DATA_BUFSIZE];
    uint8_t NextIndex;
    
    bool IsHandled;
    bool IsComplete;
};

struct ZwMidiSource {
    uint8_t FlagId;
    uintptr_t Source;
    ZW_MIDI_PROCESSOR_DATA_CALLBACK GetLatestByte;
            
    bool IsSysexMode;
    
    struct ZwMidiMessage Queue[ZW_MIDI_SOURCE_MSG_QUEUE_SIZE];
    uint8_t QueueNextIndex;
    
    struct ZwMidiMessage* IncomingMessage;
    struct ZwMidiMessage* LatestMessage;
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

    /*  A brief description of a section can be given directly below the section
        banner.
     */

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
    
void ZwMidiProcessorInitialize();

struct ZwMidiMessage* ZwMidiNextUnreadMessage();
struct ZwMidiMessage* ZwMidiNextUnreadRealtimeMessage();

void ZwMidiProcessorRegisterReader(uint8_t flagId, uintptr_t reader, ZW_MIDI_PROCESSOR_DATA_CALLBACK cb);
void ZwMidiProcessorTask(void);


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _ZWDATAPROCESSOR_H */

/* *****************************************************************************
 End of File
 */
