#include <yros/framebuffer.h>
#include <yros/idt.h>
#include <yros/ioapic.h>
#include <yros/keyboard.h>
#include <yros/lapic.h>
#include <yros/multiboot.h>
#include <yros/pmm.h>

void init_kernel()
{
	init_boot_tags();
	init_screen();
	//init_interrupt();
	//init_ioapic();
	//init_apic();
	init_memory();
	//init_keyboard();
	
	while (1);
}