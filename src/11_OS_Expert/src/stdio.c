#include <stdarg.h>
#include "libc/stdio.h"

int putchar(int *c) {
    write(1, &c, 1);    // System call for standard output on Linux and OSX
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

int printf(const char* format, ...) {
    va_list args;
    va_start(args, format);

    int count = 0;
    while (*format != '\0') {
        if (*format == '%') {
            format++;
            if (*format == 'd') {
                int num = va_arg(args, int);
                // Implementation of printing integers goes here
                // ...
                count++;
            } else if (*format == 's') {
                char* str = va_arg(args, char*);
                count += print(str);
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
