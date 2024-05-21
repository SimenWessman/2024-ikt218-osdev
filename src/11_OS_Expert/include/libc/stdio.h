// -----------------------------------------------------------------------------
// File: 11_OS_Expert/include/libc/stdio.h
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
//
// Description:
// This header file declares basic standard input/output functions 
// that interface directly with system hardware. It includes functions
// for sending data to and receiving data from hardware ports, 
// as well as functions for character and string output to the console.
// This facilitates basic text-based interaction within the operating system.
// -----------------------------------------------------------------------------

#pragma once  // Ensures the file is included only once in a single compilation.

// Sends a byte of data to the specified hardware port.
//
//   @param port: The hardware port address to send the data to.
//   @param val: The byte of data to send.
void outb(unsigned short port, unsigned char val);

// Receives a byte of data from the specified hardware port.
//
//   @param port: The hardware port address to read the data from.
//
//   @returns The byte of data received from the hardware port.
unsigned char inb(unsigned short port);

// Scrolling utility functions for kernel:
void handle_scroll_up();
void handle_scroll_down();

// Outputs a single character to the standard output device, typically the console.
//
//   @param c: The character to be printed.
//
//   @returns The character printed as an integer value.
int putchar(char c);

// No longer needed as we have implemented a more advanced version of putchar.
// bool print(const char* data, size_t length);

// Prints a formatted string to the standard output, similar to the C standard library's printf.
// Supports basic format specifiers like %d for integers, %s for strings, and %c for characters.
//
//   @param format: A format string that specifies how subsequent arguments are converted for output.
//   @param ...: Variadic parameters that will replace the format specifiers in the format string.
//
//   @returns The number of characters printed.
int printf(const char* __restrict__ format, ...);
