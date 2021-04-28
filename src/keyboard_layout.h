
#ifndef KEYBOARD_LAYOUT_H
#define KEYBOARD_LAYOUT_H

#include "scancodes.h"
#include "keyboard.h"

const keyboard_t keyboard = {
    .rows = 8, .cols = 8,
    .row_io = {
        {IOMAP_BANK_A, 0}, {IOMAP_BANK_A, 1}, {IOMAP_BANK_A, 2}, {IOMAP_BANK_A, 3},
        {IOMAP_BANK_A, 4}, {IOMAP_BANK_A, 5}, {IOMAP_BANK_A, 6}, {IOMAP_BANK_A, 7}
    },
    .col_io = {
        {IOMAP_BANK_B, 8},  {IOMAP_BANK_B, 9},  {IOMAP_BANK_B, 10}, {IOMAP_BANK_B, 11},
        {IOMAP_BANK_B, 12}, {IOMAP_BANK_B, 13}, {IOMAP_BANK_B, 14}, {IOMAP_BANK_B, 15}
    },
    .layer = {
        {
            [0][0] = KEY_ESC, [4][0] = KEY_1, [0][1] = KEY_2, [4][1] = KEY_3, [0][2] = KEY_4, [4][2] = KEY_5, [0][3] = KEY_6, [4][3] = KEY_7, [0][4] = KEY_8, [4][4] = KEY_9, [0][5] = KEY_0, [4][5] = KEY_MINUS, [0][6] = KEY_EQUAL, [4][6] = KEY_BACKSPACE,
            [1][0] = KEY_TAB, [5][0] = KEY_Q, [1][1] = KEY_W, [5][1] = KEY_E, [1][2] = KEY_R, [5][2] = KEY_T, [1][3] = KEY_Y, [5][3] = KEY_U, [1][4] = KEY_I, [5][4] = KEY_O, [1][5] = KEY_P, [5][5] = KEY_LEFTBRACE, [1][6] = KEY_RIGHTBRACE, [5][6] = KEY_BACKSLASH,
            [2][0] = KEY_LAYER_0, [6][0] = KEY_A, [2][1] = KEY_S, [6][1] = KEY_D, [2][2] = KEY_F, [6][2] = KEY_G, [2][3] = KEY_H, [6][3] = KEY_J, [2][4] = KEY_K, [6][4] = KEY_L, [2][5] = KEY_SEMICOLON, [6][5] = KEY_APOSTROPHE, [2][6] = KEY_ENTER,
            [3][0] = KEY_LEFTSHIFT, [7][0] = KEY_Z, [3][1] = KEY_X, [7][1] = KEY_C, [3][2] = KEY_V, [7][2] = KEY_B, [3][3] = KEY_N, [7][3] = KEY_M, [3][4] = KEY_COMMA, [7][4] = KEY_DOT, [3][5] = KEY_SLASH, [7][5] = KEY_RIGHTSHIFT,
            [0][7] = KEY_LEFTCTRL, [1][7] = KEY_LEFTMETA, [2][7] = KEY_LEFTALT, [3][7] = KEY_SPACE, [4][7] = KEY_LAYER_0, [5][7] = KEY_COMPOSE, [6][7] = KEY_RIGHTMETA, [7][7] = KEY_RIGHTCTRL
        },
        {
            [4][0] = KEY_F1, [0][1] = KEY_F2, [4][1] = KEY_F3, [0][2] = KEY_F4, [4][2] = KEY_F5, [0][3] = KEY_F6,
            [4][3] = KEY_F7, [0][4] = KEY_F8, [4][4] = KEY_F9, [0][5] = KEY_F10, [4][5] = KEY_F11, [0][6] = KEY_F12,
            [1][4] = KEY_UP, [6][3] = KEY_LEFT, [2][4] = KEY_DOWN, [6][4] = KEY_RIGHT,
            [4][6] = KEY_DELETE, [5][3] = KEY_PAGEUP, [5][4] = KEY_PAGEDOWN
        }
    }
};

#endif
