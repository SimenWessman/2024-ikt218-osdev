// -----------------------------------------------------------------------------
// File: 11_OS_Expert/src/string.c
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description:
// Implementation of standard string and memory manipulation functions
// that are foundational for operating system development. This file
// provides routines for string handling such as length calculation,
// copying, comparison, and concatenation, as well as memory operations
// like setting and copying. These utilities are essential for handling
// data efficiently and securely within the system.
// -----------------------------------------------------------------------------

#include <libc/string.h>

// Returns the length of a null-terminated string.
size_t strlen(const char* str) 
{
    size_t len = 0;
    while (str[len] != '\0') 
    {
        len++;  // Increment length for each character until the null terminator is reached.
    }
    return len;
}

// Copies the string pointed to by src, including the terminating null byte ('\0'),
// to the buffer pointed to by dest.
char* strcpy(char* dest, const char* src) 
{
    char* original_dest = dest;
    while ((*dest++ = *src++) != '\0') 
    {
        ; // The loop continues until the null terminator is copied.
    }
    return original_dest;
}

// Compares two strings lexicographically.
int strcmp(const char* str1, const char* str2) 
{
    while (*str1 && (*str1 == *str2)) 
    {
        str1++;
        str2++;  // Continue comparing the next characters.
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;  // Return the difference of the first non-matching characters.
}

// Appends the source string to the destination string, overwriting the
// terminating null byte at the end of dest, and then adds a terminating
// null byte.
char* strcat(char* dest, const char* src) 
{
    char* original_dest = dest;
    while (*dest) 
    {
        dest++;  // Move the pointer to the end of the destination string.
    }
    while ((*dest++ = *src++) != '\0') 
    {
        ; // Copy the source string including the null terminator.
    }
    return original_dest;
}

// Fills the first num bytes of the memory area pointed to by ptr with the byte value.
void *memset(void *ptr, int value, size_t num) 
{
    unsigned char *p = ptr;
    while (num--) 
    {
        *p++ = (unsigned char)value;  // Set each byte in the block to the value.
    }
    return ptr;
}

// Copies num bytes from memory area src to memory area dest.
void *memcpy(void *dest, const void *src, size_t num) 
{
    unsigned char *d = dest;
    const unsigned char *s = src;
    while (num--) 
    {
        *d++ = *s++;  // Copy each byte from source to destination.
    }
    return dest;
}
