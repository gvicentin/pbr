#include <stdio.h>
#include "pico/stdlib.h"
#include "log.h"
#include "uln2003.h"


int main(void) {
    stdio_init_all();

    LOG_DEBUG("Step motor testing");

    uln2003 motor;
    uln2003_config motor_config = {
        .steps_count = ULN2003_28BYJ48_STEPS,
        .pin_a = 1,
        .pin_b = 2,
        .pin_c = 3,
        .pin_d = 4,
        .is_fullstep = true
    };

    uln2003_init(&motor, motor_config);
    uln2003_step(&motor, 10 * ULN2003_28BYJ48_STEPS); // Give 10 full revolutions

    LOG_DEBUG("%u", motor.delay_time);
    
    while (true) {
        uln2003_run(&motor);
    }

    return 0;
}
