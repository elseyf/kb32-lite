
#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <stdint.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "utils.h"

#define MAX_LAYERS  4
#define MAX_ROW_IO  32
#define MAX_COL_IO  32

typedef enum {
    IOMAP_BANK_A = 0,
    IOMAP_BANK_B = 1,
    IOMAP_BANK_C = 2,
    IOMAP_BANK_D = 3,
    IOMAP_BANK_E = 4,
    IOMAP_BANK_F = 5,
    IOMAP_BANK_G = 6,
    IOMAP_BANK_INVALID = 255
} __bank_t;

typedef uint8_t bank_t;
typedef uint8_t pin_t;

typedef struct {
    bank_t  bank;
    pin_t   pin;
} __attribute__((__packed__)) io_t;

typedef struct {
    uint8_t     rows, cols;
    io_t        row_io[MAX_ROW_IO], col_io[MAX_COL_IO];
    uint8_t     layer[MAX_LAYERS][MAX_ROW_IO][MAX_COL_IO];
} __attribute__((__packed__)) keyboard_t;

void keyboard_init();
void keyboard_disable();
void keyboard_poll();

#endif
