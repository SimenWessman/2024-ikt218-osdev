#include "irqs.h"
#include "idt.h"
#include <libc/stdio.h>

// Forward declarations for ISR handler functions
void irq0_handler(void);
// void irq1_handler(void); // Handled separately
void irq2_handler(void);
void irq3_handler(void);
void irq4_handler(void);
void irq5_handler(void);
void irq6_handler(void);
void irq7_handler(void);
void irq8_handler(void);
void irq9_handler(void);
void irq10_handler(void);
void irq11_handler(void);
void irq12_handler(void);
void irq13_handler(void);
void irq14_handler(void);
void irq15_handler(void);

// Array of function pointers for the IRQ handlers
void (*irq_handlers[15]) (void) = 
{
    //irq0_handler,     Handled separately
    // irq1_handler,    Handled separately
    irq2_handler,
    irq3_handler,
    irq4_handler,
    irq5_handler,
    irq6_handler,
    irq7_handler,
    irq8_handler,
    irq9_handler,
    irq10_handler,
    irq11_handler,
    irq12_handler,
    irq13_handler,
    irq14_handler,
    irq15_handler
};

// Function definitions for each IRQ handler

// Define additional handlers as necessary...
void irq2_handler(void) 
{
    printf("IRQ2 Interrupt\n");
    outb(0x20, 0x20); // EOI
    __asm__("sti");
}

void irq3_handler(void) 
{
    printf("IRQ3 Interrupt\n");
    outb(0x20, 0x20); // EOI
    __asm__("sti");
}

void irq4_handler(void) 
{
    printf("IRQ4 Interrupt\n");
    outb(0x20, 0x20); // EOI
    __asm__("sti");
}

void irq5_handler(void) 
{
    printf("IRQ5 Interrupt\n");
    outb(0x20, 0x20); // EOI
    __asm__("sti");
}

void irq6_handler(void) 
{
    printf("IRQ6 Interrupt\n");
    outb(0x20, 0x20); // EOI
    __asm__("sti");
}

void irq7_handler(void) 
{
    printf("IRQ7 Interrupt\n");
    outb(0x20, 0x20); // EOI
    __asm__("sti");
}

void irq8_handler(void) 
{
    printf("IRQ8 Interrupt\n");
    outb(0xA0, 0x20); // EOI to slave PIC
    outb(0x20, 0x20); // EOI to master PIC
    __asm__("sti");
}

void irq9_handler(void) 
{
    printf("IRQ9 Interrupt\n");
    outb(0xA0, 0x20); // EOI to slave PIC
    outb(0x20, 0x20); // EOI to master PIC
    __asm__("sti");
}

void irq10_handler(void) 
{
    printf("IRQ10 Interrupt\n");
    outb(0xA0, 0x20); // EOI to slave PIC
    outb(0x20, 0x20); // EOI to master PIC
    __asm__("sti");
}

void irq11_handler(void) 
{
    printf("IRQ11 Interrupt\n");
    outb(0xA0, 0x20); // EOI to slave PIC
    outb(0x20, 0x20); // EOI to master PIC
    __asm__("sti");
}

void irq12_handler(void) 
{
    printf("Mouse Interrupt\n");
    outb(0xA0, 0x20); // EOI to slave PIC
    outb(0x20, 0x20); // EOI to master PIC
    __asm__("sti");
}

void irq13_handler(void) 
{
    printf("IRQ13 Interrupt\n");
    outb(0xA0, 0x20); // EOI to slave PIC
    outb(0x20, 0x20); // EOI to master PIC
    __asm__("sti");
}

void irq14_handler(void) 
{
    printf("IRQ14 Interrupt\n");
    outb(0xA0, 0x20); // EOI to slave PIC
    outb(0x20, 0x20); // EOI to master PIC
    __asm__("sti");
}

void irq15_handler(void)
{
    printf("IRQ15 Interrupt\n");
    outb(0xA0, 0x20); // EOI to slave PIC
    outb(0x20, 0x20); // EOI to master PIC
    __asm__("sti");
}

void init_irq_handlers(void) 
{
    // Initialize all IRQ Handlers (except for IRQ1)
    for (int i = 0; i < 15; i++) 
    {
        // Skip IRQ1 which is handled separately
        if (i == 0 || i == 1) continue;


        idt_set_gate(32 + i, (uint32_t)irq_handlers[i], 0x08, 0x8E);
    }
}
