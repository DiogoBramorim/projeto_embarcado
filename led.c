#include <stdio.h>        // Biblioteca padrão de entrada e saída
#include "hardware/adc.h" // Biblioteca para manipulação do ADC no RP2040
#include "pico/stdlib.h"  // Biblioteca padrão do Raspberry Pi Pico
#include "hardware/pwm.h" // Biblioteca para controle PWM

// Definindo variaveis e constantes
const int LED_PIN_A = 12;      // Pino do LED azul
const int LED_PIN_V = 13;      // Pino do LED vermelho
const int LED_PIN_VD = 11;     // Pino do LED verde
const int LED_PIN = 7;
uint slice_num;  // Variável global para armazenar o slice do PWM

// Função para configurar o PWM no LED
void setup_pwm() {
    gpio_set_function(LED_PIN_A, GPIO_FUNC_PWM); // Define o pino como PWM
    slice_num = pwm_gpio_to_slice_num(LED_PIN_A); // Obtém o slice do PWM

    pwm_config config = pwm_get_default_config();  
    pwm_config_set_clkdiv(&config, 4.0f);  // Define a frequência do PWM
    pwm_config_set_wrap(&config, 255);  // Define a resolução do PWM (0-255)

    pwm_init(slice_num, &config, true);  // Inicializa o PWM
}

// Função para ajustar o brilho do LED
void set_led_brightness(uint8_t brightness) {
    pwm_set_gpio_level(LED_PIN_A, brightness);  // Define a intensidade do LED
}
uint8_t ultimo_brilho = 0;  // Armazena o último valor de brilho para evitar atualizações desnecessárias

// Função para controlar o brilho de todos os LEDs da matriz
void update_all_leds_brightness(uint8_t brilho) {
    int matriz[5][5][3] = {
        {{r, g, b}, {200, 0, 0}, {r, g, b}, {200, 0, 0}, {r, g, b}},
        {{200, 0, 0}, {r, g, b}, {200, 0, 0}, {r, g, b}, {200, 0, 0}},
        {{200, 0, 0}, {r, g, b}, {r, g, b}, {r, g, b}, {200, 0, 0}},
        {{r, g, b}, {200, 0, 0}, {r, g, b}, {200, 0, 0}, {r, g, b}},
        {{r, g, b}, {r, g, b}, {200, 0, 0}, {r, g, b}, {r, g, b}}
    };

    // Aplica o brilho a cada LED na matriz
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            // Aplica o brilho de forma proporcional para cada LED na matriz
            uint8_t r_brilho = (matriz[i][j][0] * brilho) / 255;
            uint8_t g_brilho = (matriz[i][j][1] * brilho) / 255;
            uint8_t b_brilho = (matriz[i][j][2] * brilho) / 255;

            // Define o brilho para o LED na posição i, j
            npSetLED(i * 5 + j, r_brilho, g_brilho, b_brilho);  // mapeia os índices 2D para 1D
        }
    }

    npWrite();  // Atualiza todos os LEDs da matriz
}