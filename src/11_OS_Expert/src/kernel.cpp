// -----------------------------------------------------------------------------
// File: 11_OS_Expert/src/kernel.cpp
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description: 
// Main kernel file for UiAOS. This file initializes core system
// components such as the Global Descriptor Table (GDT), 
// Programmable Interrupt Controller (PIC), and the Interrupt
// Descriptor Table (IDT). It also sets up basic I/O and interrupt
// handling capabilities.
// -----------------------------------------------------------------------------

// TO DO: Add additional functionality for Interrupt Vector Table (IVT)
// TO DO: Enhance keyboard support with more features
// TO DO: Implement text writing functionality to the kernel
// TO DO: Develop cursor movement functionality for text manipulation

// Including essential C libraries and kernel modules.
extern "C"  // Ensures that the C++ compiler uses C linkage for the included C headers.
{
    #include "libc/stdint.h"  // Includes standard integer types.
    #include "libc/stddef.h"  // Includes standard definitions like size_t.
    #include "libc/stdbool.h" // Includes boolean type.
    #include "libc/stdio.h"   // Includes standard I/O functions.
    #include "keyboard.h"     // Includes keyboard handling functions.
    #include "idt.h"          // Includes functions for setting up the IDT.
    #include "gdt.h"          // Includes functions for setting up the GDT.
    #include "pic.h"          // Includes functions for setting up the PIC.

    int kernel_main(void);
}

// The main function for the kernel, called at boot.
int kernel_main() 
{
    // Initialize the Global Descriptor Table (GDT).
    // This setup is crucial for protected mode operations and defines
    // how memory segments are accessed and controlled.
    init_gdt();
    
    // Initialize the Programmable Interrupt Controller (PIC).
    // This controller manages hardware interrupts and ensures
    // they are correctly handled by the CPU.
    init_pic();

    // Unmask IRQ1 (keyboard interrupt) to enable keyboard input.
    unmask_irq1();

    // Unmask IRQ0 (timer interrupt) to enable timer functionality.
     unmask_irq0();

    // // Unmask IRQ12 (mouse interrupt) to enable mouse input.
    unmask_irq12();

    // Initialize the Interrupt Descriptor Table (IDT).
    // The IDT is essential for handling software and hardware interrupts,
    // mapping them to their respective handlers.
    init_idt();

    // Enable keyboard interrupts specifically, allowing the system
    // to respond to keyboard inputs.
    enable_keyboard_interrupt();

    // Basic output to verify that the system is up and running.
    printf("Hello World!\n%d", 10);  // Print "Hello World!" and a formatted number.

    // Infinite loop to keep the kernel running.
    // The 'hlt' instruction halts the CPU until the next interrupt is received,
    // which is more power efficient than a spin wait.
    while (1) 
    {
        __asm__("hlt");
    }

    // The main function should never return, but return 0 here to prevent
    // compiler warnings about missing return statements.
    return 0;
}