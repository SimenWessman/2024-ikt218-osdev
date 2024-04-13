#pragma once

void outb(unsigned short port, unsigned char val);
unsigned char inb(unsigned short port);

int putchar(char c);
//bool print(const char* data, size_t length);
int printf(const char* __restrict__ format, ...);
