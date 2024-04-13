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

void itoa(int n, char s[], int base) 
{
    int i, sign;

    if ((sign = n) < 0) // record sign
        n = -n;         // make n positive
    i = 0;
    do {       // generate digits in reverse order
        s[i++] = n % base + '0';   // get next digit
    } while ((n /= base) > 0);     // delete it
    if (sign < 0)
        s[i++] = '-';
    s[i] = '\0';
    reverse(s);
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

int printf(const char* format, ...) 
{
   va_list args;
   va_start(args, format);

   int count = 0;
   while (*format != '\0') {
       if (*format == '%') {
           format++;
           if (*format == 'd') {
               int num = va_arg(args, int);
               char buffer[32];
               itoa(num, buffer, 10);
               puts(buffer);
               count += strlen(buffer);
           } else if (*format == 's') {
               char* str = va_arg(args, char*);
               puts(str);
               count += strlen(str);
           } else if (*format == 'c') {
               char c = va_arg(args, int);
               putchar(c);
               count++;
           }
       } else {
           putchar(*format);
           count++;
       }
       format++;
   }

   va_end(args);
   return count;
    
    return 0;
}