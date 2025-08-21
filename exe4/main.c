#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdbool.h>

static const uint LED_R = 4;
static const uint LED_G = 6;
static const uint BTN_R = 28;
static const uint BTN_G = 26;

static volatile bool red_flag = false;
static volatile bool green_flag = false;

static void btn_isr(uint gpio, uint32_t events) {
    if (gpio == BTN_R && (events & GPIO_IRQ_EDGE_FALL)) red_flag = true;
    if (gpio == BTN_G && (events & GPIO_IRQ_EDGE_RISE)) green_flag = true;
}

int main(void) {
    stdio_init_all();

    gpio_init(LED_R); 
    gpio_set_dir(LED_R, GPIO_OUT); 
    gpio_put(LED_R, 0);
    gpio_init(LED_G); 
    gpio_set_dir(LED_G, GPIO_OUT); 
    gpio_put(LED_G, 0);

    gpio_init(BTN_R); 
    gpio_set_dir(BTN_R, GPIO_IN); 
    gpio_pull_up(BTN_R);
    gpio_init(BTN_G); 
    gpio_set_dir(BTN_G, GPIO_IN); 
    gpio_pull_up(BTN_G);

    gpio_set_irq_enabled_with_callback(BTN_R, GPIO_IRQ_EDGE_FALL, true, &btn_isr);
    gpio_set_irq_enabled(BTN_G, GPIO_IRQ_EDGE_RISE, true);

    uint32_t rc = 0, gc = 0;

    while (true) {
        if (red_flag)   { red_flag = false;   gpio_put(LED_R, (++rc) & 1); }
        if (green_flag) { green_flag = false; gpio_put(LED_G, (++gc) & 1); }
        tight_loop_contents();
    }
}
