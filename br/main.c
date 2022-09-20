#include <stdio.h>
#include "pico/stdlib.h"
#include "log.h"
#include "uln2003.h"


int main(void) {
    stdio_init_all();

    LOG_DEBUG("Step motor testing");

    uln2003 motor;
    uln2003_config motor_config = {
        .smethod = ULN2003_WAVE_DRIVE,
        .pin_a = 1,
        .pin_b = 2,
        .pin_c = 3,
        .pin_d = 4,
    };
    uln2003_init(&motor, motor_config);
    uln2003_step(&motor, -4096);

    while (true) {
        uln2003_run(&motor);
    }

    return 0;
}
