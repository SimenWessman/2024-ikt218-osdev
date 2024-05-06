// -----------------------------------------------------------------------------
// File: 11_OS_Expert/src/keyboard.c
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description: 
// This file implements the keyboard handling logic for the operating
// system. It includes functions for handling keyboard interrupts,
// managing scancode to character translation, and configuring keyboard
// interrupts within the system's Programmable Interrupt Controller (PIC).
// The handling includes detecting and responding to key presses and releases,
// managing shift state, and ultimately allowing for character input
// processing in the system.
// -----------------------------------------------------------------------------

#include "keyboard.h"
#include "libc/stdio.h"
#include "libc/stdint.h"

// Global array defining ASCII characters for each scancode. Index corresponds to
// the scancode emitted by the keyboard, and the value at that index is the character.
char scancode_to_char[128] = 
{
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8',  /* 9 */
    '9', '0', '-', '=', '\b',  /* \b -> Backspace */
    '\t',         /* \t -> Tab */
    'q', 'w', 'e', 'r',   /* 19 */
    't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n',  /* \n -> Enter key */
    0,          /* 29   - Control */
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';',  /*  ; -> 39 */
    '\'', '`', 0,        /* Left shift */
    '\\', 'z', 'x', 'c', 'v', 'b', 'n',            /* \\ -> 49 */
    'm', ',', '.', '/', 0,              /* Right shift */
    '*',
    0,  /* Alt */
    ' ',  /* Space bar */
    0,  /* Caps lock */
};

// Alternative scancode array used when the shift key is active.
char scancode_to_char_shift[128] =
{
    0,   27,  '!', '@', '#', '$', '%', '^', '&', '*',  /* 1-9 */
    '(', ')', '_', '+', '\b', /* Backspace */
    '\t', /* Tab */
    'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', /* Enter key */
    0,   /* 29   - Control */
    'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', /* ; : */
    '"', '~', 0,   /* Left shift */
    '|', 'Z', 'X', 'C', 'V', 'B', 'N', /* 49 */
    'M', '<', '>', '?', 0,   /* Right shift */
    '*',
    0,   /* Alt */
    ' ', /* Space bar */
    0,   /* Caps lock */
    // More mappings can be added here for other keys
};

bool shift_pressed = false;  // State variable indicating whether shift key is active.

// Handles keyboard interrupts by reading the scancode from the keyboard's data port
// and translating it into an ASCII character if applicable.
void keyboard_handler() 
{
    __asm__("cli");  // Disable interrupts to handle the keyboard input atomically.

    uint8_t scancode = inb(0x60);  // Read scancode from keyboard data port.
    bool key_released = scancode & 0x80;  // Check if the key was released.

    if (!key_released) 
    {
        if (scancode == 0x2A || scancode == 0x36) 
        {
            shift_pressed = true;  // Shift key pressed.
        } 
        else if (scancode == 0xAA || scancode == 0xB6) 
        {
            shift_pressed = false;  // Shift key released.
        } 
        else 
        {
            char ch = (shift_pressed) ? scancode_to_char_shift[scancode] : scancode_to_char[scancode];
            if (ch) 
            {
                putchar(ch);  // Output the character.
            }
        }
    } 
    else if (scancode == (0x2A + 0x80) || scancode == (0x36 + 0x80)) 
    {
        shift_pressed = false;  // Reset shift status on release.
    }

    outb(0x20, 0x20);  // Send EOI (End Of Interrupt) signal to the PIC.
    __asm__("sti");  // Re-enable interrupts.
}

// Enables the keyboard interrupt on IRQ1 by clearing the appropriate mask bit.
void enable_keyboard_interrupt() 
{
    uint8_t mask = inb(0x21);  // Read the current mask from the PIC.
    mask &= ~(1 << 1);  // Clear the mask bit for IRQ1 (keyboard interrupt).
    outb(0x21, mask);  // Write the new mask to the PIC.
}

// Returns the character corresponding to the given scancode, respecting shift status.
char keyboard_scancode_to_char(uint8_t scancode)
{
    return shift_pressed ? scancode_to_char_shift[scancode] : scancode_to_char[scancode];
}