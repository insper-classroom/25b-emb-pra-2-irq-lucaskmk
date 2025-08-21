#include "hardware/gpio.h"
#include "pico/stdlib.h"
#include <stdio.h>
#include <stdbool.h>

static const uint BTN_PIN = 28;

static volatile bool fall_flag = false;
static volatile bool rise_flag = false;

static void btn_isr(uint gpio, uint32_t events) {
    if (gpio != BTN_PIN) return;
    if (events & GPIO_IRQ_EDGE_FALL) fall_flag = true;
    if (events & GPIO_IRQ_EDGE_RISE) rise_flag = true;
}

int main(void) {
stdio_init_all();

gpio_init(BTN_PIN);
gpio_set_dir(BTN_PIN, GPIO_IN);
gpio_pull_up(BTN_PIN);

gpio_set_irq_enabled_with_callback(
  BTN_PIN, GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_isr);

volatile int capture_flag = 0;
bool was_pressed = false;

while (true) {
  if (fall_flag) {
      fall_flag = false;
      if (!was_pressed) {
          printf("btn pressed \n");
          was_pressed = true;
      }
  }
  if (rise_flag) {
      rise_flag = false;
      if (was_pressed) {
          printf("btn released \n");
          capture_flag = 1;
          was_pressed = false;
      }
  }
  if (capture_flag) {
      capture_flag = 0;
  }

  tight_loop_contents();
}
}
