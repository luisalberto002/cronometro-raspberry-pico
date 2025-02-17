#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"

#define BTN_START_STOP 5
#define BTN_RESET 6
#define LED_PIN 12

volatile uint32_t segundos = 0;
volatile bool rodando = false;

bool timer_callback(struct repeating_timer *t) {
    if (rodando) {
        segundos++;
        printf("Tempo: %d segundos\n", segundos);
    }
    return true;
}

int main() {
    stdio_init_all();
    gpio_init(BTN_START_STOP);
    gpio_set_dir(BTN_START_STOP, GPIO_IN);
    gpio_pull_up(BTN_START_STOP);

    gpio_init(BTN_RESET);
    gpio_set_dir(BTN_RESET, GPIO_IN);
    gpio_pull_up(BTN_RESET);

    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    struct repeating_timer timer;
    add_repeating_timer_ms(1000, timer_callback, NULL, &timer);

    printf("Cronômetro Iniciado!\n");

    while (1) {
        if (gpio_get(BTN_START_STOP) == 0) {
            sleep_ms(200);
            rodando = !rodando;
            gpio_put(LED_PIN, rodando);
        }

        if (gpio_get(BTN_RESET) == 0) {
            sleep_ms(200);
            segundos = 0;
            printf("Cronômetro resetado!\n");
        }
    }
}
