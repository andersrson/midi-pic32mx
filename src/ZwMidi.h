/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Anders Runesson

  @File Name
    midi.h

  @Summary
    MIDI definitions

  @Description
 */
/* ************************************************************************** */

#ifndef _ZWMIDI_H    /* Guard against multiple inclusion */
#define _ZWMIDI_H

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

    
#define MIDI_BAUD_RATE 31250    // Bits per second
#define MIDI_BPS        3125    // Bytes per second - excluding start and stop bits


    /*  
     * MIDI status byte values
     */

#define MIDI_STATUS_BYTE_FLAG                   0x80
#define MIDI_STATUS_TYPE_MASK                   0x70
#define MIDI_STATUS_CHANNEL_MASK                0x0F
#define MIDI_STATUS_UPPER_NIBBLE_MASK           0xF0
#define MIDI_STATUS_LOWER_NIBBLE_MASK           0x0F
    
#define MIDI_STRINGS_MAX_CHANNEL_TYPES           8
#define MIDI_STRINGS_MAX_SYSTEM_TYPES           16
#define MIDI_STRINGS_MAX_NOTES                  12

#define MIDI_CHANNEL_NOTE_OFF                   0x80
#define MIDI_CHANNEL_NOTE_ON                    0x90
#define MIDI_CHANNEL_POL_AT                     0xA0
#define MIDI_CHANNEL_CTRL_CHG                   0xB0
#define MIDI_CHANNEL_PGM_CHG                    0xC0
#define MIDI_CHANNEL_CH_AT                      0xD0
#define MIDI_CHANNEL_PITCH_BEND                 0xE0

#define MIDI_STATUS_SYSTEM                      0xF0

#define MIDI_SYSTEM_REALTIME_FLAG               0x08
#define MIDI_SYSTEM_REALTIME_MASK               0xF8
    
#define MIDI_SYSTEM_EX_SOE                      0x00
    
#define MIDI_SYSTEM_COMMON_TIME_CODE            0x01
#define MIDI_SYSTEM_COMMON_SONG_POS             0x02
#define MIDI_SYSTEM_COMMON_SONG_SEL             0x03
#define MIDI_SYSTEM_COMMON_UNDEF1               0x04
#define MIDI_SYSTEM_COMMON_UNDEF2               0x05
#define MIDI_SYSTEM_COMMON_TUNE                 0x06
    
#define MIDI_SYSTEM_EX_EOE                      0x07
    
#define MIDI_SYSTEM_REALTIME_TIME_CLK           0x08
#define MIDI_SYSTEM_REALTIME_UNDEF3             0x09
#define MIDI_SYSTEM_REALTIME_START              0x0A
#define MIDI_SYSTEM_REALTIME_CONTINUE           0x0B
#define MIDI_SYSTEM_REALTIME_STOP               0x0C
#define MIDI_SYSTEM_REALTIME_UNDEF4             0x0D
#define MIDI_SYSTEM_REALTIME_ACT_SENSE          0x0E
#define MIDI_SYSTEM_REALTIME_RESET              0x0F

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


struct ZwMidiString {
    char* ShortName;
    char* FullName;
};


extern const struct ZwMidiString MidiStringsChannel[MIDI_STRINGS_MAX_CHANNEL_TYPES];
extern const struct ZwMidiString MidiStringsSystem[MIDI_STRINGS_MAX_SYSTEM_TYPES];
extern const char* MidiNoteSharps[MIDI_STRINGS_MAX_NOTES];
extern const char* MidiNoteFlats[MIDI_STRINGS_MAX_NOTES];
extern const char* MidiSysexTypes[MIDI_STRINGS_MAX_SYSTEM_TYPES];

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

//#define MIDI_IS_STATUS_BYTE(byte) ((byte & MIDI_STATUS_BYTE_FLAG) == MIDI_STATUS_BYTE_FLAG && !((byte & MIDI_STATUS_SYSTEM) == MIDI_STATUS_SYSTEM))

#define MIDI_IS_STATUS_BYTE(byte) (byte >= 128)

#define MIDI_IS_CHANNEL(byte) (MIDI_IS_STATUS_BYTE(byte) && ((byte & MIDI_STATUS_UPPER_NIBBLE_MASK) < MIDI_STATUS_SYSTEM)

#define MIDI_IS_NOTE_OFF(byte) ((byte & MIDI_STATUS_UPPER_NIBBLE_MASK) == MIDI_CHANNEL_NOTE_OFF)
#define MIDI_IS_NOTE_ON(byte) ((byte & MIDI_STATUS_UPPER_NIBBLE_MASK) == MIDI_CHANNEL_NOTE_ON)
#define MIDI_IS_NOTE(byte) (MIDI_IS_NOTE_ON(byte) || MIDI_IS_NOTE_OFF(byte))

#define MIDI_IS_SYSTEM(byte) ((byte & MIDI_STATUS_UPPER_NIBBLE_MASK) == MIDI_STATUS_SYSTEM)

#define MIDI_IS_SYSTEM_COMMON(byte) (((byte & MIDI_STATUS_UPPER_NIBBLE_MASK) == MIDI_STATUS_SYSTEM) \
        && ((byte & MIDI_SYSTEM_REALTIME_FLAG) > 0) && ((byte & MIDI_SYSTEM_REALTIME_FLAG) < MIDI_SYSTEM_EX_EOE))

#define MIDI_IS_SYSTEM_SONG_POS(byte) (byte == (MIDI_STATUS_SYSTEM | MIDI_SYSTEM_COMMON_SONG_POS))
#define MIDI_IS_SYSTEM_SONG_SEL(byte) (byte == (MIDI_STATUS_SYSTEM | MIDI_SYSTEM_COMMON_SONG_SEL))

#define MIDI_GET_STATUS_VALUE(byte) (byte & MIDI_STATUS_LOWER_NIBBLE_MASK)
    
#define MIDI_GET_CHANNEL_SHORT_STRING(byte, str)                                 \
do {                                                                            \
    str = MidiStringsChannel[(byte & MIDI_STATUS_TYPE_MASK) >> 4].ShortName;     \
} while (0)

#define MIDI_GET_CHANNEL_FULL_STRING(byte, str)                                 \
do {                                                                            \
    str = MidiStringsChannel[(byte & MIDI_STATUS_TYPE_MASK) >> 4].FullName;     \
} while (0)

#define MIDI_GET_NOTE_STRING(byte, str)                 \
do {                                                    \
    str[0] = MidiNoteSharps[byte % 12][0];              \
    if(MidiNoteSharps[byte % 12][1] != '\0')            \
        str[1] = MidiNoteSharps[byte % 12][1];          \
    str[2] = '0' + byte / 12;                           \
} while (0)

#define MIDI_GET_SYSTEM_STRING_SHORT(byte, str)                                  \
do {                                                                            \
    str = MidiStringsSystem[(byte & MIDI_STATUS_LOWER_NIBBLE_MASK)].ShortName;   \
} while (0)

#define MIDI_GET_SYSTEM_STRING_FULL(byte, str)                                   \
do {                                                                            \
    str = MidiStringsSystem[(byte & MIDI_STATUS_LOWER_NIBBLE_MASK)].FullName;   \
} while (0)


    /* Provide C++ Compatibility */
#ifdef __cplusplus
}
#endif

#endif /* _ZWMIDI_H */

/* *****************************************************************************
 End of File
 */
