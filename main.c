#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/timer.h"
#include "hardware/pwm.h"

#define BUZZER_PIN 21
#define NUM_PIXELS 25
#define OUT_PIN 7
#define BUTTON_A 5
#define BUTTON_B 6
#define LED_RED 13  // LED vermelho piscando

// Mapeamento da posição lógica para a posição física (em zig-zag)
int mapa_leds[25] = {
    24, 23, 22, 21, 20,
    15, 16, 17, 18, 19,
    14, 13, 12, 11, 10,
     5,  6,  7,  8,  9,
     4,  3,  2,  1,  0
};

// Simulação de controle de LEDs WS2812 sem Adafruit Neopixel
void set_pixel_color(int pin, uint8_t r, uint8_t g, uint8_t b) {
    uint32_t color = (r << 16) | (g << 8) | b;
    gpio_put(pin, color);
}

// Função para exibir um número na matriz de LEDs
void desenho_pio(double *desenho, double r, double g, double b) {
    float brilho = 0.8;  // Reduz potência para 80%

    for (int i = 0; i < NUM_PIXELS; i++) {
        int posicao_fisica = mapa_leds[i];
        if (desenho[i] == 1) {
            set_pixel_color(OUT_PIN, (uint8_t)(r * 255 * brilho), 
                                      (uint8_t)(g * 255 * brilho), 
                                      (uint8_t)(b * 255 * brilho));
        } else {
            set_pixel_color(OUT_PIN, 0, 0, 0);
        }
    }
}

// Declaração dos números de 0 a 10 e um coração
double numeros[10][25] = {
    {0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0}, // 0

    {0, 0, 1, 0, 0,
     0, 1, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0}, // 1

    {0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0}, // 2

    {0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0}, // 3

    {0, 1, 0, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 0, 0, 1, 0}, // 4

    {0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0}, // 5

    {0, 1, 1, 1, 0,
     0, 1, 0, 0, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0}, // 6

    {0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0,
     0, 0, 1, 0, 0}, // 7

    {0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0}, // 8

    {0, 1, 1, 1, 0,
     0, 1, 0, 1, 0,
     0, 1, 1, 1, 0,
     0, 0, 0, 1, 0,
     0, 1, 1, 1, 0}, // 9
};

// Inicializa os pinos de entrada e saída
void init_gpio() {
    gpio_init(BUTTON_A);
    gpio_set_dir(BUTTON_A, GPIO_IN);
    gpio_pull_up(BUTTON_A);

    gpio_init(BUTTON_B);
    gpio_set_dir(BUTTON_B, GPIO_IN);
    gpio_pull_up(BUTTON_B);

    gpio_init(LED_RED);
    gpio_set_dir(LED_RED, GPIO_OUT);
}

// Função para piscar o LED vermelho 5 vezes por segundo
void piscar_led_vermelho() {
    static uint32_t lastMillis = 0;
    static bool ledState = false;
    
    if (time_us_64() / 1000 - lastMillis >= 100) {  // Pisca 5 vezes por segundo
        lastMillis = time_us_64() / 1000;
        ledState = !ledState;
        gpio_put(LED_RED, ledState);
    }
}

int main() {
    stdio_init_all();
    init_gpio();

    int index = 0;

    while (1) {
        // Verifica os botões para mudar o número exibido
        if (!gpio_get(BUTTON_A)) {
            index = (index + 1) % 10;
            sleep_ms(200);
        }
        if (!gpio_get(BUTTON_B)) {
            index = (index - 1 + 10) % 10;
            sleep_ms(200);
        }

        desenho_pio(numeros[index], 1.0, 0.0, 0.0);  // Atualiza a matriz de LEDs
        piscar_led_vermelho();  // Faz o LED vermelho piscar 5 vezes por segundo
    }
}
