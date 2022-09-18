#ifndef ULN2003_H
#define ULN2003_H

#include <stdbool.h>
#include <stdint.h>

// 28BYJ48 Motor number of fullsteps.
// 
// stride angle: 5.625 
// gear ratio: 1/64
// (360/5.625)*64 = 4096
#define ULN2003_28BYJ48_STEPS 4096

typedef struct uln2003_config_t {
    // Total steps per revolution
    uint32_t steps_count;

    // Select fullstep or halfstep
    bool is_fullstep;

    // GPIO pins
    uint8_t pin_a, pin_b,
            pin_c, pin_d;
} uln2003_config;

typedef struct uln2003_t {
    uint32_t steps_count;

    uint32_t bit_mask;
    uint32_t sequence_mask[4];
    uint8_t current_mask;
    
    int32_t steps;
    uint64_t last_step_time;
    uint64_t delay_time;
} uln2003;

int uln2003_init(uln2003 *motor, uln2003_config config);

void uln2003_stop(uln2003 *motor);

void uln2003_set_rpm(uln2003 *motor, uint32_t rpm);

void uln2003_step(uln2003 *motor, int32_t amount);

bool uln2003_run(uln2003 *motor);

#endif // ULN2003_H
