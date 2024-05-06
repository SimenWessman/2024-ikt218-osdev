// -----------------------------------------------------------------------------
// File: 11_OS_Expert/src/idt.c
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description: 
// This file contains functions to manage and initialize the Interrupt
// Descriptor Table (IDT) for handling interrupts in the system. The
// IDT is essential for mapping interrupt numbers to the functions
// that handle them.
// -----------------------------------------------------------------------------

#include "idt.h"
#include "keyboard.h"
#include "timer.h"
#include "mouse.h"
#include "libc/stdio.h"

// Array of IDT entries; defines an IDT with 256 possible interrupt handlers.
struct idt_entry idt_entries[256];

// The IDT pointer used by the 'lidt' instruction to locate the IDT in memory.
struct idt_ptr idtp;

// Function:
// idt_set_gate
// 
// Description: 
// Configures a single entry in the IDT.
//
//   @param uint8_t num - The index of the entry in the IDT to configure.
//   @param uint32_t base - The base address of the interrupt handler function.
//   @param uint16_t sel - The segment selector for the code segment where the handler resides.
//   @param uint8_t flags - Flags to control the behavior of the interrupt gate.
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags) {
    idt_entries[num].base_lo = base & 0xFFFF;          // Lower 16 bits of the handler address.
    idt_entries[num].base_hi = (base >> 16) & 0xFFFF;  // Upper 16 bits of the handler address.
    idt_entries[num].sel = sel;                        // Segment selector where the handler function exists.
    idt_entries[num].always0 = 0;                      // Must always be zero.
    idt_entries[num].flags = flags;                    // Flags set properties such as the privilege level.
}

// Function:
// print_idt_entry
//
// Description: 
// Prints the configuration of an IDT entry.
//
//   @param uint8_t num - The index of the entry in the IDT to print.
void print_idt_entry(uint8_t num) {
    struct idt_entry e = idt_entries[num];
    printf("IDT Entry %d: base_lo=%X, base_hi=%X, sel=%X, flags=%X\n",
           num, e.base_lo, e.base_hi, e.sel, e.flags);
}

// Function: 
// init_idt
//
// Description: 
// Initializes the IDT with default settings 
// and loads it into the CPU's IDT register.
void init_idt() 
{
    idtp.limit = sizeof(struct idt_entry) * 256 - 1;  // Set the size of the IDT.
    idtp.base = (uint32_t)&idt_entries;               // Set the base address of the IDT.

    memset(&idt_entries, 0, sizeof(struct idt_entry) * 256);  // Clear out the old IDT, if any.

    // Set specific gates; for example, set gate 0x21 for keyboard interrupts.
    idt_set_gate(0x21, (uint32_t)keyboard_handler, 0x08, 0x8E);

    // // Set gate for the timer interrupt (IRQ0 -> INT 32)
    idt_set_gate(32, (uint32_t)timer_handler, 0x08, 0x8E);  // 0x08 is your kernel code segment selector, 0x8E sets the present, ring 0, and interrupt gate

    // // Set gate for the mouse interrupt (IRQ12 -> INT 44)
    idt_set_gate(44, (uint32_t)mouse_handler, 0x08, 0x8E);

    // Load the IDT into the IDT register (IDTR) of the CPU.
    __asm__ __volatile__("lidt (%0)" : : "r" (&idtp));

    // Enable interrupts on the processor.
    __asm__ __volatile__("sti");

    // Uncomment to debug or print the entry for keyboard interrupts after initialization.
    // print_idt_entry(0x21);
}
