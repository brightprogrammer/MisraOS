    ;; this is 64 bit code
    [bits 64]

    ;; load gdt function
load_gdt:
    lgdt [rdi]

    mov ax, 0x10                ; offset to kernel data segment
    mov ds, ax
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax

    ;; store the return address in rdi
    pop rdi
    mov rax, 0x08               ; offset to kernel cod esegment
    push rax
    push rdi
    retfq                       ; far return

GLOBAL load_gdt
