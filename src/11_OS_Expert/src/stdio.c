// -----------------------------------------------------------------------------
// File: 11_OS_Expert/src/stdio.c
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description: 
// This file contains the implementation of standard input/output
// functions for the operating system, such as putchar, printf, and
// a simple implementation of puts. It also includes utility functions
// like itoa (integer to ASCII conversion) and reverse (for string reversing),
// crucial for number formatting and display.
// -----------------------------------------------------------------------------

#include <libc/stdio.h>
#include <libc/string.h>
#include <libc/stdarg.h>
#include <libc/stdbool.h>

#define MAX_LINES 25
#define MAX_COLS 80

volatile char *video = (volatile char*)0xB8000; // VGA text buffer.

unsigned short screen_buffer[MAX_LINES][MAX_COLS];  // buffer to store lines of text
int buffer_start = 0;  // index of the first line in the buffer
int buffer_end = 0;    // index of the last line in the buffer

// // Selection state variables
// static bool selection_active = false;
// static int selection_start_x = 0, selection_start_y = 0;
// static int selection_end_x = 0, selection_end_y = 0;

// void start_selection(int x, int y) 
// {
//     selection_active = true;
//     selection_start_x = x;
//     selection_start_y = y;
//     selection_end_x = x;
//     selection_end_y = y;
// }

// void update_selection(int x, int y) 
// {
//     if (selection_active) 
//     {
//         selection_end_x = x;
//         selection_end_y = y;
//     }
// }

// void handle_text_selection(int x, int y) 
// {
//     if (!selection_active) 
//     {
//         start_selection(x, y);
//     } 
    
//     else 
//     {
//         update_selection(x, y);
//     }
// }

void reverse(char s[]) 
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i < j; i++, j--) 
    {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int value, char* str, int base) 
{
    static char num[] = "0123456789ABCDEF"; // Support for hexadecimal
    char* wstr = str;
    int sign;
    if (base < 2 || base > 16) return; // Check supported base range

    // Handle negative numbers only for base 10
    if ((sign = value) < 0 && base == 10) value = -value;

    // Conversion in reverse order
    do 
    {
        *wstr++ = num[value % base];
    } while (value /= base);

    if (sign < 0 && base == 10) *wstr++ = '-';
    *wstr = '\0';

    // Reverse string
    reverse(str);
}

// Sends a byte (val) to the hardware port (port).
void outb(unsigned short port, unsigned char val) 
{
    asm volatile ("outb %0, %1" : : "a"(val), "Nd"(port));
}

// Receives a byte from the hardware port (port) and returns said byte.
unsigned char inb(unsigned short port) 
{
    unsigned char result;
    asm volatile ("inb %1, %0" : "=a"(result) : "Nd"(port));
    return result;
}

// Updates the cursor position on the screen. Used for newlines and writing.
void update_cursor(int row, int col) 
{
    unsigned short position = (row * 80) + col;

    // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));

    // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
}

