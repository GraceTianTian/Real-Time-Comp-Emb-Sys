Repository for the COMP3215 first laboratory exercise. A simple FreeRTOS program was constructed which flashes 2 LEDs at different speeds. The program was constructed in 3 different environments:

## 1) Keil MDK
The project can be opened in Keil. Set up the JLink, build the program and download it.

## 2) MCUXpresso IDE
The project can be opened in MCUXpresso IDE. Build the program, debug it and set up the JLink.

## 3) GNU Toolchain
Navigate to the GNU directory in a terminal/command prompt. Then use:
- make flash to build the program
- make debug to build the program and start GDB and the JLink GDB Server
- make clean to clean the directories

Note that the gcc compilers need to be added to the path and the JLink GDB Server path may need modifying (currently, the default Windows directory is specified). The file removal tool may also need to be changed for Unix users.

## Trace

Also note that the micro trace buffer has not been used yet.
