
#include "systick.h"

volatile uint32_t systick_time_ms;

void systick_setup() {
    systick_time_ms = 0;
    systick_set_clocksource(STK_CSR_CLKSOURCE_AHB_DIV8);
    systick_set_reload(((rcc_ahb_frequency / 8) / 1000) - 1);
    systick_interrupt_enable();
    systick_counter_enable();
}

void sys_tick_handler() {
    systick_time_ms++;
}

void delay_ms(uint32_t _ms) {
    uint32_t _start = systick_time_ms;
    while((systick_time_ms - _start) < _ms);
}

uint32_t millis()  {
    return systick_time_ms;
}

timer_t* timer_init(timer_t* _timer, uint16_t _period_ms, void (*_callback)(void)) {
    _timer->start = systick_time_ms;
    _timer->period = _period_ms;
    _timer->callback = _callback;
    return _timer;
}

void timer_update(timer_t* _timer) {
    if((systick_time_ms - _timer->start) >= _timer->period) {
        _timer->start += _timer->period;
        if(_timer->callback) _timer->callback();
    }
}
