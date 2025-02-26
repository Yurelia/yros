ENTRY(load_kernel)

SECTIONS
{
	.boot 0x7e00:
    {
        *(.boot.*)
    }
	
	kaddr = 0xffffffff00000000;
	.text 0x100000 + kaddr: AT(ADDR(.text) - kaddr)
	{
		*(.text)
		*(.rodata)
		*(.rodata.*)
	}
	.data : AT(ADDR(.data) - kaddr)
	{
		*(.data)
		*(.bss)
	}
	kernel_end = .;
}
