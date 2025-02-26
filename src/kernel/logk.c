#include <yros/logk.h>

static char buf[100];

void logkint(long long value) {
	static char *screen = (char*)0xB8010;
	const int base = 16;
	char *ptr = buf;
	do {
        *ptr++ = "0123456789abcdefghijklmnopqrstuvwxyz"[value % base];
        value /= base;
    } while (value);
    do {
		*screen++ = *(--ptr);
		*screen++ = 15;
	} while (ptr != buf);
}