#include <stdio.h>
#include "pico/stdlib.h"

#include "log.h"
#include "imu.h"

bool user_loop(repeating_timer_t *tr) {
    imu_calculate();
    return false;
}


int main(void) {
    stdio_init_all();

    LOG_DEBUG("Pico Balancing Robot");

    // Init
    imu_init(false);

    // Setup main loop
    add_repeating_timer_ms(200, user_loop, NULL, NULL);

    return 0;
}
