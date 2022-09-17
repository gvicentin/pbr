#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "log.h"

#define IN_1 0
#define IN_2 1
#define IN_3 2
#define IN_4 3

static uint32_t m_bit_mask;
static uint32_t m_sequence_mask[4];

void step(uint32_t steps);

int main(void) {
    stdio_init_all();

    // Init GPIO
    m_bit_mask = (1 << IN_1) | (1 << IN_2) |
                 (1 << IN_3) | (1 << IN_4);
    gpio_init_mask(m_bit_mask);
    gpio_set_dir_out_masked(m_bit_mask);

    // Init sequence steps
    bool fullstep = true;
    if (fullstep) {
        m_sequence_mask[0] = (1 << IN_1) | (1 << IN_4);
        m_sequence_mask[1] = (1 << IN_1) | (1 << IN_2);
        m_sequence_mask[2] = (1 << IN_2) | (1 << IN_3);
        m_sequence_mask[3] = (1 << IN_3) | (1 << IN_4);
    }
    else {
        m_sequence_mask[0] = (1 << IN_1);
        m_sequence_mask[1] = (1 << IN_2);
        m_sequence_mask[2] = (1 << IN_3);
        m_sequence_mask[3] = (1 << IN_4);
    }

    gpio_put_masked(m_bit_mask, m_sequence_mask[0]);

    while (true) {
        step(4096);
        sleep_ms(2500);
    }

    return 0;
}

void step(uint32_t steps) {
    for (uint32_t i = 0; i < steps; i++) {
        gpio_put_masked(m_bit_mask, m_sequence_mask[i % 4]);
        busy_wait_us(1300);
    }
}
