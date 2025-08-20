#include <stdio.h>

#include "hardware/gpio.h"
#include "pico/stdlib.h"

const int BTN_PIN_R = 28;
const int BTN_PIN_G = 26;

volatile bool btn_r_flag = false;
volatile bool btn_g_flag = false;

void btn_callback(uint gpio, uint32_t events) {
    if (events == 0x4) {  // fall edge
        if (gpio == BTN_PIN_R) {
            btn_r_flag = true;
        } else if (gpio == BTN_PIN_G) {
            btn_g_flag = true;
        }
    }
}

int main() {
    stdio_init_all();

    gpio_init(BTN_PIN_R);
    gpio_set_dir(BTN_PIN_R, GPIO_IN);
    gpio_pull_up(BTN_PIN_R);

    gpio_init(BTN_PIN_G);
    gpio_set_dir(BTN_PIN_G, GPIO_IN);
    gpio_pull_up(BTN_PIN_G);

    gpio_set_irq_enabled_with_callback(BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true,
                                       &btn_callback);

    gpio_set_irq_enabled(BTN_PIN_G, GPIO_IRQ_EDGE_FALL, true);

    while (true) {
        if (btn_r_flag) {
            btn_r_flag = false;
            printf("fall red\n");
        }
        if (btn_g_flag) {
            btn_g_flag = false;
            printf("fall green\n");
        }
    }
}
