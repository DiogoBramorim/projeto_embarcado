#include <stdio.h>        // Biblioteca padrão de entrada e saída
#include "hardware/adc.h" // Biblioteca para manipulação do ADC no RP2040
#include "pico/stdlib.h"  // Biblioteca padrão do Raspberry Pi Pico
#include "hardware/pwm.h" // Biblioteca para controle PWM

// Definindo variaveis e constantes
const int LED_PIN_A = 12;      // Pino do LED azul
const int LED_PIN_V = 13;      // Pino do LED vermelho
const int LED_PIN_VD = 11;     // Pino do LED verde

// Função para configurar o PWM no pino do LED
void setup_pwm() {
    gpio_set_function(LED_PIN_A, GPIO_FUNC_PWM); // Configura o pino do LED para função PWM
    uint slice_num = pwm_gpio_to_slice_num(LED_PIN_A); // Obtém o número do slice PWM
    pwm_set_wrap(slice_num, 255); // Define o valor máximo do PWM (8 bits)
    pwm_set_enabled(slice_num, true); // Habilita o slice PWM
  }

// Função para ajustar o brilho do LED usando PWM
void set_led_brightness(uint8_t brightness) {
  pwm_set_gpio_level(LED_PIN_A, brightness); // Define o nível de brilho do LED
}
