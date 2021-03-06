// -------------------------------------------------- //
// This file is autogenerated by pioasm; do not edit! //
// -------------------------------------------------- //

#pragma once

#if !PICO_NO_HARDWARE
#include "hardware/pio.h"
#endif

// ------ //
// SkyCOM //
// ------ //

#define SkyCOM_wrap_target 0
#define SkyCOM_wrap 5

static const uint16_t SkyCOM_program_instructions[] = {
            //     .wrap_target
    0x84a0, //  0: pull   block                  [4] 
    0xe081, //  1: set    pindirs, 1                 
    0xe03f, //  2: set    x, 31                      
    0x6001, //  3: out    pins, 1                    
    0x0643, //  4: jmp    x--, 3                 [6] 
    0xe000, //  5: set    pins, 0                    
            //     .wrap
};

#if !PICO_NO_HARDWARE
static const struct pio_program SkyCOM_program = {
    .instructions = SkyCOM_program_instructions,
    .length = 6,
    .origin = -1,
};

static inline pio_sm_config SkyCOM_program_get_default_config(uint offset) {
    pio_sm_config c = pio_get_default_sm_config();
    sm_config_set_wrap(&c, offset + SkyCOM_wrap_target, offset + SkyCOM_wrap);
    return c;
}

#include "hardware/clocks.h"
// this is a raw helper function for use by the user which sets up the GPIO output, and configures the SM to output on a particular pin
void PIO_SkyCOM_init(PIO pio, uint sm, uint offset, uint pin_tx, uint baud) {
    // Tell PIO to initially drive output-high on the selected pin, then map PIO
    // onto that pin with the IO muxes.
    pio_sm_set_pins_with_mask(pio, sm, 1u << pin_tx, 1u << pin_tx);
    pio_sm_set_pindirs_with_mask(pio, sm, 1u << pin_tx, 1u << pin_tx);
    pio_gpio_init(pio, pin_tx);
    pio_sm_config c = SkyCOM_program_get_default_config(offset);
    sm_config_set_set_pins(&c, pin_tx, 1);
    // OUT shifts to left, no autopull
    sm_config_set_out_shift(&c, false, false, 32);
    // We are mapping both OUT and side-set to the same pin, because sometimes
    // we need to assert user data onto the pin (with OUT) and sometimes
    // assert constant values (start/stop bit)
    sm_config_set_out_pins(&c, pin_tx, 1);
    //sm_config_set_sideset_pins(&c, pin_tx);
    // We only need TX, so get an 8-deep FIFO!
    sm_config_set_fifo_join(&c, PIO_FIFO_JOIN_TX);
    // SM transmits 1 bit per 8 execution cycles.
    float div = (float)clock_get_hz(clk_sys) / (8 * baud);
    sm_config_set_clkdiv(&c, div);
    pio_sm_init(pio, sm, offset, &c);
    pio_sm_set_enabled(pio, sm, true);
}
static inline void PIO_Send_Byte(PIO pio, uint sm, uint32_t c) {
    pio_sm_put(pio, sm, c);
}

#endif

