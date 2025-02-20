/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Anders Runesson

  @File Name
    midi.c

  @Summary
    MIDI definitions

  @Description
 */
/* ************************************************************************** */


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

#include "ZwMidi.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
/* ************************************************************************** */


// *****************************************************************************
// *****************************************************************************
// Section: Data Types
// *****************************************************************************
// *****************************************************************************


const struct ZwMidiMessageData MidiMessageDefinitionsChannel[MIDI_STRINGS_MAX_CHANNEL_TYPES] = {
    { 
        .ShortName = "nOff",
        .FullName = "Note Off",
        .Class = ZwMidiMessageClassChannel,
        .Type = ZwMidiMessageTypeNoteOff,
        .MessageLength = 2
    },
    { 
        .ShortName = "nOn",
        .FullName = "Note On",
        .Class = ZwMidiMessageClassChannel,
        .Type = ZwMidiMessageTypeNoteOn,
        .MessageLength = 2
    },
    { 
        .ShortName = "PAT",
        .FullName = "Polyphonic Aftertouch",
        .Class = ZwMidiMessageClassChannel,
        .Type = ZwMidiMessageTypePolyphonicPressure,
        .MessageLength = 2
    },
    { 
        .ShortName = "CC",
        .FullName = "Control/Mode Change",
        .Class = ZwMidiMessageClassChannel,
        .Type = ZwMidiMessageTypeControlChange,
        .MessageLength = 2
    },
    { 
        .ShortName = "PC",
        .FullName = "Program Change",
        .Class = ZwMidiMessageClassChannel,
        .Type = ZwMidiMessageTypeProgramChange,
        .MessageLength = 1
    },
    { 
        .ShortName = "ChAT",
        .FullName = "Channel Aftertouch",
        .Class = ZwMidiMessageClassChannel,
        .Type = ZwMidiMessageTypeChannelPressure,
        .MessageLength = 1
    },
    { 
        .ShortName = "Bnd",
        .FullName = "Pitch Bend",
        .Class = ZwMidiMessageClassChannel,
        .Type = ZwMidiMessageTypePitchBend,
        .MessageLength = 2
    }
};

const struct ZwMidiMessageData MidiMessageDefinitionsSystem[MIDI_STRINGS_MAX_SYSTEM_TYPES] = {
    { 
        .ShortName = "SOEX",
        .FullName = "Start of SysEx",
        .Class = ZwMidiMessageClassSysEx,
        .Type = ZwMidiMessageTypeSysExSOE,
        .MessageLength = UINT8_MAX
    },
    { 
        .ShortName = "TmCd",
        .FullName = "Midi Time Code",
        .Class = ZwMidiMessageClassSysCommon,
        .Type = ZwMidiMessageTypeTimeCode,
        .MessageLength = 1
    },
    { 
        .ShortName = "SPos",
        .FullName = "Song Position",
        .Class = ZwMidiMessageClassSysCommon,
        .Type = ZwMidiMessageTypeSongPosition,
        .MessageLength = 2
    },
    { 
        .ShortName = "SSel",
        .FullName = "Song Select",
        .Class = ZwMidiMessageClassSysCommon,
        .Type = ZwMidiMessageTypeSongSelect,
        .MessageLength = 1
    },
    { 
        .ShortName = "UDef",
        .FullName = "Undefined",
        .Class = ZwMidiMessageClassSysCommon,
        .Type = ZwMidiMessageTypeUndefined,
        .MessageLength = 0
    },
    { 
        .ShortName = "UDef",
        .FullName = "Undefined",
        .Class = ZwMidiMessageClassSysCommon,
        .Type = ZwMidiMessageTypeUndefined2,
        .MessageLength = 0
    },
    { 
        .ShortName = "Tune",
        .FullName = "Tuning request",
        .Class = ZwMidiMessageClassSysCommon,
        .Type = ZwMidiMessageTypeTuneRequest,
        .MessageLength = 0
    },
    { 
        .ShortName = "EOEX",
        .FullName = "End Of SysEx",
        .Class = ZwMidiMessageClassSysEx,
        .MessageLength = 0
    },
    { 
        .ShortName = "TmCk",
        .FullName = "Timing Clock",
        .Class = ZwMidiMessageClassSysRealTime,
        .Type = ZwMidiMessageTypeSysExEOE,
        .MessageLength = 0
    },
    { 
        .ShortName = "UDef",
        .FullName = "Undefined",
        .Class = ZwMidiMessageClassSysRealTime,
        .Type = ZwMidiMessageTypeUndefined3,
        .MessageLength = 0
    },
    { 
        .ShortName = "Strt",
        .FullName = "Start",
        .Class = ZwMidiMessageClassSysRealTime,
        .Type = ZwMidiMessageTypeStart,
        .MessageLength = 0
    },
    { 
        .ShortName = "Cont",
        .FullName = "Continue",
        .Class = ZwMidiMessageClassSysRealTime,
        .Type = ZwMidiMessageTypeContinue,
        .MessageLength = 0
    },
    { 
        .ShortName = "Stop",
        .FullName = "Stop",
        .Class = ZwMidiMessageClassSysRealTime,
        .Type = ZwMidiMessageTypeStop,
        .MessageLength = 0
    },
    { 
        .ShortName = "UDef",
        .FullName = "Undefined",
        .Class = ZwMidiMessageClassSysRealTime,
        .Type = ZwMidiMessageTypeUndefined4,
        .MessageLength = 0
    },
    { 
        .ShortName = "ASns",
        .FullName = "Active Sensing",
        .Class = ZwMidiMessageClassSysRealTime,
        .Type = ZwMidiMessageTypeActiveSense,
        .MessageLength = 0
    },
    { 
        .ShortName = "Rset",
        .FullName = "System Reset",
        .Class = ZwMidiMessageClassSysRealTime,
        .Type = ZwMidiMessageTypeReset,
        .MessageLength = 0
    },
};

const char* MidiNoteSharps[MIDI_STRINGS_MAX_NOTES] = {
    "c",
    "c#",
    "d",
    "d#",
    "e",
    "f",
    "f#",
    "g",
    "g#",
    "a",
    "a#",
    "b",         
};
const char* MidiNoteFlats[MIDI_STRINGS_MAX_NOTES] = {
    "c",
    "db",
    "d",
    "eb",
    "e",
    "f",
    "gb",
    "g",
    "ab",
    "a",
    "bb",
    "b",         
};


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

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */


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

const struct ZwMidiMessageData* ZwMidiGetMessageData(uint8_t statusByte) {
    if(MIDI_IS_STATUS_BYTE(statusByte) && MIDI_IS_CHANNEL(statusByte)) { 
        return &MidiMessageDefinitionsChannel[(statusByte & MIDI_STATUS_TYPE_MASK) >> 4]; 
    } else { 
        return &MidiMessageDefinitionsSystem[(statusByte & MIDI_STATUS_TYPE_MASK)];
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
