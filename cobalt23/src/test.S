section .data
    message db 'Hello, world!', 0

section .text
    global _start

_start:
    ; Write the message to stdout
    mov eax, 4      ; System call for write
    mov ebx, 1      ; File descriptor for stdout
    mov ecx, message    ; Address of the message
    mov edx, 13     ; Length of the message
    int 0x80        ; Call the kernel

    ; Exit the program
    mov eax, 1      ; System call for exit
    xor ebx, ebx    ; Return code of 0
    int 0x80        ; Call the kernel