# ICT_Inspection_MCU_Program
STM32F4XX program in C for Inspection system.

This is a program that performs Inspection of PCBs.
For this program to run, the USB drive must be attached to MCU. The USB drive has spec files in certain format.
Without the spec files, the system cannot run.
When the system turns on, it first checks the spec data from files on USB disk.
To run this application, there is a separate bootloader program which is not present on this project.

