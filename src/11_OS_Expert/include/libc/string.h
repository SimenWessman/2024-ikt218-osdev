#pragma once
#include <libc/stddef.h>
#include <libc/stdint.h>


size_t strlen(const char *str);
char *strcpy(char *dest, const char *src);
int strcmp(const char *s1, const char *s2);
char *strcat(char *dest, const char *src);
void *memset(void *ptr, int value, size_t num);