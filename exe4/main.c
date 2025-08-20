#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int LED_R = 4;
const int LED_G = 6;
const int BTN_R = 28;
const int BTN_G = 26;

volatile bool red_flag = false;
volatile bool green_flag = false;

void btn_isr(uint gpio, uint32_t events) {
    if (gpio == BTN_R && (events & GPIO_IRQ_EDGE_FALL)) red_flag = true;
    if (gpio == BTN_G && (events & GPIO_IRQ_EDGE_RISE)) green_flag = true;
}

int main() {
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

  bool led_r_state = false;
  bool led_g_state = false;

  while (true) {
    if (red_flag) {
      red_flag = false;
      led_r_state = !led_r_state;
      gpio_put(LED_R, led_r_state);
    }
    if (green_flag) {
      green_flag = false;
      led_g_state = !led_g_state;
      gpio_put(LED_G, led_g_state);
    }
    tight_loop_contents();
  }
}
