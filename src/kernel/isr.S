%macro isr_no_err 1
isr_%+%1:
    ; When this macro is called the status registers are already on the stack
    push 0	; since we have no error code, to keep things consistent we push a default EC of 0
    push %1 ; pushing the interrupt number for easier identification by the handler
    ;save_context
    ;mov rdi, rsp    ; Let's set the current stack pointer as a parameter of the interrupts_handler
    ;cld ; Clear the direction flag
    ;call interrupts_handler ; Now we call the interrupt handler
    ;mov rsp, rax    ; use the returned context
    ;restore_context ; We served the interrupt let's restore the previous context
    add rsp, 16
    iretq
%endmacro

%macro isr_with_err 1
isr_%+%1:
    mov rax, %1
    push %1
    ;save_context
    ;mov rdi, rsp
    cld
    ;call interrupts_handler
    ;restore_context
    add rsp, 16
    iretq
%endmacro


%macro save_context 0
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rsi
    push rdi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15
%endmacro

%macro restore_context 0
    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rdi
    pop rsi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax
%endmacro

isr_no_err    0
isr_no_err    1
isr_no_err    2
isr_no_err    3
isr_no_err    4
isr_no_err    5
isr_no_err    6
isr_no_err    7
isr_with_err  8
isr_no_err    9
isr_with_err  10
isr_with_err  11
isr_with_err  12
isr_with_err  13
isr_with_err  14
isr_no_err    15
isr_no_err    16
isr_with_err  17
isr_no_err    18
isr_no_err    19
isr_no_err    20
isr_no_err    21
isr_no_err    22
isr_no_err    23
isr_no_err    24
isr_no_err    25
isr_no_err    26
isr_no_err    27
isr_no_err    28
isr_no_err    29
isr_with_err  30
isr_no_err    31

extern test_irq
global isr_40
isr_40:
    ;cld ; Clear the direction flag
    call test_irq ; Now we call the interrupt handler
    iretq ; Now we can return from the interrupt

[global isrt]
isrt:
%assign i 0 
%rep    32 
    dq isr_%+i
%assign i i+1 
%endrep
