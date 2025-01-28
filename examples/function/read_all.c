#include <stdbool.h>
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

int main() {
    stdio_init_all();

    // Initialize I2C
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C_SDA_PIN);
    gpio_pull_up(I2C_SCL_PIN);

    // Attempt to create the QwSTPad instance
    QwSTPad qwstpad;
    int init_result = qwstpad_init(&qwstpad, I2C_PORT, I2C_ADDRESS);
    if (init_result != PICO_OK) {
        printf("QwSTPad: Not Connected ... Exiting\n");
        return init_result;
    }

    printf("QwSTPad: Connected ... Starting\n");

    // Loop forever
    while (true) {
        // Read all the buttons from the qwstpad and print them out
        uint8_t buttons[NUM_BUTTONS];
        if (qwstpad_read_buttons(&qwstpad) < 0) {
            printf("QwSTPad: Disconnected .. Exiting\n");
            /* qwstpad_deinit(&qwstpad); */
            return 1;
        }

        for (int i = 0; i < NUM_BUTTONS; i++) {
            printf("Button %d = %d, ", i, buttons[i]);
        }
        printf("\n");

        sleep_ms(SLEEP_MS);
    }

    // Turn off all four LEDs if there is still a QwSTPad
    qwstpad_clear_leds(&qwstpad);
    qwstpad_deinit(&qwstpad);

    return 0;
}