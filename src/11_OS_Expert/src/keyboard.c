#include "keyboard.h"
#include "libc/stdio.h"
#include "libc/stdint.h"

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

void keyboard_handler(void)
{
    __asm__("cli");
    uint8_t scancode = inb(0x60);
    char ch = scancode < 128 ? scancode_to_char[scancode] : 0;
    if (ch) {
        putchar(ch);  // Print the character corresponding to the key pressed
    }
    outb(0x20, 0x20);  // Slend EOI to PIC
    __asm__("sti");
}

void enable_keyboard_interrupt() 
{
    uint8_t mask = inb(0x21);
    mask &= ~(1 << 1);  // IRQ1 is the second bit, so we clear it to enable
    outb(0x21, mask);
}


char keyboard_scancode_to_char(uint8_t scancode)
{
    return scancode_to_char[scancode];
}
