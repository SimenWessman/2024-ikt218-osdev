// -----------------------------------------------------------------------------
// File: 11_OS_Expert/include/libc/string.h
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description: 
// This header file declares common string and memory manipulation 
// functions that are widely used throughout the operating system.
// These functions provide essential capabilities for handling 
// strings and raw memory, which are fundamental for many system
// operations and higher-level application functionalities.
// -----------------------------------------------------------------------------

#pragma once  // Ensures the file is included only once in a single compilation.

#include <libc/stddef.h>  // Include for size_t definition.
#include <libc/stdint.h>   // Include for standard integer types.

// Returns the length of a null-terminated byte string.
//
//   @param str: Pointer to the null-terminated byte string.
//
//   @returns The number of characters in the string excluding the null-terminator.
size_t strlen(const char *str);

// Copies the string pointed to by src, including the null terminator, to the buffer pointed to by dest.
//
//   @param dest: Pointer to the destination array where the content is to be copied.
//   @param src: Pointer to the null-terminated byte string to copy from.
//
//   @returns A pointer to the destination string 'dest'.
char *strcpy(char *dest, const char *src);

// Compares two strings lexicographically.
//
//   @param s1: Pointer to the first null-terminated byte string.
//   @param s2: Pointer to the second null-terminated byte string.
//
//   @returns An integer less than, equal to, or greater than zero if 's1' is found,
//   respectively, to be less than, to match, or be greater than 's2'.
int strcmp(const char *s1, const char *s2);

// Appends the string pointed to by src to the end of the string pointed to by dest.
//
//   @param dest: Pointer to the destination array, which should contain a null-terminated
//         string and sufficient space to contain the concatenated resulting string.
//   @param src: Pointer to the null-terminated byte string to be appended.
//
//   @returns A pointer to the resulting string dest.
char *strcat(char *dest, const char *src);

// Fills the first num bytes of the memory area pointed to by ptr with the constant byte value.
//
//   @param ptr: Pointer to the memory block to fill.
//   @param value: Value to be set. The value is passed as an int but is converted to an unsigned char when set.
//   @param num: Number of bytes to be set to the value.
//
//   @returns A pointer to the memory area ptr.
void *memset(void *ptr, int value, size_t num);
