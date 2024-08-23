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

#include "midi.h"

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


const struct MidiString_t MidiStringsStatus[MIDI_STRINGS_MAX_STATUS_TYPES] = {
    { 
        .ShortName = "nOff",
        .FullName = "Note Off"
    },
    { 
        .ShortName = "nOn",
        .FullName = "Note On"
    },
    { 
        .ShortName = "PAT",
        .FullName = "Polyphonic Aftertouch"
    },
    { 
        .ShortName = "CC",
        .FullName = "Control/Mode Change"
    },
    { 
        .ShortName = "PC",
        .FullName = "Program Change"
    },
    { 
        .ShortName = "ChAT",
        .FullName = "Channel Aftertouch"
    },
    { 
        .ShortName = "Bnd",
        .FullName = "Pitch Bend"
    },
    { 
        .ShortName = "Sys",
        .FullName = "System Common"
    },
};

const struct MidiString_t MidiStringsSystem[MIDI_STRINGS_MAX_SYSTEM_TYPES] = {
    { 
        .ShortName = "SysE",
        .FullName = "System Exclusive"
    },
    { 
        .ShortName = "TmCd",
        .FullName = "Midi Time Code"
    },
    { 
        .ShortName = "SPos",
        .FullName = "Song Position"
    },
    { 
        .ShortName = "SSel",
        .FullName = "Song Select"
    },
    { 
        .ShortName = "UDef",
        .FullName = "Undefined"
    },
    { 
        .ShortName = "UDef",
        .FullName = "Undefined"
    },
    { 
        .ShortName = "Tune",
        .FullName = "Tuning request"
    },
    { 
        .ShortName = "EOFX",
        .FullName = "End Of SysEx"
    },
    { 
        .ShortName = "TmCk",
        .FullName = "Timing Clock"
    },
    { 
        .ShortName = "UDef",
        .FullName = "Undefined"
    },
    { 
        .ShortName = "Strt",
        .FullName = "Start"
    },
    { 
        .ShortName = "Cont",
        .FullName = "Continue"
    },
    { 
        .ShortName = "Stop",
        .FullName = "Stop"
    },
    { 
        .ShortName = "UDef",
        .FullName = "Undefined"
    },
    { 
        .ShortName = "ASns",
        .FullName = "Active Sensing"
    },
    { 
        .ShortName = "Rset",
        .FullName = "System Reset"
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


/* *****************************************************************************
 End of File
 */
