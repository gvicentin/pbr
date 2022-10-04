#include <stdio.h>
#include "pico/stdlib.h"

#include "log.h"

bool user_loop(repeating_timer_t *tr) {
    return false;
}


int main(void) {
    stdio_init_all();

    LOG_DEBUG("Pico Balancing Robot");
    
    add_repeating_timer_ms(200, user_loop, NULL, NULL);

    return 0;
}
