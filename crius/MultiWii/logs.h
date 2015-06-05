#ifndef LOGS_H
#define LOGS_H

#include <stdarg.h>

#if 0
enum {
    OPCODE_LOGS = 0x89,
    OPCODE_OK = 0x70,
    OPCODE_ERR = 0x70,
};

#define serial_log serial_log_puts

#define serial_log_puts(_str) serial_puts(OPCODE_LOGS, _str);
#define serial_log_print_dec(_int) serial_print_dec(OPCODE_LOGS, _int)
#define serial_log_print_hex(_int) serial_print_dec(OPCODE_LOGS, _int)

#define serial_ok(_str) serial_puts(OPCODE_OK, _str);
#define serial_err(_str) serial_puts(OPCODE_ERR, _str);

#endif

int serial_puts(const char *str);
int serial_put_string(const char *str);
int serial_write(uint8_t value);
int serial_printf(const char *str, ...);

#if 0
int serial_print_dec(uint8_t opcode, int32_t integer);
int serial_print_hex(uint8_t opcode, int32_t integer);
#endif

#endif
