#include <stdio.h>
#include "pico/stdlib.h"

#include "log.h"
#include "imu.h"

bool user_loop(repeating_timer_t *tr) {
    imu_calculate();
    return true;
}


int main(void) {
    stdio_init_all();
    sleep_ms(1000);

    LOG_DEBUG("Pico Balancing Robot");

    // Init
    imu_init(true);

    // Setup main loop
    repeating_timer_t timer;
    add_repeating_timer_ms(50, user_loop, NULL, &timer);
    while (true) {
        tight_loop_contents();
    }

    return 0;
}
