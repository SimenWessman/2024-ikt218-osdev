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

// Function to set a block of memory with a byte value
void* memset (void * ptr, int value, size_t num)
{
    unsigned char* p=ptr;     // Cast the pointer to unsigned char*
    while(num--)
        *p++ = (unsigned char)value;   // Set each byte to the given value
    return ptr;               // Return the pointer to the block of memory
}

// Copies num bytes from memory area src to memory area dest.
void* memcpy(void* dest, const void* src, size_t count )
{
    char* dst8 = (char*)dest;  // Cast the destination pointer to char*
    char* src8 = (char*)src;   // Cast the source pointer to char*

    if (count & 1) {           // If the count is odd
        dst8[0] = src8[0];     // Copy one byte from source to destination
        dst8 += 1;             // Increment destination pointer by one byte
        src8 += 1;             // Increment source pointer by one byte
    }

    count /= 2;                // Divide the count by two to copy 2 bytes at a time
    while (count--) {          // Loop through each pair of 2 bytes
        dst8[0] = src8[0];     // Copy the first byte of the pair from source to destination
        dst8[1] = src8[1];     // Copy the second byte of the pair from source to destination

        dst8 += 2;             // Increment destination pointer by 2 bytes
        src8 += 2;             // Increment source pointer by 2 bytes
    }

    return (void*)dest;        // Return the destination pointer
}
