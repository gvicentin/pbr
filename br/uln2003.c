#include "uln2003.h"

#include "pico/stdlib.h"
#include "log.h"

#define ULN2003_DEFAULT_RPM 10

int uln2003_init(uln2003 *motor, uln2003_config config) {
    motor->steps_count = config.steps_count;
    motor->last_step_time = 0;
    motor->steps = 0;

    // Init GPIO pins
    motor->bit_mask = (1 << config.pin_a) | (1 << config.pin_b) |
                      (1 << config.pin_c) | (1 << config.pin_d);
    gpio_init_mask(motor->bit_mask);
    gpio_set_dir_out_masked(motor->bit_mask);

    // Init step sequence
    if (config.is_fullstep) {
        motor->sequence_mask[0] = (1 << config.pin_a) | (1 << config.pin_d);
        motor->sequence_mask[1] = (1 << config.pin_a) | (1 << config.pin_b);
        motor->sequence_mask[2] = (1 << config.pin_b) | (1 << config.pin_c);
        motor->sequence_mask[3] = (1 << config.pin_c) | (1 << config.pin_d);
    }
    else {
        motor->sequence_mask[0] = (1 << config.pin_a);
        motor->sequence_mask[1] = (1 << config.pin_b);
        motor->sequence_mask[2] = (1 << config.pin_c);
        motor->sequence_mask[3] = (1 << config.pin_d);
    }

    uln2003_stop(motor);
    uln2003_set_rpm(motor, ULN2003_DEFAULT_RPM);

    return 0;
}

void uln2003_stop(uln2003 *motor) {
    gpio_put_masked(motor->bit_mask, 0x00);
    motor->current_mask = 0;
}

void uln2003_set_rpm(uln2003 *motor, uint32_t rpm) {
    motor->delay_time = 6e7 / motor->steps_count / rpm;
}

void uln2003_step(uln2003 *motor, int32_t amount) {
    motor->steps = amount;
    motor->last_step_time = time_us_64() + motor->delay_time;
}

bool uln2003_run(uln2003 *motor) {
    if (motor->steps == 0) {
        // Done moving
        return true;
    }

    uint64_t now = time_us_64();
    if (now - motor->last_step_time >= motor->delay_time) {
        // It's time to give a step
        gpio_put_masked(motor->bit_mask, motor->current_mask);
        motor->last_step_time = now;

        if (motor->steps > 0) {
            // Walk forward in the sequence
            motor->current_mask = (motor->current_mask + 1) % 4;
            motor->steps -= 1;
        }
        else {
            // Walk backwards in the sequence
            int8_t next_mask = motor->current_mask - 1;
            motor->current_mask = next_mask < 0 ? 3 : next_mask;
            motor->steps += 1;
        }

        if (motor->steps == 0) {
            // Stop the motor if we don't to move anymore
            uln2003_stop(motor);
        }
    }

    return false;
}
