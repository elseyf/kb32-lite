
#include <string.h>
#include "keyboard.h"
#include "hid.h"
#include "keyboard_layout.h"
#include "scancodes.h"

const extern keyboard_t keyboard;

uint8_t keyboard_code, keyboard_last_code;
uint8_t keyboard_modifier, keyboard_last_modifier;
uint8_t keyboard_layer;

bool keyboard_matrix[num_elements(keyboard.row_io)][num_elements(keyboard.col_io)];
bool keyboard_last_matrix[num_elements(keyboard.row_io)][num_elements(keyboard.col_io)];

uint32_t io_bank_rcc(bank_t _bank) {
    switch(_bank) {
        case IOMAP_BANK_A:  return RCC_GPIOA;
        case IOMAP_BANK_B:  return RCC_GPIOB;
        case IOMAP_BANK_C:  return RCC_GPIOC;
        case IOMAP_BANK_D:  return RCC_GPIOD;
        case IOMAP_BANK_E:  return RCC_GPIOE;
        case IOMAP_BANK_F:  return RCC_GPIOF;
        case IOMAP_BANK_G:  return RCC_GPIOG;
        default:            return 0;
    };
}
uint32_t io_bank_addr(bank_t _bank) {
    switch(_bank) {
        case IOMAP_BANK_A:  return GPIOA;
        case IOMAP_BANK_B:  return GPIOB;
        case IOMAP_BANK_C:  return GPIOC;
        case IOMAP_BANK_D:  return GPIOD;
        case IOMAP_BANK_E:  return GPIOE;
        case IOMAP_BANK_F:  return GPIOF;
        case IOMAP_BANK_G:  return GPIOG;
        default:            return 0;
    };
}

uint16_t io_pin_mask(pin_t _pin) {
    return (1 << _pin);
}

bool keyboard_matrix_get(uint8_t _row, uint8_t _col) {
    return keyboard_matrix[_row][_col];
}
void keyboard_matrix_set(uint8_t _row, uint8_t _col, bool _set) {
    keyboard_matrix[_row][_col] = _set;
}
bool keyboard_last_matrix_get(uint8_t _row, uint8_t _col) {
    return keyboard_last_matrix[_row][_col];
}
void keyboard_last_matrix_set(uint8_t _row, uint8_t _col, bool _set) {
    keyboard_last_matrix[_row][_col] = _set;
}
void keyboard_matrix_copy() {
    memcpy(keyboard_last_matrix, keyboard_matrix, sizeof(keyboard_last_matrix));
}

bool keyboard_key_down(uint8_t _row, uint8_t _col) {
    return keyboard_matrix_get(_row, _col);
}
bool keyboard_key_up(uint8_t _row, uint8_t _col) {
    return !keyboard_matrix_get(_row, _col);
}

bool keyboard_key_push(uint8_t _row, uint8_t _col) {
    return (!keyboard_last_matrix_get(_row, _col) && keyboard_matrix_get(_row, _col));
}
bool keyboard_key_release(uint8_t _row, uint8_t _col) {
    return (keyboard_last_matrix_get(_row, _col) && !keyboard_matrix_get(_row, _col));
}

bool row_get(pin_t _pin) {
    return gpio_get(io_bank_addr(keyboard.row_io[_pin].bank), io_pin_mask(keyboard.row_io[_pin].pin));
}
bool col_get(pin_t _pin) {
    return gpio_get(io_bank_addr(keyboard.col_io[_pin].bank), io_pin_mask(keyboard.col_io[_pin].pin));
}
void row_set(pin_t _pin) {
    gpio_set(io_bank_addr(keyboard.row_io[_pin].bank), io_pin_mask(keyboard.row_io[_pin].pin));
}
void row_clear(pin_t _pin) {
    gpio_clear(io_bank_addr(keyboard.row_io[_pin].bank), io_pin_mask(keyboard.row_io[_pin].pin));
}
void col_set(pin_t _pin) {
    gpio_set(io_bank_addr(keyboard.col_io[_pin].bank), io_pin_mask(keyboard.col_io[_pin].pin));
}
void col_clear(pin_t _pin) {
    gpio_clear(io_bank_addr(keyboard.col_io[_pin].bank), io_pin_mask(keyboard.col_io[_pin].pin));
}

void keyboard_init() {
    for(size_t i = 0; i < keyboard.rows; i++) {
        rcc_periph_clock_enable(io_bank_rcc(keyboard.row_io[i].bank));
        gpio_set(io_bank_addr(keyboard.row_io[i].bank), io_pin_mask(keyboard.row_io[i].pin));
        gpio_set_mode(io_bank_addr(keyboard.row_io[i].bank), GPIO_MODE_OUTPUT_50_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, io_pin_mask(keyboard.row_io[i].pin));
    }
    for(size_t i = 0; i < keyboard.cols; i++) {
        rcc_periph_clock_enable(io_bank_rcc(keyboard.col_io[i].bank));
        gpio_set(io_bank_addr(keyboard.col_io[i].bank), io_pin_mask(keyboard.col_io[i].pin));
        gpio_set_mode(io_bank_addr(keyboard.col_io[i].bank), GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, io_pin_mask(keyboard.col_io[i].pin));
    }
    
    memset(keyboard_matrix, 0, sizeof(keyboard_matrix));
    memset(keyboard_last_matrix, 0, sizeof(keyboard_last_matrix));
}

