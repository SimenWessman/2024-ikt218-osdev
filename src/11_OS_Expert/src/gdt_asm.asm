; -----------------------------------------------------------------------------
; File: 11_OS_Expert/src/gdt_asm.asm
;
; Authors: Group 11_OS_Expert (Thy Dang Vo & Simen Wessman Martinsen)
; - @SimenWessman
; - @ThyDV
; 
; Description:
; Assembly routines for loading and refreshing the Global Descriptor
; Table (GDT) for the system. This file includes necessary assembly
; instructions to load the new GDT and refresh segment registers to
; ensure that the new segment descriptors are used by the CPU.
; -----------------------------------------------------------------------------

; Declare external symbols that are defined in other files.
; gdt_ptr is a struct that holds the address and limit of the GDT defined in (gdt.c).
extern gdt_ptr

; Make the label 'load_gdt' accessible from other files. This is important
; for linking, allowing the 'load_gdt' function to be called from C code.
global load_gdt

; Start of the text section where executable instructions are placed.
section .text

; Function: load_gdt
; Purpose: Load the new Global Descriptor Table and refresh the segment registers.
load_gdt:
    ; Load the new GDT from the address specified by gdt_ptr.
    ; The lgdt instruction takes a memory operand that points to a 6-byte structure:
    ;   - First 2 bytes: Size of the GDT minus one (limit)
    ;   - Next 4 bytes: Linear address of the GDT (base)
    lgdt [gdt_ptr]

    ; Perform a far jump to a code segment specified by the new GDT to refresh the CS register.
    ; The far jump is essential as it forces the CPU to fetch a new CS descriptor from the GDT.
    ; 0x08 is the selector for the new code segment in the GDT. Each GDT entry is 8 bytes, so
    ; the code segment descriptor is at the second position in the GDT (0x08 = 8).
    jmp 0x08:flush_cs

; Label: flush_cs
; After performing the far jump, we are now executing in the context of the new code segment.
; Next, we need to update all other segment registers (DS, ES, FS, GS, SS) to use the new data segment.
flush_cs:
    ; Load the data segment selector into AX register. 0x10 is the selector for the data
    ; segment in the GDT, located at the third position (0x10 = 16).
    mov ax, 0x10

    ; Update the segment registers with the new data segment selector. This step is necessary
    ; because changing the GDT does not automatically update the segment registers other than CS.
    mov ds, ax  ; Data Segment
    mov es, ax  ; Extra Segment
    mov fs, ax  ; FS Segment
    mov gs, ax  ; GS Segment
    mov ss, ax  ; Stack Segment

    ; Return from this function. The CPU will continue executing from the instruction
    ; that called 'load_gdt'. This 'ret' effectively completes the transition to the new GDT.
    ret
