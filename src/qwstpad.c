//#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "qwstpad.h"

const uint8_t BUTTON_MAPPING[NUM_BUTTONS] = {0xE, 0xC, 0xF, 0xD, 0x1, 0x4, 0x2, 0x3, 0xB, 0x5};
const uint8_t LED_MAPPING[NUM_LEDS] = {0x6, 0x7, 0x9, 0xA};
const uint8_t ADDRESSES[4] = {DEFAULT_ADDRESS, ALT_ADDRESS_1, ALT_ADDRESS_2, ALT_ADDRESS_3};

int reg_write_uint16(i2c_inst_t *i2c, uint8_t address, uint8_t reg, uint16_t value) {
    uint8_t buffer[3] = {reg, value & 0xFF, (value >> 8) & 0xFF};
    return i2c_write_blocking(i2c, address, buffer, 3, false);
}

int16_t reg_read_int16(i2c_inst_t *i2c, uint8_t address, uint8_t reg) {
    uint8_t buffer[2];
    if (i2c_write_blocking(i2c, address, &reg, 1, true) < 0) return PICO_ERROR_GENERIC;
    if (i2c_read_blocking(i2c, address, buffer, 2, false) < 0) return PICO_ERROR_GENERIC;
    return buffer[0] | (buffer[1] << 8);
}

// return PICO_OK if successful otherwise return PICO_ERROR_GENERIC
int qwstpad_init(QwSTPad *pad, i2c_inst_t *i2c, uint8_t address) {
    pad->i2c = i2c;
    pad->address = address;
    pad->button_states = 0;
    pad->led_states = 0;

    if (reg_write_uint16(i2c, address, CONFIGURATION_PORT0, 0b1111100100111111) < 0) return PICO_ERROR_GENERIC;
    if (reg_write_uint16(i2c, address, POLARITY_PORT0, 0b1111100000111111) < 0) return PICO_ERROR_GENERIC;
    if (reg_write_uint16(i2c, address, OUTPUT_PORT0, 0b0000011011000000) < 0) return PICO_ERROR_GENERIC;

    return PICO_OK;
}

int16_t qwstpad_read_buttons(QwSTPad *pad) {
    int16_t result = reg_read_int16(pad->i2c, pad->address, INPUT_PORT0);
    if (result < 0) return result;
    pad->button_states = result;
    return pad->button_states;
}

void qwstpad_set_leds(QwSTPad *pad, uint8_t states) {
    pad->led_states = states & 0b1111;
    uint16_t output = 0;
    for (int i = 0; i < NUM_LEDS; i++) {
        output |= ((pad->led_states & (1 << i)) ? 0 : 1) << LED_MAPPING[i];
    }
    reg_write_uint16(pad->i2c, pad->address, OUTPUT_PORT0, output);
}

void qwstpad_set_led(QwSTPad *pad, uint8_t led, bool state) {
    if (led < 1 || led > NUM_LEDS) return;
    if (state) {
        pad->led_states |= (1 << (led - 1));
    } else {
        pad->led_states &= ~(1 << (led - 1));
    }
    qwstpad_set_leds(pad, pad->led_states);
}

void qwstpad_clear_leds(QwSTPad *pad) {
    pad->led_states = 0;
    qwstpad_set_leds(pad, pad->led_states);
}
