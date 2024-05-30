# File Structure
1. Makefile: Build automation tool to compile the program.
 
2. Header Files:
- common.h: Common definitions and includes.
- receiver.h: Functions and data structures for the receiver thread.
- processor.h: Functions and data structures for the processor threads.
- responder.h: Functions and data structures for the responder thread.


Source Files:
main.c
receiver.c: Implementation of the receiver thread.
processor.c: Implementation of the processor threads.
responder.c: Implementation of the responder thread


command_program.py: Python script to send commands to the responder thread.
