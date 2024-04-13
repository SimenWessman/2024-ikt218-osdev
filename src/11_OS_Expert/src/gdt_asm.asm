; ------------------
; |  gdt_asm.asm   |
; ------------------

extern gdt_ptr ; Declares the external variable gdt_ptr, which is defined in another file

global load_gdt ; Makes the label accessible from other files
section .text
load_gdt:
    lgdt [gdt_ptr]  ; Load the GDT with the new descriptor table

    ; After loading the GDT, we need to refresh the CS register.
    ; We use a far jump to do this, which updates the CS register and the instruction pointer.
    ; Each entry in the GDT is 8 bytes. Meaning the offset is decided by (index * 8)
    ; 0x08 is the offset for the new code segment in the GDT.
    jmp 0x08:flush_cs

flush_cs:
    ; Update the data segment registers
    mov ax, 0x10  ; 0x10 (8*2 = 16 = 0x10) is the offset for the data segment in the GDT.
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ; Return to the calling function.
    ret