int putchar(char c) 
{
    static unsigned short *video_memory = (unsigned short *)0xB8000;
    static int cursorX = 0, cursorY = 0;
    const int maxCols = 80;  // Typical width for a VGA text mode screen
    unsigned char attributeByte = (0 /*background color*/ << 4) | (9 /*foreground color*/ & 0x0F);
    unsigned short attribute = attributeByte << 8;
    unsigned short *location;

    // if (selection_active && cursorX >= selection_start_x && cursorX <= selection_end_x &&
    //     cursorY >= selection_start_y && cursorY <= selection_end_y) 
    // {
    //     // Adjust the attribute to highlight the background
    //     attribute = (attribute & 0xFF00) | 0x70; // Example: Grey on black background
    // }

    // Newline character should move the cursor to the next line
    if (c == '\n') 
    {
        cursorX = 0;
        cursorY++;
    } 
    
    // Backspace should move the cursor back one space 
    // and clear the character at the cursor.
    else if (c == '\b') 
    {
        if (cursorX == 0) 
        {
            if (cursorY > 0) 
            {
                cursorY--;
                cursorX = maxCols;  // Set cursorX at the end to start checking for non-space characters
                int hasContent = 0;  // Flag to check if the line has any content. Will save us time later.
                for (int i = maxCols-1; i >= 0; i--) 
                {
                    location = video_memory + (cursorY * maxCols + i);
                    if ((*location & 0xFF) != ' ') 
                    {
                        cursorX = i + 1;  // Place cursor right after the last non-space character
                        hasContent = 1;
                        break;
                    }
                }


                if (!hasContent) 
                {  // If no content is found, move cursor to the start of the line
                    cursorX = 0;
                }
            }
        } 
        
        else 
        {
            cursorX--;
        }
        location = video_memory + (cursorY * maxCols + cursorX);
        *location = ' ' | attribute;  // Clear the character at cursor
    } 
    
    // Any other character should be printed as usual
    else 
    {
        location = video_memory + (cursorY * maxCols + cursorX);
        *location = c | attribute;
        cursorX++;
    }
    
    // If cursor is at the end of the screen, move to the next line
    if (cursorX >= maxCols) 
    {
        cursorX = 0;
        cursorY++;
    }

    // If cursor is at bottom of screen, put cursor on top again.
    // Very temporary solution, scrolling will be implemented soon.
    if(cursorY >= MAX_LINES)
    {
        cursorY = 0;
    }


    // Scrolling functionality, needs work.
    // if (cursorY >= 25) {

        
    //     // Scroll the screen
    //     for (int i = 0; i < 24; i++) 
    //     {
    //         memcpy(video_memory + (i * maxCols), video_memory + ((i + 1) * maxCols), maxCols * sizeof(unsigned short));
    //     }
    //     memset(video_memory + (24 * maxCols), 0, maxCols * sizeof(unsigned short));
    //     cursorY = 24;  // Keep cursor on the last line of the screen
    // }

    // Update cursor position on screen
    update_cursor(cursorY, cursorX);
    return c;
}

// bool print(const char* data, size_t length) {
//     int count = 0;
//     while (*str != '\0') {
//         putchar(*str);
//         str++;
//         count++;
//     }
//     return count;
// }

int puts( const char *str) 
{
    while( *str ) {
        putchar( *str++ );
    }
    return 0;
}

int printf(const char* format, ...) 
{
    va_list args;
    va_start(args, format);

    int count = 0;

    // Loop through the format string
    while (*format != '\0') 
    {
        // Check for format specifiers
        if (*format == '%') 
        {
            format++; // Skip '%'
            // Handle the different format specifiers.
            switch (*format) 
            {
                // Integer format specifier.
                case 'd': 
                {
                    int num = va_arg(args, int);
                    char buffer[32];
                    itoa(num, buffer, 10); // Decimal
                    puts(buffer);
                    count += strlen(buffer);
                    break;
                }

                // Hexadecimal format specifier.
                // Useful for debugging and memory addresses.
                case 'X':
                {
                    int num = va_arg(args, int);
                    char buffer[32];
                    itoa(num, buffer, 16); // Hexadecimal
                    puts(buffer);
                    count += strlen(buffer);
                    break;
                }

                // String format specifier.
                case 's': 
                {
                    char* str = va_arg(args, char*);
                    puts(str);
                    count += strlen(str);
                    break;
                }

                // Character format specifier.
                case 'c': 
                {
                    char c = (char)va_arg(args, int); // Char are promoted to int
                    putchar(c);
                    count++;
                    break;
                }

                // Undefined format specifier.
                default:
                    putchar('%'); // In case of %% or unknown format specifier
                    putchar(*format);
                    count += 2;
            }
        } 
        
        else 
        {
            putchar(*format);
            count++;
        }
        format++;
    }

    va_end(args);
    return count;
}