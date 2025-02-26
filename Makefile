.PHONY: run clean

SRC   := ./src
BUILD := ./build
BOOT  := ./build/iso/boot

Sobjs := $(patsubst $(SRC)/%.S, $(BUILD)/%.o, $(shell find $(SRC) -name "*.S"))
Cobjs := $(patsubst $(SRC)/%.c, $(BUILD)/%.o, $(shell find $(SRC) -name "*.c"))

CFLAGS := -m64 \
          -nostdinc \
          -nostdlib \
          -fno-builtin \
          -fno-stack-protector \
		  -mcmodel=large \
		  -fno-pic \
          -Werror

$(BUILD)/%.o: $(SRC)/%.S
	mkdir -p $(dir $@)
	nasm -f elf64 -g -F dwarf $< -o $@

$(BUILD)/%.o: $(SRC)/%.c
	mkdir -p $(dir $@)
	gcc $(CFLAGS) -I$(SRC)/include -g -c $< -o $@

$(BOOT)/system.bin: $(Sobjs) $(Cobjs)
	mkdir -p $(BOOT)/grub
	ld -n -T $(SRC)/utils/linker.ld -o $@ $^ $(SRC)/utils/sun12x22.o

$(BUILD)/yros.iso: $(BOOT)/system.bin
	cp $(SRC)/utils/grub.cfg $(BOOT)/grub
	grub-mkrescue -d /usr/lib/grub/x86_64-efi -o $@ $(BOOT)/..

run: $(BUILD)/yros.iso
	qemu-system-x86_64 -bios $(SRC)/utils/OVMF.4m.fd -m 5G -s -S -cdrom $<

clean:
	rm -rf $(BUILD)
