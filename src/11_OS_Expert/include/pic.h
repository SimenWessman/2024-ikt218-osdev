// -----------------------------------------------------------------------------
// File: 11_OS_Expert/include/pic.h
//
// Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
// - @SimenWessman
// - @ThyDV
// 
// Description: 
// Header file for Programmable Interrupt Controller (PIC) management.
// This file declares functions for initializing and configuring the PIC,
// which is essential for controlling the hardware interrupts in the system.
// -----------------------------------------------------------------------------

#ifndef PIC_H
#define PIC_H

// Function: 
// init_pic
// 
// Description: 
// Initializes the Programmable Interrupt Controller (PIC).
// This function sets up both the master and slave PICs for handling
// interrupts. It remaps the PIC interrupts to appropriate vectors
// in the Interrupt Descriptor Table (IDT) to avoid conflicts with
// processor-reserved interrupts.
void init_pic(void);

// Function: 
// unmask_irq1
// 
// Description: 
// Enables (unmasks) the IRQ1 line on the PIC, which is typically
// used for keyboard interrupts. This function changes the PIC's
// mask register to allow IRQ1 interrupts to be delivered to the CPU,
// enabling the system to respond to keyboard input events.
void unmask_irq1(void);
void unmask_irq0(void);
void unmask_irq12(void);

void unmask_all_irqs(void);

#endif // PIC_H
