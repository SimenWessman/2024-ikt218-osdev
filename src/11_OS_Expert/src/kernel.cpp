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
// TO DO: Develop cursor movement functionality for text manipulation

// Including essential C libraries and kernel modules.
extern "C"  // Ensures that the C++ compiler uses C linkage for the included C headers.
{
    #include "libc/stdint.h"  // Includes standard integer types.
    #include "libc/stddef.h"  // Includes standard definitions like size_t.
    #include "libc/stdbool.h" // Includes boolean type.
    #include "libc/stdio.h"   // Includes standard I/O functions.
    #include "kernel/memory.h"// Includes memory management functions.
    #include "kernel/pit.h"   // Includes functions for the Programmable Interval Timer.
    #include "timer.h"        // Includes functions for handling timer interrupts.
    #include "keyboard.h"     // Includes keyboard handling functions.
    #include "idt.h"          // Includes functions for setting up the IDT.
    #include "gdt.h"          // Includes functions for setting up the GDT.
    #include "pic.h"          // Includes functions for setting up the PIC.
    #include "irqs.h"         // Includes functions for handling IRQs.
    int kernel_main(void);

    extern uint32_t end; // This is defined in arch/i386/linker.ld
}

// Operator overloads for new and delete operators.
// Existing global operator new overloads
void* operator new(size_t size) {
    return malloc(size);
}

void* operator new[](size_t size) {
    return malloc(size);
}

// Existing global operator delete overloads
void operator delete(void* ptr) noexcept {
    free(ptr);
}

void operator delete[](void* ptr) noexcept {
    free(ptr);
}

// Add sized-deallocation functions
void operator delete(void* ptr, size_t size) noexcept {
    (void)size; // Size parameter is unused, added to match required signature
    free(ptr);
}

void operator delete[](void* ptr, size_t size) noexcept {
    (void)size; // Size parameter is unused, added to match required signature
    free(ptr);
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

    unmask_irq0();  // Unmask IRQ0 (timer interrupt) to enable timer functionality.

    // Unmask IRQ1 (keyboard interrupt) to enable keyboard input.
    unmask_irq1();


    // Initialize the Interrupt Descriptor Table (IDT).
    // The IDT is essential for handling software and hardware interrupts,
    // mapping them to their respective handlers.
    init_idt();

    // Initialize all IRQ handlers, except for the keyboard handler.
    init_irq_handlers();

    // Enable all interrupts except for the keyboard interrupt.
    unmask_all_irqs();

    // Enable keyboard interrupts specifically, allowing the system
    // to respond to keyboard inputs.
    // This is done separately because of the more advanced functionality
    // essentially init_irq1() is called here.
    // not really though :)
    enable_keyboard_interrupt();

    // Initialize the kernel's memory management system.
    // This setup is crucial for managing memory allocation and deallocation.
    init_kernel_memory(&end);

    // Initialize paging for virtual memory management.
    init_paging();


    // Print the memory layout of the kernel.
    print_memory_layout();


    // Initialize PIT (Programmable Interval Timer) for scheduling.
    init_pit();

    // Basic output to verify that the system is up and running.
    printf("Hello World!\n%d\n", 10);  // Print "Hello World!" and a formatted number.

    // Just allocating some memory for the assignment:
    void* some_memory = malloc(12345); 
    void* memory2 = malloc(54321); 
    void* memory3 = malloc(13331);
    char* memory4 = new char[1000]();

    int counter = 0;

    //printf("Tick: %d\n", get_tick_count());
        
    while(true)
    {
        printf("[%d]: Sleeping with busy-waiting (HIGH CPU).\n", counter);
        sleep_busy(1000);
        printf("[%d]: Slept using busy-waiting.\n", counter++);

        printf("[%d]: Sleeping with interrupts (LOW CPU).\n", counter);
        sleep_interrupt(1000);
        printf("[%d]: Slept using interrupts.\n", counter++);
    }

    //printf("Tick: %d\n", get_tick_count());

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
