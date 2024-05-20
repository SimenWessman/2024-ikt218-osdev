// -----------------------------------------------------------------------------
// File: 11_OS_Expert/include/idt.h
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description: 
// Header file for the Interrupt Descriptor Table (IDT) management.
// This file defines the structures and constants necessary for
// setting up the IDT, which is crucial for handling hardware and
// software interrupts in a protected mode environment.
// -----------------------------------------------------------------------------

#ifndef IDT_H
#define IDT_H

#include "libc/string.h"   // Include for memory manipulation functions.
#include "libc/stdint.h"   // Include for fixed-width integer types.

// External declaration of the keyboard, timer and mouse interrupt handlers.
// This function is implemented elsewhere, typically in an assembly file or
// a C file where keyboard interrupt logic is handled.
extern void keyboard_handler(void); // irq1
extern void timer_handler(void); // irq0
extern void mouse_handler(void); // irq12
extern void irq2(void);
extern void irq3(void);
extern void irq4(void);
extern void irq5(void);
extern void irq6(void);
extern void irq7(void);
extern void irq8(void);
extern void irq9(void);
extern void irq10(void);
extern void irq11(void);
extern void irq13(void);
extern void irq14(void);
extern void irq15(void);

// Structure defining a single entry in the Interrupt Descriptor Table.
struct idt_entry
{
    uint16_t base_lo;  // Lower 16 bits of the address where the interrupt handler resides.
    uint16_t sel;      // Segment selector in GDT or LDT that points to the segment containing the interrupt handler.
    uint8_t  always0;  // This must always be zero; reserved for future use.
    uint8_t  flags;    // Type and attributes, e.g., present bit, privilege level (DPL), and descriptor type.
    uint16_t base_hi;  // Upper 16 bits of the address where the interrupt handler resides.
} __attribute__((packed));  // The 'packed' attribute is used to ensure the compiler does not add padding to the structure.

// Structure for the IDTR register, used with the 'lidt' instruction to load the address and size of the IDT.
struct idt_ptr
{
    uint16_t limit;   // Limit of the IDT, defining the size of the table minus one.
    uint32_t base;    // Linear base address of the IDT.
} __attribute__((packed));

// Function prototypes for managing the IDT.

// Initializes the Interrupt Descriptor Table.
void init_idt();

// Sets an entry in the IDT with specified attributes.
//
//   @param uint8_t num: The number of the IDT entry to set.
//   @param uint32_t base: The base address of the interrupt handler.
//   @param uint16_t sel: The segment selector for the code segment containing the handler.
//   @param uint8_t flags: Flags to specify characteristics like the present bit and privilege level.
void idt_set_gate(uint8_t num, uint32_t base, uint16_t sel, uint8_t flags);

#endif // IDT_H
