// -----------------------------------------------------------------------------
// File: 11_OS_Expert/include/keyboard.h
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description: 
// Header file for keyboard-related functions and data structures.
// This file defines the interfaces for handling keyboard interrupts,
// translating scancodes to characters, and enabling keyboard
// interrupts. These functions are essential for keyboard input
// processing in the operating system.
// -----------------------------------------------------------------------------

#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/stdint.h"  // Include for standard integer types.
#include <libc/stdbool.h> // Include for boolean type.

// Array mapping scancodes to their corresponding ASCII characters.
// This array should be populated with appropriate character values
// corresponding to each keyboard scancode.
extern char scancode_to_char[128];

// Function:
// keyboard_handler
//
// Description: 
// Interrupt handler for keyboard interrupts.
// This function is called automatically when a keyboard interrupt occurs.
// It reads the scancode from the keyboard hardware and processes it,
// typically resulting in updating some type of input buffer or handling
// control keys (like Shift, Ctrl, etc.).
void keyboard_handler(void);

// Function:
// enable_keyboard_interrupt
// 
// Description: 
// Enables keyboard interrupts by modifying the appropriate mask
// in the Programmable Interrupt Controller (PIC). This allows
// keyboard interrupts to be delivered to the CPU, ensuring that
// keypresses result in calls to the keyboard_handler.
void enable_keyboard_interrupt();

// Function: keyboard_scancode_to_char
// Description: Translates a keyboard scancode to its corresponding ASCII character.
//
//   @param uint8_t scancode - The scancode received from the keyboard interrupt.
//
//   @returns char - The corresponding ASCII character for the given scancode, as defined
//          in the scancode_to_char array.
char keyboard_scancode_to_char(uint8_t scancode);

#endif // KEYBOARD_H
