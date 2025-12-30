
# MIDI-Embedded-Player

This is an embedded **class project** using the **STM32 Arm Cortex** controller (STM32L476RG) to process and play various MIDI songs. 

As a part of the SWEN **Software Design for Embedded Computing Systems** class, the software has two modes of operation: Remote and Local. 

In Remote mode, the board reads and parses input commands from the USART. In local mode, the board detects a button press and executes commands based on the number and the timing of the presses. 

There are a total of 4 operations that can be done.
- **PLAY**: Play the current song
- **NEXT**: Cycles to the next song
- **PAUSE**: Pause the song
- **STOP**: Stop the song

Songs are stored as a bunch of bytes within a songs array. Following the MIDI format, audio tracks and events are parsed out according to their variable lengths, and based on the MIDI Channel Voice Messages, notes are played/paused on the timing specified.


# System Design

The system is separated into 6 major components, each with its own distinct functionality. 
- **Project.c**: The class that holds the context of the entire program. 
- **Project_util.c**: Helper functions that are used across the program: buffer handling, state setting, local/remote input parsing, etc.
- **Command.c**: Implementation of the different command functionalities.
- **Parse.c**: Logic of reading and parsing the different songs.
- **Song.c**: Stores the actual songs within an array.
- **Tone.c**: Logic of how to play each song tone.

The following is a diagram detailing the control flow of the system

![Code ](./Codeflow.svg)



## Documentation

- [SWEN 340 Course Site](https://www.se.rit.edu/~swen-340/syllabus.html)
- [MIDI File Format](http://personal.kent.edu/~sbirch/Music_Production/MP-II/MIDI/midi_file_format.htm)


## Special Thanks

Thank you to my professor **Bruce Herring**, as well as my TA **Daniel Catorcini**, for guiding me whenever I am lost and helping me learn so much about embedded systems.
