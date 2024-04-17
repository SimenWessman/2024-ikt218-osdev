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
    // F1-F12, etc, you might want to handle these differently depending on your needs
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    // More mappings can be added here for other keys
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

bool shift_pressed = false;

void keyboard_handler(void) 
{
    __asm__("cli");  // Disable interrupts

    uint8_t scancode = inb(0x60);
    bool key_released = scancode & 0x80;  // Check if the key was released

    if (!key_released) 
    {  // Only process key presses
        if (scancode == 0x2A || scancode == 0x36) 
        {
            shift_pressed = true;  // Shift key pressed
        } 
        
        else if (scancode == 0xAA || scancode == 0xB6) 
        {
            shift_pressed = false;  // Shift key released
        } 
        
        else 
        {
            // Normal key processing
            char ch;
            if(shift_pressed) 
            {
                ch = scancode_to_char_shift[scancode];
            } 
            
            else 
            {
                ch = scancode_to_char[scancode];
            }
        
            if (ch) 
            {
                putchar(ch);  // Print the character corresponding to the key pressed
            }
        }
    } 
    
    else 
    {
        // Here it is possible to add functionality for key release events
        // For now, we do nothing with the release event
        if (scancode == (0x2A + 0x80) || scancode == (0x36 + 0x80)) 
        {
            shift_pressed = false;  // Reset shift status on release
        }
    }

    outb(0x20, 0x20);  // Send EOI
    __asm__("sti");  // Re-enable interrupts
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
