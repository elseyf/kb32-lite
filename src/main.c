
#include <string.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>

#include "utils.h"
#include "systick.h"
#include "hid.h"
#include "keyboard.h"

void clock_setup() {
    rcc_clock_setup_pll(&rcc_hsi_configs[RCC_CLOCK_HSI_48MHZ]);
}

void gpio_setup() {
    rcc_periph_clock_enable(RCC_GPIOA);
    rcc_periph_clock_enable(RCC_USB);
    gpio_set_mode(GPIOA, GPIO_MODE_OUTPUT_2_MHZ, GPIO_CNF_OUTPUT_PUSHPULL, GPIO12);
    gpio_clear(GPIOA, GPIO12);
    delay_ms(100);
}

int main(void) {
    clock_setup();
    systick_setup();
    gpio_setup();
    hid_setup();
    keyboard_init();
    
    timer_t _keyboard_timer;
    timer_init(&_keyboard_timer, 10, keyboard_poll);
    
    while(true) {
        usbd_poll(usbd_dev);
        timer_update(&_keyboard_timer);
    }
}
