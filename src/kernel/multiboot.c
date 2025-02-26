#include <yros/multiboot.h>
#include <yros/logk.h>

mboot_header_t *mboot_tags[22];

void init_boot_tags()
{
	asm ("mov %%ebx, %0": "=m" (mboot_tags[0]));
	for (mboot_header_t *tag = ++mboot_tags[0]; tag->type; tag += (tag->size + 7) / 8)
		mboot_tags[tag->type] = tag;
}