#include <libc/stdio.h>
#include <libc/string.h>
#include <libc/stdarg.h>
#include <libc/stdbool.h>

volatile char *video = (volatile char*)0xB8000; // VGA text buffer.

void reverse(char s[]) 
{
    int i, j;
    char c;

    for (i = 0, j = strlen(s)-1; i < j; i++, j--) {
        c = s[i];
        s[i] = s[j];
        s[j] = c;
    }
}

void itoa(int value, char* str, int base) {
    static char num[] = "0123456789ABCDEF"; // Support for hexadecimal
    char* wstr = str;
    int sign;
    if (base < 2 || base > 16) return; // Check supported base range

    // Handle negative numbers only for base 10
    if ((sign = value) < 0 && base == 10) value = -value;

    // Conversion in reverse order
    do {
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
    const int maxRows = 25, maxCols = 80;
    unsigned char attributeByte = (0 /*background color*/ << 4) | (13 /*foreground color*/ & 0x0F);
    unsigned short attribute = attributeByte << 8;

    // Handle a newline
    if(c == '\n') {
        cursorX = 0;
        cursorY++;
    } else {
        unsigned short *location = video_memory + (cursorY * maxCols + cursorX);
        *location = c | attribute;
        cursorX++;
    }

    // Handle line wrap or scrolling if needed
    if(cursorX >= maxCols) {
        cursorX = 0;
        cursorY++;
    }
    if(cursorY >= maxRows) {
        cursorY = 0; // Implement scrolling or reset cursorY based on your needs
    }

    // Update cursor position
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

int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    int count = 0;
    while (*format != '\0') {
        if (*format == '%') {
            format++; // Skip '%'
            switch (*format) {
                case 'd': {
                    int num = va_arg(args, int);
                    char buffer[32];
                    itoa(num, buffer, 10); // Decimal
                    puts(buffer);
                    count += strlen(buffer);
                    break;
                }
                case 'X': {
                    int num = va_arg(args, int);
                    char buffer[32];
                    itoa(num, buffer, 16); // Hexadecimal
                    puts(buffer);
                    count += strlen(buffer);
                    break;
                }
                case 's': {
                    char* str = va_arg(args, char*);
                    puts(str);
                    count += strlen(str);
                    break;
                }
                case 'c': {
                    char c = (char)va_arg(args, int); // Char are promoted to int
                    putchar(c);
                    count++;
                    break;
                }
                default:
                    putchar('%'); // In case of %% or unknown format specifier
                    putchar(*format);
                    count += 2;
            }
        } else {
            putchar(*format);
            count++;
        }
        format++;
    }

    va_end(args);
    return count;
}