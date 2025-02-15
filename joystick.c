#include "hardware/adc.h"

// Definição dos pinos usados para o joystick e LED
const uint JOYSTICK_X_PIN = 26;       // Pino do eixo X do joystick
const uint JOYSTICK_Y_PIN = 27;       // Pino do eixo Y do joystick
const uint JOYSTICK_BUTTON_PIN = 28;  // Pino do botão do joystick
const int vRx = 26;                   // Pino de leitura do eixo X do joystick (conectado ao ADC)
const int vRy = 27;                   // Pino de leitura do eixo Y do joystick (conectado ao ADC)
const int ADC_CHANNEL_0 = 0;          // Canal ADC para o eixo X do joystick
const int ADC_CHANNEL_1 = 1;          // Canal ADC para o eixo Y do joystick
const int SW = 22;                    // Pino de leitura do botão do joystick
uint16_t valor_x, valor_y;            // Variáveis para armazenar os valores do joystick (eixos X e Y)


// Função para configurar o joystick (pinos de leitura e ADC)
void setup_joystick() {
  // Inicializa o ADC e os pinos de entrada analógica
  adc_init();         // Inicializa o módulo ADC
  adc_gpio_init(vRx); // Configura o pino VRX (eixo X) para entrada ADC
  adc_gpio_init(vRy); // Configura o pino VRY (eixo Y) para entrada ADC

  // Inicializa o pino do botão do joystick
  gpio_init(SW);             // Inicializa o pino do botão
  gpio_set_dir(SW, GPIO_IN); // Configura o pino do botão como entrada
  gpio_pull_up(SW);          // Ativa o pull-up no pino do botão para evitar flutuações
}

// Função para ler os valores dos eixos do joystick (X e Y)
void joystick_read_axis(uint16_t *eixo_x, uint16_t *eixo_y) {
  // Leitura do valor do eixo X do joystick
  adc_select_input(ADC_CHANNEL_0); // Seleciona o canal ADC para o eixo X
  sleep_us(2);                     // Pequeno delay para estabilidade
  *eixo_x = adc_read();            // Lê o valor do eixo X (0-4095)

  // Leitura do valor do eixo Y do joystick
  adc_select_input(ADC_CHANNEL_1); // Seleciona o canal ADC para o eixo Y
  sleep_us(2);                     // Pequeno delay para estabilidade
  *eixo_y = adc_read();            // Lê o valor do eixo Y (0-4095)
}
int ler_joystick_x() {
  adc_select_input(0);  // Seleciona o canal 0 (pino 26) para o eixo X
  return adc_read();    // Lê o valor do ADC do eixo X
}

// Função para ler o eixo Y do joystick
int ler_joystick_y() {
  adc_select_input(1);  // Seleciona o canal 1 (pino 27) para o eixo Y
  return adc_read();    // Lê o valor do ADC do eixo Y
}

// Função para ler o botão do joystick
int ler_botao_joystick() {
  return gpio_get(JOYSTICK_BUTTON_PIN);  // Lê o estado do botão
}

// controla a intensidade do led
void controlar_intensidade_led(int y_value) {
  // Definindo os limites do joystick (0-4095)
  int max_value = 4095;
  int min_value = 0;

  // Controla a intensidade do LED com base no valor do eixo Y
  int intensidade_led = (y_value * 255) / max_value;  // Mapeia o valor para a intensidade de 0 a 255

  // Exemplo de como a intensidade do LED pode ser aplicada na matriz de LEDs
  printf("Intensidade do LED: %d\n", intensidade_led);

  // Aqui você pode controlar a intensidade de cada LED na matriz com base na intensidade calculada
  for (int linha = 0; linha < 5; linha++) {
    for (int coluna = 0; coluna < 5; coluna++) {
      int posicao = getIndex(linha, coluna);
      npSetLED(posicao, intensidade_led, intensidade_led, intensidade_led); // Ajusta a intensidade
    }
  }
  npWrite();  // Atualiza a matriz de LEDs
}

