#include "mouse.h"
#include "pic.h"  // You need methods to send commands to the PIC
#include "idt.h"
#include <libc/stdio.h>

#define MOUSE_IRQ 0x2C  // IRQ12, after PIC remap
#define MOUSE_LEFT_BUTTON 0x01 // Left mb

// Some global mouse related variables.
static uint8_t mouse_cycle = 0;
static int8_t mouse_byte[3];

// Function to initialize the mouse device
void init_mouse() 
{
    // Enable the auxiliary mouse device
    outb(0x64, 0xA8);

    // Enable the interrupts
    outb(0x64, 0x20);  // Requesting settings from command byte
    uint8_t status = (inb(0x60) | 2);  // Set the correct bit to enable IRQ12
    outb(0x64, 0x60);  // Setting command byte
    outb(0x60, status);

    // Tell the mouse to use default settings
    outb(0x64, 0xD4);
    outb(0x60, 0xF6);
    inb(0x60);  // Acknowledge

    // Enable the mouse
    outb(0x64, 0xD4);
    outb(0x60, 0xF4);
    inb(0x60);  // Acknowledge

    // Register the mouse handler
    idt_set_gate(MOUSE_IRQ, (uint32_t)mouse_handler, 0x08, 0x8E);
}

// Function to handle mouse interrupts
void mouse_handler(void) 
{

    printf("Mouse interrupt\n");
    outb(0xA0, 0x20);  // EOI to slave PIC
    outb(0x20, 0x20);  // EOI to master PIC
    __asm__("sti");  // Re-enable interrupts.
}
