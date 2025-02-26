#include <yros/ioapic.h>
#include <yros/multiboot.h>

void init_ioapic()
{
    mboot_acpiv2_t *acpitag = (mboot_acpiv2_t*)mboot_tags[MBOOT_TAG_ACPIV2];
    //u64_t *table = ((XSDT*)acpitag->XsdtAddress)->tables;
	XSDT *xsdt = (XSDT*)((mboot_acpiv2_t*)mboot_tags[MBOOT_TAG_ACPIV2])->XsdtAddress;
    //for (u64_t *table = &xsdt->tables; (u64_t)table < (u64_t)xsdt)
    //for (u64_t addr = (u64_t)xsdt + sizeof(acpi_header_t); addr)
    MADT *madt;
	for (int i = 0; i < (xsdt->header.Length - sizeof(acpi_header_t)) / 8; i++) {
		char *sig = (char*)xsdt->tables[i], *target = "APIC";
        for (; *target && (*sig == *target); sig++, target++);
        if (!*target) {
            madt = (MADT*)xsdt->tables[i];
        }
	}

    u64_t header = ((u64_t)madt + sizeof(*madt));
    for (; header < (u64_t)madt + madt->header.Length; header += *(u08_t*)(header+1)) {
        if (*(u08_t*)header == 2) {
            IO_APIC_source_override_item_t *overover = (IO_APIC_source_override_item_t *)(header + 2);
            int aaaa = 1;
        }
    }
}