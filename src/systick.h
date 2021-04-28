
#ifndef SYSTICK_H
#define SYSTICK_H

#include <stdint.h>
#include <libopencm3/cm3/systick.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/rcc.h>

extern volatile uint32_t systick_time_ms;

void systick_setup();
void delay_ms(uint32_t _ms);
uint32_t millis();

typedef struct {
    uint32_t period, start;
    void (*callback)(void);
} timer_t;

timer_t* timer_init(timer_t* _timer, uint16_t _period_ms, void (*_callback)(void));
void timer_update(timer_t* _timer);

#endif

