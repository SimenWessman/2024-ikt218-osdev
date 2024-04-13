// 11_OS_Expert/src/kernel.cpp
//
// ---------------------------------------------------
// (c) 2024, UiAOS                                   |
// All rights reserved.                              |
// ---------------------------------------------------
// 

// TOOD: Add IDT and IVT
// TODO: Keyboard support?
// TODO: Add writing functionality to the kernel
// TODO: Add cursor movement functionality to the kernel

// Extern C to avoid name mangling
extern "C" int kernel_main();
extern "C" int putchar(char c);
extern "C" int puts(const char *s);
extern "C" int printf(const char *format, ...);
extern "C" void init_gdt();
//extern "C" void init_idt();

int kernel_main() 
{
    // Initialize the GDT.
    init_gdt();
    // Initialize the IDT.
    //init_idt();

    // Have some fun with these commands:
    // write_string( (2 << 2) | 13, "Hello");
    // printf("Hello World!\n%d", 10);
    // puts("Hello World!\nMan");

    // Write a string to the screen
    printf("Hello World!\n%d", 10);


    return 0;
}