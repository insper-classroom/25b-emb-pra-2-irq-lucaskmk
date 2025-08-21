#include <stdio.h>
#include "hardware/gpio.h"
#include "pico/stdlib.h"

const uint BTN = 28;
const uint32_t LIMITE_LONGO_US = 800000;

volatile bool apertado = false;
volatile bool aperto_curto = false;
volatile bool aperto_longo = false;
volatile uint64_t t_inicio = 0;

void btn_isr(uint gpio, uint32_t events) {
    if (gpio != BTN) return;

    if (events & GPIO_IRQ_EDGE_FALL) {
        apertado = true;
        t_inicio = time_us_64();
    }
    if (events & GPIO_IRQ_EDGE_RISE) {
        apertado = false;
        uint64_t duracao = time_us_64() - t_inicio;

        if (duracao >= LIMITE_LONGO_US)
            aperto_longo = true;
        else
            aperto_curto = true;
    }
}

int main() {
    stdio_init_all();

    gpio_init(BTN);
    gpio_set_dir(BTN, GPIO_IN);
    gpio_pull_up(BTN);

    gpio_set_irq_enabled_with_callback(BTN,
        GPIO_IRQ_EDGE_FALL | GPIO_IRQ_EDGE_RISE, true, &btn_isr);

    while (true) {
        if (aperto_curto) {
            aperto_curto = false;
            printf("Aperto curto!\n");
        }
        if (aperto_longo) {
            aperto_longo = false;
            printf("Aperto longo!\n");
        }
        tight_loop_contents();
    }
}
