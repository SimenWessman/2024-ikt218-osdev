#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "libc/stdint.h"
#include <libc/stdbool.h>

extern char scancode_to_char[128];

void keyboard_handler(void);

void enable_keyboard_interrupt();

char keyboard_scancode_to_char(uint8_t scancode);

#endif // KEYBOARD_H