#include <yros/keyboard.h>
#include <yros/io.h>

void init_keyboard() {
    //Let's do a keyboard read just to make sure it's empty
    inb(KEYBOARD_ENCODER_PORT);
    // The following two bytes will read the scancode set used by the keyboard
    outb(KEYBOARD_ENCODER_PORT, 0xF0);
    outb(KEYBOARD_ENCODER_PORT, 0x00);
    u08_t status_read = inb(KEYBOARD_ENCODER_PORT);
    if(status_read == KEYBOARD_ACK_BYTE) {
        status_read = inb(KEYBOARD_ENCODER_PORT);
    }
    outb(0x64, 0x20);
    status_read = inb(0x64);
    int a = 0;
    /*while((status_read & 2) != 0) {
        pretty_logf(Verbose, "Not ready yet... %x", status_read);
        status_read = inportb(PS2_STATUS_REGISTER);
    }
    uint8_t configuration_byte = inportb(PS2_DATA_REGISTER);
    if((configuration_byte & (1 << 6)) != 0) {
        pretty_log(Verbose, "Translation enabled");
        kernel_settings.keyboard.translation_enabled = true;
    } else {
        kernel_settings.keyboard.translation_enabled = false;
    }
    buf_position = 0;*/
}