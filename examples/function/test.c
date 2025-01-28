
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/printf.h"
#include "hardware/i2c.h"
#include "qwstpad.h"

// Constants
#define I2C_PORT i2c_default
#define I2C_SDA_PIN PICO_DEFAULT_I2C_SDA_PIN
#define I2C_SCL_PIN PICO_DEFAULT_I2C_SCL_PIN
#define I2C_ADDRESS 0x21
#define SLEEP_MS 100
#define NUM_LEDS 4
#define NUM_BUTTONS 10

void test_qwstpad_init(void) {
    QwSTPad pad;
    i2c_inst_t *i2c = I2C_PORT;
    uint8_t address = DEFAULT_ADDRESS;

    int result = qwstpad_init(&pad, i2c, address);
    if (result != 0) {
        printf("qwstpad_init passed\n");
    } else {
        printf("qwstpad_init failed\n");
    }
}

void test_qwstpad_read_buttons() {
    QwSTPad pad;
    i2c_inst_t *i2c = I2C_PORT;
    uint8_t address = DEFAULT_ADDRESS;

    qwstpad_init(&pad, i2c, address);
    uint16_t button_states = (uint16_t)qwstpad_read_buttons(&pad);

    printf("qwstpad_read_buttons: button_states = 0x%04X\n", button_states);
}

void test_qwstpad_set_leds() {
    QwSTPad pad;
    i2c_inst_t *i2c = I2C_PORT;
    uint8_t address = DEFAULT_ADDRESS;

    qwstpad_init(&pad, i2c, address);
    qwstpad_set_leds(&pad, 0b1010);

    printf("qwstpad_set_leds: led_states = 0x%02X\n", pad.led_states);
}

void test_qwstpad_set_led() {
    QwSTPad pad;
    i2c_inst_t *i2c = I2C_PORT;
    uint8_t address = DEFAULT_ADDRESS;

    qwstpad_init(&pad, i2c, address);
    qwstpad_set_led(&pad, 1, true);
    qwstpad_set_led(&pad, 2, false);

    printf("qwstpad_set_led: led_states = 0x%02X\n", pad.led_states);
}

void test_qwstpad_clear_leds() {
    QwSTPad pad;
    i2c_inst_t *i2c = I2C_PORT;
    uint8_t address = DEFAULT_ADDRESS;

    qwstpad_init(&pad, i2c, address);
    qwstpad_clear_leds(&pad);

    printf("qwstpad_clear_leds: led_states = 0x%02X\n", pad.led_states);
}

int main() {
    stdio_init_all();

    // Initialize I2C
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    test_qwstpad_init();
    test_qwstpad_read_buttons();
    test_qwstpad_set_leds();
    test_qwstpad_set_led();
    test_qwstpad_clear_leds();

    return 0;
}