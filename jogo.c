#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/pio.h"
#include "hardware/clocks.h"
#include "ws2818b.pio.h"

// Definições
#define LED_COUNT 25    // referente a matriz de led
#define LED_PIN 7
#define BUTTON_A 5      // Pin botão A
#define BUTTON_B 6      // Pin botão B
#define JOYSTICK_X_PIN 26
#define JOYSTICK_Y_PIN 27

int eixo_x, eixo_y;
int cursor_x = 2, cursor_y = 2; // Posição inicial do cursor (centro da matriz)

// Estrutura para o pixel
struct pixel_tp {
  uint8_t G, R, B;
};
typedef struct pixel_tp pixel_tp;
typedef pixel_tp npLED_tp;

// Variáveis globais
npLED_tp leds_jogo[LED_COUNT];
PIO np_pio;
uint sm;

// Função para inicializar a matriz de LEDs
void npInit_jogo(uint pin) {
  uint offset = pio_add_program(pio0, &ws2818b_program);
  np_pio = pio0;
  sm = pio_claim_unused_sm(np_pio, false);
  if (sm < 0) {
    np_pio = pio1;
    sm = pio_claim_unused_sm(np_pio, true);
  }
  ws2818b_program_init(np_pio, sm, offset, pin, 800000.f);
  for (uint i = 0; i < LED_COUNT; ++i) {
    leds_jogo[i].R = 0;
    leds_jogo[i].G = 0;
    leds_jogo[i].B = 0;
  }
}

// Função para definir a cor de um LED
void npSetLED_jogo(const uint index, const uint8_t r, const uint8_t g, const uint8_t b) {
  leds_jogo[index].R = r;
  leds_jogo[index].G = g;
  leds_jogo[index].B = b;
}

// Função para limpar a matriz de LEDs
void npClear_jogo() {
  for (uint i = 0; i < LED_COUNT; ++i)
    npSetLED_jogo(i, 0, 0, 0);
}

// Função para escrever os dados na matriz de LEDs
void npWrite_jogo() {
  for (uint i = 0; i < LED_COUNT; ++i) {
    pio_sm_put_blocking(np_pio, sm, leds_jogo[i].G);
    pio_sm_put_blocking(np_pio, sm, leds_jogo[i].R);
    pio_sm_put_blocking(np_pio, sm, leds_jogo[i].B);
  }
  sleep_us(100);
}

// Função para converter coordenadas (x, y) em índice linear
int getIndex_jogo(int x, int y) {
  if (y % 2 == 0) {
    return 24 - (y * 5 + x);
  } else {
    return 24 - (y * 5 + (4 - x));
  }
}

// Função para configurar o joystick
void setup_joystick_jg() {
  adc_init();
  adc_gpio_init(JOYSTICK_X_PIN); // Eixo X (GPIO 26)
  adc_gpio_init(JOYSTICK_Y_PIN); // Eixo Y (GPIO 27)
  gpio_init(BUTTON_A);
  gpio_set_dir(BUTTON_A, GPIO_IN);
  gpio_pull_up(BUTTON_A);
}

// Função para ler os valores do joystick
void joystick_read_axis_jogo(int *eixo_x, int *eixo_y) {
  adc_select_input(0); // Eixo X
  *eixo_x = adc_read();
  adc_select_input(1); // Eixo Y
  *eixo_y = adc_read();
}
int obstaculos[5][5] = {
  {1, 1, 1, 1, 1},
  {0, 1, 0, 1, 0},
  {0, 0, 0, 0, 0},
  {0, 1, 0, 1, 0},
  {1, 1, 1, 1, 1}
};
// Função principal
// Variável de pontuação


// Variáveis globais
int pontuacao = 0;
int pontos[5][5] = {
  {0, 0, 0, 0, 0},
  {0, 0, 0, 0, 1},
  {1, 0, 0, 0, 0},
  {0, 0, 1, 0, 0},
  {0, 0, 0, 0, 0}
};

// Função principal: ela foi colocada no arquivo montagem com algumas alterações
// Aqui esta presente apenas o exemplo 
int led_joy() {
  stdio_init_all();
  npInit_jogo(LED_PIN);
  setup_joystick_jg();

  pontuacao = 0;
  printf("Pontuação: %d\n", pontuacao);

  while (true) {
    // Limpa a matriz de LEDs
    npClear_jogo();

    // Desenha os obstáculos (LED vermelho)
    for (int y = 0; y < 5; y++) {
      for (int x = 0; x < 5; x++) {
        if (obstaculos[y][x] == 1) {
          npSetLED_jogo(getIndex_jogo(x, y), 255, 0, 0); // LED vermelho para obstáculos
        } else if (pontos[y][x] == 1) {
          npSetLED_jogo(getIndex_jogo(x, y), 0, 255, 0); // LED verde para pontos
        }
      }
    }

    // Desenha o cursor (LED azul)
    npSetLED_jogo(getIndex_jogo(cursor_x, cursor_y), 0, 0, 255);

    // Atualiza a matriz de LEDs
    npWrite_jogo();

    // Lê o joystick
    joystick_read_axis_jogo(&eixo_y, &eixo_x);

    // Calcula a nova posição
    int novo_x = cursor_x, novo_y = cursor_y;

    if (eixo_x > 3000 && cursor_x > 0) novo_x--; // Esquerda
    if (eixo_x < 1000 && cursor_x < 4) novo_x++; // Direita
    if (eixo_y < 1000 && cursor_y > 0) novo_y++; // Baixo
    if (eixo_y > 3000 && cursor_y < 4) novo_y--; // Cima

    // Se colidir com obstáculo, encerra o jogo
    if (obstaculos[novo_y][novo_x] == 1) {
      printf("Game Over! Pontuação final: %d\n", pontuacao);
      npClear_jogo();
      npWrite_jogo();
      while (true); // Fica travado no Game Over
    }

    // Se pegar um ponto, aumenta a pontuação e remove o ponto do tabuleiro
    if (pontos[novo_y][novo_x] == 1) {
      pontuacao += 10;
      pontos[novo_y][novo_x] = 0; // Remove o ponto coletado
      printf("Pontuação: %d\n", pontuacao);
    }

    // Atualiza posição do cursor
    cursor_x = novo_x;
    cursor_y = novo_y;

    // Pequeno delay para suavizar a movimentação
    sleep_ms(150);
  }
}
