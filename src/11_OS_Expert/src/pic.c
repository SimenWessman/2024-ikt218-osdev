#include "libc/stdio.h"
#include "libc/stdint.h"

void init_pic(void) 
{
    // ICW1: Start initialization
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2: Remap offset to IDT entries
    outb(0x21, 0x20);  // Master PIC vector start at 32
    outb(0xA1, 0x28);  // Slave PIC vector start at 40

    // ICW3: Configure cascading
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // ICW4: Environment info
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Mask all interrupts temporarily
    outb(0x21, 0xFF);
    outb(0xA1, 0xFF);
}

void unmask_irq1() 
{
    uint8_t mask = inb(0x21);
    mask &= ~(1 << 1); // Clear the second bit to enable IRQ1
    outb(0x21, mask);
}