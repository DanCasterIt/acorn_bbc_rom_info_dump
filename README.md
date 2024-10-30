# Acorn BBC ROM info dump
Software to dump ROM information from a 16k binary ROM image for Acorn MOS

## Why did I made this program?
Have you ever found ROM files on your PC that you don't remember anymore what they are? \
Now you can read the ROM header without an hex dump or flashing it into a real ROM to put on an Acorn BBC machine

## Usage examples
```
$ ./rom_info_reader.macM1 BASICEX ViewJ300lo BASICED135
------------------------------------------------
File: BASICEX
Code entry point: indirect "JMP (&0058)"
Service entry point: absolute "JMP &8043"
Type: Sideways ROM
Load address: 0xFFFF8000
Binary software version: 01
Title: "BASIC Extensions"
Software version: "1.20"
Copyright: "(C)1984 R.T.Florance & R.J.Harrison"
------------------------------------------------
File: ViewJ300lo
Code entry point: absolute "JMP &80BD"
Service entry point: absolute "JMP &8021"
Type: Language ROM
Load address: 0x00008000
Binary software version: 04
Title: "VIEW"
Copyright: "(C)Acornsoft, JGH "
------------------------------------------------
File: BASICED135
Code entry point: absolute "JMP &8083"
Service entry point: absolute "JMP &8033"
Type: Language ROM
Load address: 0x00008000
Binary software version: 35
Title: "The BASIC Editor"
Software version: " 1.35"
Copyright: "(C) 1984 Acornsoft"
------------------------------------------------
```

## To compile the program
```
gcc rom_info_reader.c -o rom_info_reader
```

### Warnings
This program has been tested and compiled on little-endian machines. If you compile it on a big-endian one, you might find all the addresses shown in the reverse order.
Example: `0x80434C00` on little-endian becomes `0x004C4380` on big-endia.

This program has been tested with ROM images for the Acorn BBC master compact. It should work well with ROMs for the rest of the Acorn BBC machines, but I didn't test it.

## Links
- [Code header](https://beebwiki.mdfs.net/Code_header)