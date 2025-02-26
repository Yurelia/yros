#ifndef YROS_IOAPIC_H
#define YROS_IOAPIC_H

#include <yros/types.h>

typedef struct {
    char  Signature[4];
    u32_t Length;
    u08_t Revision;
    u08_t Checksum;
    char  OEMID[6];
    char  OEMTableID[8];
    u32_t OEMRevision;
    u32_t CreatorID;
    u32_t CreatorRevision;
} __packed__ acpi_header_t;

typedef struct {
    acpi_header_t header;
    u64_t tables[];
} __packed__ XSDT;

typedef struct {
    acpi_header_t header;
    u32_t lapic_base;
    u32_t flags;
} __packed__ MADT;

typedef struct {
    u08_t type;
    u08_t size;
} __packed__ madt_header_t;

typedef struct {
    u08_t ioapic_id;
    u08_t reserved;
    u32_t address;
    u32_t global_system_interrupt_base;
} __packed__ IO_APIC_Item;

typedef struct {
    u08_t bus_source;
    u08_t irq_source;
    u32_t    global_system_interrupt;
    u16_t    flags;
} __packed__ IO_APIC_source_override_item_t;

void init_ioapic();

#endif