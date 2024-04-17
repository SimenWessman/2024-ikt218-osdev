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

extern "C"{
    #include "libc/stdint.h"
    #include "libc/stddef.h"
    #include "libc/stdbool.h"
    #include "libc/stdio.h"
    #include "keyboard.h"
    #include "idt.h"
    #include "gdt.h"
    #include "pic.h"

    int kernel_main(void);
}

int kernel_main() 
{
    // Initialize the GDT.
    init_gdt();
    
    // Initialize the PIC.
    init_pic();

    // Unmask IRQ1.
    unmask_irq1();

    // Initialize the IDT.
    init_idt();

    enable_keyboard_interrupt();

    // Have some fun with these commands:
    // write_string( (2 << 2) | 13, "Hello");
    // printf("Hello World!\n%d", 10);
    // puts("Hello World!\nMan");

    // Write a string to the screen
    printf("Hello World!\n%d", 10);

    while (1) 
    {
    __asm__("hlt");
    }

    return 0;
}