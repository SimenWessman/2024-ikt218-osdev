#include <libc/stdio.h>
#include "kernel/memory.h"
#include <libc/stdarg.h>
#include <libc/stdbool.h>
#include <libc/string.h>

#define MAX_LINES 25
#define MAX_COLS 80
#define BUFFER_SIZE 100 // Size of the scrollback buffer in lines

volatile char *video = (volatile char*)0xB8000; // VGA text buffer.

unsigned short screen_buffer[BUFFER_SIZE][MAX_COLS];  // buffer to store lines of text
int buffer_start = 0;  // index of the first line in the buffer
int buffer_end = 0;    // index of the last line in the buffer
int current_line = 0;  // index of the current line being displayed

// Declare the cursor position globally
static int cursorX = 0, cursorY = 0;

// Updates the cursor position on the screen. Used for newlines and writing.
void update_cursor(int row, int col) 
{
    unsigned short position = (row * MAX_COLS) + col;

    // Tell the VGA board we are setting the high cursor byte.
    outb(0x3D4, 0x0E);
    outb(0x3D5, (unsigned char)((position >> 8) & 0xFF));

    // Tell the VGA board we are setting the low cursor byte.
    outb(0x3D4, 0x0F);
    outb(0x3D5, (unsigned char)(position & 0xFF));
}

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

void put_char_in_buffer(int x, int y, char c)
{
    screen_buffer[(buffer_start + y) % BUFFER_SIZE][x] = c | (0x09 << 8); // assuming white text on black background
}

void clear_buffer_line(int line)
{
    memset(screen_buffer[line], 0, MAX_COLS * 2);
}

void update_screen_from_buffer()
{
    for (int y = 0; y < MAX_LINES; y++) 
    {
        memcpy((void *)(video + y * MAX_COLS * 2), (void *)(screen_buffer[(current_line + y) % BUFFER_SIZE]), MAX_COLS * 2);
    }
}

void scroll_screen_up() {
    buffer_start = (buffer_start + 1) % BUFFER_SIZE;
    buffer_end = (buffer_end + 1) % BUFFER_SIZE;
    clear_buffer_line(buffer_end);
    current_line = buffer_start;
    update_screen_from_buffer();
}

int putchar(char c) 
{
    const int maxCols = MAX_COLS;
    unsigned char attributeByte = (0 /*background color*/ << 4) | (9 /*foreground color*/ & 0x0F);
    unsigned short attribute = attributeByte << 8;

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
                    if ((screen_buffer[(buffer_start + cursorY) % BUFFER_SIZE][i] & 0xFF) != ' ') 
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
        put_char_in_buffer(cursorX, cursorY, ' ');  // Clear the character at cursor
    } 
    // Any other character should be printed as usual
    else 
    {
        put_char_in_buffer(cursorX, cursorY, c);
        cursorX++;
    }
    
    // If cursor is at the end of the screen, move to the next line
    if (cursorX >= maxCols) 
    {
        cursorX = 0;
        cursorY++;
    }

    // Scroll the screen if the cursor goes past the bottom
    if (cursorY >= MAX_LINES)
    {
        scroll_screen_up();
        cursorY = MAX_LINES - 1;
    }

    // Update the screen from the buffer
    update_screen_from_buffer();

    // Update cursor position on screen
    update_cursor(cursorY, cursorX);
    return c;
}

int puts(const char *str) 
{
    while(*str) {
        putchar(*str++);
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
                case 'x':
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
