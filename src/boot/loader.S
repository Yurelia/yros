SECTION .boot.header
hbeg:
	align 8
	dd 0xe85250d6                                   ; magic number
	dd 0                                            ; architecture: i386
	dd hend - hbeg                                  ; header size
	dd 0x100000000 - (0xe85250d6 + 0 + hend - hbeg) ; checksum

	align 8
    dw  5                                           ; tag: framebuffer
    dw  0                                           ; flag
    dd  20                                          ; size
    dd  0                                           ; width: auto
    dd  0                                           ; height: auto
    dd  32                                          ; depth: auto

	align 8
	dw 0                                            ; tag: end
	dw 0                                            ; flag
	dd 8                                            ; size
hend:


SECTION .boot.text exec

%define TO_PHY(addr) (addr - 0xffffffff00000000)
extern init_kernel
global load_kernel

load_kernel:
[bits 32]
	; map a l3 ptable for physical memory
	mov dword [TO_PHY(ptable_l4) + 0], TO_PHY(ptable_phy) + 0b11
	; map the l4 ptable to itself
	mov dword [TO_PHY(ptable_l4) + 510 * 8], TO_PHY(ptable_l4) + 0b11
	; map a l3 ptable for kernel space
	mov dword [TO_PHY(ptable_l4) + 511 * 8], TO_PHY(ptable_ker) + 0b11

	; map 4G physical memory identically
	mov eax, TO_PHY(ptable_phy)
	mov ecx, 0b10000011
	.map_ptable_phy:
	mov [eax], ecx
	add eax, 8
	add ecx, 0x40000000
	jnc .map_ptable_phy
	; map 1G physical memory to kernel space
	mov [TO_PHY(ptable_ker) + 508 * 8], ecx

	; load page table
	mov eax, TO_PHY(ptable_l4)
	mov cr3, eax
	; enable PAE
	mov eax, cr4
	or  eax, 1 << 5
	mov cr4, eax
	; enable long mode
	mov ecx, 0xc0000080
	rdmsr
	or eax, 1 << 8
	wrmsr
	; enable paging
	mov eax, cr0
	or eax, 1 << 31
	or eax, 1 << 16
	mov cr0, eax
	; load gdt and set the code segment
	lgdt [TO_PHY(gdtptr)]
	mov ax, 0x8
	mov cs, ax
	mov ax, 0x10
	mov ss, ax

[bits 64]
	mov rax, init_kernel
	jmp rax
	

SECTION .bss
align 4096
ptable_l4:
	resb 4096
ptable_phy:
	resb 4096
ptable_ker:
	resb 4096
stack_bot:
	resb 4096
stack_top:


SECTION .rodata
gdt64:
	dq 0x0000000000000000 ; zero
	dq 0x00209a0000000000 ; kernel code
	dq 0x0040920000000000 ; kernel data
	dq 0x0020fa0000000000 ; user code
	dq 0x0040f20000000000 ; user data
gdtptr:
	dw $ - gdt64 - 1      ; gdt length
	dq gdt64              ; gdt address