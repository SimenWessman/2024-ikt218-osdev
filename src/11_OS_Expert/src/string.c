#include <libc/string.h>

size_t strlen(const char* str) 
{
    size_t len = 0;
    while (str[len] != '\0') 
    {
        len++;
    }
    return len;
}

char* strcpy(char* dest, const char* src) 
{
    char* original_dest = dest;
    while ((*dest++ = *src++) != '\0') 
    {
        ;
    }
    return original_dest;
}

int strcmp(const char* str1, const char* str2) 
{
    while (*str1 && (*str1 == *str2)) 
    {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}

char* strcat(char* dest, const char* src) 
{
    char* original_dest = dest;
    while (*dest) 
    {
        dest++;
    }

    while ((*dest++ = *src++) != '\0') 
    {
        ;
    }
    return original_dest;
}

void *memset(void *ptr, int value, size_t num) 
{
    unsigned char *p = ptr;
    while (num--) 
    {
        *p++ = (unsigned char)value;
    }
    return ptr;
}
