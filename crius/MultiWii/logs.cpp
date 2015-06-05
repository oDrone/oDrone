#include <stdio.h>
#include <Arduino.h>
#include "Serial.h"

int serial_write(uint8_t val)
{
    SerialWrite(0, val);
}

int serial_put_string(const char *str)
{
    while (*str) {
        serial_write(*str++);
    }

    return 0;
}

int serial_puts(const char *str)
{
    serial_put_string(str);

    serial_write('\n');

    return 0;
}

int serial_printf(const char *fmt, ...)
{
    va_list ap;

    static char buf[256];

    va_start(ap, fmt);

    vsnprintf(buf, sizeof(buf), fmt, ap);

    serial_put_string(buf);

    va_end(ap);

    return 0;
}

#if 0
int serial_print_dec(uint8_t opcode, int32_t integer)
{
    Serial.write(&opcode, 1);
    Serial.println(integer, DEC);
    return 0;
}

int serial_print_hex(uint8_t opcode, int32_t integer)
{
    Serial.write(&opcode, 1);
    Serial.println(integer, HEX);
    return 0;
}
#endif