void keyboard_disable() {
    for(size_t i = 0; i < keyboard.rows; i++) {
        gpio_set(io_bank_addr(keyboard.row_io[i].bank), io_pin_mask(keyboard.row_io[i].pin));
        gpio_set_mode(io_bank_addr(keyboard.row_io[i].bank), GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, io_pin_mask(keyboard.row_io[i].pin));
    }
    for(size_t i = 0; i < keyboard.cols; i++) {
        gpio_set(io_bank_addr(keyboard.col_io[i].bank), io_pin_mask(keyboard.col_io[i].pin));
        gpio_set_mode(io_bank_addr(keyboard.col_io[i].bank), GPIO_MODE_INPUT, GPIO_CNF_INPUT_PULL_UPDOWN, io_pin_mask(keyboard.col_io[i].pin));
    }
    
    memset(keyboard_matrix, 0, sizeof(keyboard_matrix));
    memset(keyboard_last_matrix, 0, sizeof(keyboard_last_matrix));
    hid_write(0, 0);
}

void keyboard_read_matrix() {
    memset(keyboard_matrix, 0, sizeof(keyboard_matrix));
    for(uint8_t _r = 0; _r < keyboard.rows; _r++) {
        for(uint8_t i = 0; i < keyboard.rows; i++) row_set(i);
        row_clear(_r);
        for(uint8_t _c = 0; _c < keyboard.cols; _c++) {
            keyboard_matrix_set(_r, _c, !col_get(_c));
        }
    }
}

uint8_t keyboard_read_layer(uint8_t _layer, uint8_t _row, uint8_t _col) {
    return keyboard.layer[_layer][_row][_col];
}

uint8_t layer_poll(uint8_t _layer) {
    for(uint8_t _r = 0; _r < keyboard.rows; _r++) {
        for(uint8_t _c = 0; _c < keyboard.cols; _c++) {
            if(keyboard_key_push(_r, _c)) {
                uint8_t _code = keyboard_read_layer(_layer, _r, _c);
                switch(_code) {
                    case KEY_NONE:
                    case KEY_LAYER_0:
                    case KEY_LAYER_1:
                    case KEY_LAYER_2:
                    case KEY_LAYER_3:
                    case KEY_LEFTSHIFT:
                    case KEY_LEFTCTRL:
                    case KEY_LEFTMETA:
                    case KEY_LEFTALT:
                    case KEY_RIGHTSHIFT:
                    case KEY_RIGHTMETA:
                    case KEY_RIGHTCTRL:
                        break;
                    default:
                        return _code;
                }
            }
        }
    }
    return KEY_NONE;
}

uint8_t modifier_poll(uint8_t _layer) {
    uint8_t _modifier = 0;
    for(uint8_t _r = 0; _r < keyboard.rows; _r++) {
        for(uint8_t _c = 0; _c < keyboard.cols; _c++) {
            if(keyboard_key_down(_r, _c)) {
                switch(keyboard_read_layer(_layer, _r, _c)) {
                    case KEY_LEFTSHIFT:
                        _modifier |= KEY_MOD_LSHIFT;
                        break;
                    case KEY_LEFTCTRL:
                        _modifier |= KEY_MOD_LCTRL;
                        break;
                    case KEY_LEFTMETA:
                        _modifier |= KEY_MOD_LMETA;
                        break;
                    case KEY_LEFTALT:
                        _modifier |= KEY_MOD_LALT;
                        break;
                    case KEY_RIGHTSHIFT:
                        _modifier |= KEY_MOD_RSHIFT;
                        break;
                    case KEY_RIGHTMETA:
                        _modifier |= KEY_MOD_RMETA;
                        break;
                    case KEY_RIGHTCTRL:
                        _modifier |= KEY_MOD_RCTRL;
                        break;
                }
            }
        }
    }
    return _modifier;
}

bool keyboard_code_down(uint8_t _layer, uint8_t _code) {
    for(uint8_t _r = 0; _r < keyboard.rows; _r++) {
        for(uint8_t _c = 0; _c < keyboard.cols; _c++) {
            if(keyboard_key_down(_r, _c)) {
                if(keyboard_read_layer(_layer, _r, _c) == _code)
                    return true;
            }
        }
    }
    return false;
}

bool keyboard_code_up(uint8_t _layer, uint8_t _code) {
    for(uint8_t _r = 0; _r < keyboard.rows; _r++) {
        for(uint8_t _c = 0; _c < keyboard.cols; _c++) {
            if(keyboard_key_up(_r, _c)) {
                if(keyboard_read_layer(_layer, _r, _c) == _code)
                    return true;
            }
        }
    }
    return false;
}

void keyboard_poll() {
    keyboard_last_code = keyboard_code;
    keyboard_last_modifier = keyboard_modifier;
    
    keyboard_matrix_copy();
    keyboard_read_matrix();
    
    keyboard_layer = 0;
    if(keyboard_code_down(0, KEY_LAYER_0))      keyboard_layer = 1;
    else if(keyboard_code_down(0, KEY_LAYER_1)) keyboard_layer = 2;
    else if(keyboard_code_down(0, KEY_LAYER_2)) keyboard_layer = 3;
    else if(keyboard_code_down(0, KEY_LAYER_3)) keyboard_layer = 4;
    
    keyboard_modifier = modifier_poll(keyboard_layer);
    keyboard_code = layer_poll(keyboard_layer);
    
    if(keyboard_code == KEY_NONE) keyboard_code = keyboard_code_down(keyboard_layer, keyboard_last_code) ? keyboard_last_code : KEY_NONE;
    
    if((keyboard_code != keyboard_last_code) || (keyboard_modifier != keyboard_last_modifier)) {
        hid_write(keyboard_modifier, keyboard_code);
    }
}
