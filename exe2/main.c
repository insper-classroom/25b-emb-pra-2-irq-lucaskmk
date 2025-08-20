#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>

const int BTN_PIN_R = 28;
const int LED = 4;

volatile bool btn_fall_flag = false;
volatile uint32_t press_count = 0;

void btn_callback(uint gpio, uint32_t events) {
  if (events == 0x4) { // fall edge
    btn_fall_flag = true;
  }
}

int main() {
  stdio_init_all();

  gpio_init(LED);
  gpio_set_dir(LED, GPIO_OUT);

  gpio_init(BTN_PIN_R);
  gpio_set_dir(BTN_PIN_R, GPIO_IN);
  gpio_pull_up(BTN_PIN_R);

  gpio_set_irq_enabled_with_callback(
      BTN_PIN_R, GPIO_IRQ_EDGE_FALL, true, &btn_callback);

  while (true) {
    if (btn_fall_flag) {
      btn_fall_flag = false;
      press_count++;
      gpio_put(LED, press_count % 2);
    }
  }
}
