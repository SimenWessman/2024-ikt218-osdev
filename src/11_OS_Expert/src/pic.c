// -----------------------------------------------------------------------------
// File: 11_OS_Expert/src/pic.c
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description: 
// Implementation of functions for initializing and configuring the
// Programmable Interrupt Controller (PIC). These functions are 
// critical for setting up how the system handles hardware interrupts.
// -----------------------------------------------------------------------------

#include "libc/stdio.h"
#include "libc/stdint.h"

// Function:
// init_pic
//
// Description: 
// Initializes the Programmable Interrupt Controller (PIC) to ensure
// that IRQ lines are properly managed and interrupts are correctly
// routed to the CPU. This setup involves sending a series of
// Initialization Command Words (ICWs) to the PIC.
void init_pic(void) 
{
    // ICW1: Start initialization
    // Sends the initialization command to both the master (0x20) and slave (0xA0) PIC.
    // 0x11 = Init + edge triggered mode + expect ICW4.
    outb(0x20, 0x11);
    outb(0xA0, 0x11);

    // ICW2: Remap offset to IDT entries
    // Sets the starting offset of the interrupt vectors in the IDT.
    // Master PIC's IRQs start at 32 (0x20), slave's start at 40 (0x28).
    outb(0x21, 0x20);  // Master PIC vector offset to 32
    outb(0xA1, 0x28);  // Slave PIC vector offset to 40

    // ICW3: Configure cascading
    // Tells Master PIC that there is a slave PIC at IRQ2 (0x04),
    // and tells Slave PIC its cascade identity (0x02).
    outb(0x21, 0x04);
    outb(0xA1, 0x02);

    // ICW4: Environment info
    // Sets additional operation mode settings for the PICs.
    // 0x01 = 8086/88 (MCS-80/85) mode
    outb(0x21, 0x01);
    outb(0xA1, 0x01);

    // Mask all interrupts temporarily to prevent any interrupts
    // while the system is still setting up.
    outb(0x21, 0xFF);  // Mask all on master
    outb(0xA1, 0xFF);  // Mask all on slave
}

// Function to enable the timer IRQ (IRQ0)
void unmask_irq0() 
{
    uint8_t mask = inb(0x21);  // Read from the master PIC
    mask &= ~(1 << 0);  // Clear the mask bit for IRQ0
    outb(0x21, mask);  // Write back to the master PIC
}

// Function:
// unmask_irq1
//
// Description: 
// Enables the IRQ1 line, which is typically used for keyboard
// interrupts. This function modifies the PIC's mask register to
// allow IRQ1 interrupts to be recognized by the system.
void unmask_irq1() 
{
    // Read the current mask register value from the master PIC.
    uint8_t mask = inb(0x21);

    // Clear the second bit (corresponding to IRQ1) to enable it.
    // This uses bitwise AND with the negation of the bit mask for IRQ1.
    mask &= ~(1 << 1);

    // Write the new mask value back to the master PIC.
    outb(0x21, mask);
}

// Function to enable IRQ2
void unmask_irq2() 
{
    uint8_t mask = inb(0x21);  // Read from the master PIC
    mask &= ~(1 << 2);  // Clear the mask bit for IRQ2
    outb(0x21, mask);  // Write back to the master PIC
}

// Function to enable the mouse IRQ (IRQ12)
void unmask_irq12() 
{
    uint8_t mask = inb(0xA1);  // Read from the slave PIC
    mask &= ~(1 << 4);  // Clear the mask bit for IRQ12 (note: 4 because IRQ12 is 12 - 8 = 4 on the slave)
    outb(0xA1, mask);  // Write back to the slave PIC
}

// Utility function to unmask an IRQ line
void unmask_irq(uint8_t IRQ_line) 
{
    uint16_t port;
    uint8_t value;

    if (IRQ_line < 8) {
        port = 0x21;  // Master PIC
    } else {
        port = 0xA1;  // Slave PIC
        IRQ_line -= 8;
    }

    value = inb(port) & ~(1 << IRQ_line);
    outb(port, value);
}

// Utility function to unmask all IRQs except IRQ1 (keyboard)
void unmask_all_irqs() 
{
    for (uint8_t i = 0; i < 16; i++) 
    {
        if (i != 1 && i != 0 && i != 12 && i != 2) // Skip IRQ1 (keyboard)
        {  
            unmask_irq(i);
        }
    }

    unmask_irq0();  // Unmask IRQ0 (timer)
    unmask_irq1();  // Unmask IRQ1 (keyboard)
    unmask_irq2();  // Unmask IRQ2 (cascading)
    unmask_irq12(); // Unmask IRQ12 (mouse)
}
