#ifndef QWSTPAD_H
#define QWSTPAD_H

#include "hardware/i2c.h"

#define NUM_LEDS 4
#define NUM_BUTTONS 10

#define DEFAULT_ADDRESS 0x21
#define ALT_ADDRESS_1 0x23
#define ALT_ADDRESS_2 0x25
#define ALT_ADDRESS_3 0x27

#define INPUT_PORT0 0x00
#define INPUT_PORT1 0x01
#define OUTPUT_PORT0 0x02
#define OUTPUT_PORT1 0x03
#define POLARITY_PORT0 0x04
#define POLARITY_PORT1 0x05
#define CONFIGURATION_PORT0 0x06
#define CONFIGURATION_PORT1 0x07

extern const uint8_t BUTTON_MAPPING[NUM_BUTTONS];
extern const uint8_t LED_MAPPING[NUM_LEDS];
extern const uint8_t ADDRESSES[4];

typedef struct {
    i2c_inst_t *i2c;
    uint8_t address;
    uint16_t button_states;
    uint8_t led_states;
} QwSTPad;

int reg_write_uint16(i2c_inst_t *i2c, uint8_t address, uint8_t reg, uint16_t value);
int16_t reg_read_int16(i2c_inst_t *i2c, uint8_t address, uint8_t reg);
int qwstpad_init(QwSTPad *pad, i2c_inst_t *i2c, uint8_t address);
int16_t qwstpad_read_buttons(QwSTPad *pad);
void qwstpad_set_leds(QwSTPad *pad, uint8_t states);
void qwstpad_set_led(QwSTPad *pad, uint8_t led, bool state);
void qwstpad_clear_leds(QwSTPad *pad);

#endif // QWSTPAD_H