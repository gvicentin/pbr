#ifndef ULN2003_H
#define ULN2003_H

#include <stdbool.h>
#include <stdint.h>

// Stepping method
typedef enum {
    ULN2003_WAVE_DRIVE = 0,
    ULN2003_FULL_STEP,
    ULN2003_HALF_STEP
} uln2003_sm;

typedef struct uln2003_config_t {
    // Select stepping method
    uln2003_sm smethod;

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
