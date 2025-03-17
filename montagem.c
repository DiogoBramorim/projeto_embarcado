// // <------------- DESENVOLVIDO POR DIOGO BRUNO -------------------->
// // <--------------      DATA 02/2025           -------------------->
// // <--------------   PROJETO EMBARCATECH       -------------------->


#include <stdio.h>        // Biblioteca padrão de entrada e saída
#include "hardware/adc.h" // Biblioteca para manipulação do ADC no RP2040
#include "pico/stdlib.h"  // Biblioteca padrão do Raspberry Pi Pico
#include "hardware/pwm.h" // Biblioteca para controle PWM
#include "inc/ssd1306.h"  // Biblioteca para controle do display
#include "matriz.c"       // Armazena informações da matriz
#include "led.c"          // Armazena informações dos leds
#include "joystick.c"     // Armazena informações do joystick
#include "display.c"      // Armazena informações do display
#include "jogo.c"         // Armazena informações da interação joystick e a matriz de led

// Função de configuração geral
void setup() {
  stdio_init_all(); // Inicializa a porta serial para saída de dados
  setup_joystick(); // Chama a função de configuração do joystick
  setup_pwm();      // Chama a função de configuração do PWM
}


// Função principal
int projeto() {
  stdio_init_all();

  // Inicializa o display 
  i2c_init(i2c1, ssd1306_i2c_clock * 1000);
  gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
  gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
  gpio_pull_up(I2C_SDA_PIN);
  gpio_pull_up(I2C_SCL_PIN);
  ssd1306_init();
  

  struct render_area frame_area = {
      start_column : 0,
      end_column : ssd1306_width - 1,
      start_page : 0,
      end_page : ssd1306_n_pages - 1
  };

  calculate_render_area_buffer_length(&frame_area);

  while (true){
        // Inicializa entradas e saídas.
        stdio_init_all();

        // Inicializar o PWM no pino do buzzer
        pwm_init_buzzer(BUZZER_PIN_A);
    
        // Inicializa matriz de LEDs NeoPixel.
        npInit(LED_PIN);
        npClear();
        // Inicializa os botões
        gpio_get(BUTTON_A);
        gpio_set_dir(BUTTON_A, GPIO_IN);
        gpio_pull_up(BUTTON_A);
      
        gpio_get(BUTTON_B);
        gpio_set_dir(BUTTON_B, GPIO_IN);
        gpio_pull_up(BUTTON_B);
        gpio_set_dir(LED_PIN_A, GPIO_OUT);
        npWrite(); // Escreve os dados nos LEDs.

        setup();                 // Chama a função de configuração
        printf("Joystick e LED\n"); // Exibe uma mensagem inicial via porta serial
        uint8_t buf[ssd1306_buffer_length];
          memset(buf, 0, ssd1306_buffer_length); // limpa o display
          render_on_display(buf, &frame_area); // Renderiza
          int y = 0;

        memset(buf, 0, ssd1306_buffer_length); // limpa o display
        char *text[] = { //texto do display
            "  Bem-Vindo  ",
            "  Jogo A  ",
            " Intensidade B"
        };
        for (uint i = 0; i < count_of(text); i++)
          {
              ssd1306_draw_string(buf, 5, y, text[i]);
              y += 16; // Espaço maior entre as linhas
          }
        render_on_display(buf, &frame_area); // Renderiza
    // CONDIÇÃO PARA ACESAR A INTERAÇÃO DO JOYSTICK COM O LED
  while (true)
  {
  
    if(gpio_get(BUTTON_A)==0){
      memset(buf, 0, ssd1306_buffer_length);
          render_on_display(buf, &frame_area);
            pontuacao = 0;
            printf("Pontuação: %d\n", pontuacao);
          
            while (1) {
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
              memset(buf, 0, ssd1306_buffer_length);
              render_on_display(buf, &frame_area);
          
              // Atualiza a matriz de LEDs
              npWrite_jogo();
          
              // Lê o joystick
              joystick_read_axis_jogo(&eixo_y, &eixo_x);
          
              // Calcula a nova posição
              int novo_x = cursor_x, novo_y = cursor_y;
          
              if (eixo_x > 3000 && cursor_x < 4) novo_x++; // Esquerda
              if (eixo_x < 1000 && cursor_x > 0) novo_x--; // Direita
              if (eixo_y < 1000 && cursor_y > 0) novo_y++; // Baixo
              if (eixo_y > 3000 && cursor_y < 4) novo_y--; // Cima
          
              // Se colidir com obstáculo, encerra o jogo
              if (obstaculos[novo_y][novo_x] == 1) {
                beep(BUZZER_PIN_A, 600);
                beep(BUZZER_PIN_B,600);
                char pontuacao_str[10];  // Buffer para armazenar o valor da pontuação como string
                sprintf(pontuacao_str, "  FINAL: %d", pontuacao); // Converte a pontuação para string
                memset(buf, 0, ssd1306_buffer_length);
                render_on_display(buf, &frame_area);
          
            char *text[] = {
                " GAME OVER!  ",
                "  POTUAcaO   ",
                 pontuacao_str
            };
          
            int y = 0;
            memset(buf, 0, ssd1306_buffer_length); // Garante que o buffer esteja limpo
          
            for (uint i = 0; i < count_of(text); i++)
            {
                ssd1306_draw_string(buf, 5, y, text[i]);
                y += 16; // Espaço maior entre as linhas
            }
            
            render_on_display(buf, &frame_area); // Renderiza apenas após desenhar tudo
                printf("Game Over! Pontuação final: %d\n", pontuacao);
                npClear_jogo();
                npWrite_jogo();
                while (1); // Fica travado no Game Over
              }
          
              // Se pegar um ponto, aumenta a pontuação e remove o ponto do tabuleiro
              if (pontos[novo_y][novo_x] == 1) {
                beep(BUZZER_PIN_A, 100);
                pontuacao += 10;
                pontos[novo_y][novo_x] = 0; // Remove o ponto coletado
                printf("Pontuação: %d\n", pontuacao);
              }
          
              // Verifica se todos os pontos foram coletados
              int pontos_restantes = 0;
              for (int y = 0; y < 5; y++) {
                for (int x = 0; x < 5; x++) {
                  if (pontos[y][x] == 1) {
                    pontos_restantes++;
                  }
                }
              }
          
              if (pontos_restantes == 0) {
                npClear_jogo();
                beep(BUZZER_PIN_A,100);
                beep(BUZZER_PIN_B,100);
                memset(buf, 0, ssd1306_buffer_length);
                render_on_display(buf, &frame_area);
        
          char *text[] = {
              "  PARABENSS  ",
              "    VOCE    ",
              "   VENCEU      ", 
          };
        
          int y = 0;
          memset(buf, 0, ssd1306_buffer_length); // Garante que o buffer esteja limpo
        
          for (uint i = 0; i < count_of(text); i++)
          {
              ssd1306_draw_string(buf, 5, y, text[i]);
              y += 16; // Espaço maior entre as linhas
          }
          
          render_on_display(buf, &frame_area); // Renderiza apenas após desenhar tudo
                beep(BUZZER_PIN_A,100);
                beep(BUZZER_PIN_B,100);
                beep(BUZZER_PIN_A,100);
                printf("Todos os pontos coletados! Fogos de artifício!\n");
                for (int camada = 0; camada <= 2; camada++) {
                  npClear_jogo();
                  for (int y = 2 - camada; y <= 2 + camada; y++) {
                    for (int x = 2 - camada; x <= 2 + camada; x++) {
                      if (x >= 0 && x < 5 && y >= 0 && y < 5) {
                        npSetLED_jogo(getIndex_jogo(x, y), rand() % 255, rand() %1, rand() %1);
                      }
                    }
                  }
                  npWrite_jogo();
                  sleep_ms(500);
                }
                npClear_jogo();
                npWrite_jogo();
                sleep_ms(apagamento);
              }
          
              // Atualiza posição do cursor
              cursor_x = novo_x;
              cursor_y = novo_y;
          
              // Pequeno delay para suavizar a movimentação
              sleep_ms(100);
          }
        }
        if(gpio_get(BUTTON_B)==0){
          uint16_t eixo_y;
          joystick_read_axis(&eixo_y, NULL);  // Lê o valor do eixo Y do joystick
      
          // Mapeia o valor do eixo Y (0-4095) para a intensidade do LED (0-255)
          uint8_t brilho = (eixo_y * 255) / 4095;
      
          // Evita atualizar a matriz se o brilho não mudou
          if (brilho != ultimo_brilho) {
              update_all_leds_brightness(brilho);  // Atualiza o brilho de todos os LEDs
              ultimo_brilho = brilho;  // Armazena o novo brilho
          }
      
          // Exibe o valor do brilho no terminal para depuração
          printf("Brilho de todos os LEDs da matriz: %d\n", brilho);
      
          sleep_ms(100);
    }
    }
  }
}









// <------------- DESENVOLVIDO POR DIOGO BRUNO -------------------->
// <--------------      DATA 02/2025           -------------------->
// <--------------   PROJETO EMBARCATECH       -------------------->

  // #include <stdio.h>        // Biblioteca padrão de entrada e saída
// #include "hardware/adc.h" // Biblioteca para manipulação do ADC no RP2040
// #include "pico/stdlib.h"  // Biblioteca padrão do Raspberry Pi Pico
// #include "hardware/pwm.h" // Biblioteca para controle PWM
// #include "inc/ssd1306.h"  // Biblioteca para controle do display
// #include "matriz.c"       // Armazena informações da matriz
// #include "led.c"          // Armazena informações dos leds
// #include "joystick.c"     // Armazena informações do joystick
// #include "display.c"      // Armazena informações do display
// #include "jogo.c"         // Armazena informações da interação joystick e a matriz de led

// // Função de configuração geral
// void setup() {
//   stdio_init_all(); // Inicializa a porta serial para saída de dados
//   setup_joystick(); // Chama a função de configuração do joystick
//   setup_pwm();      // Chama a função de configuração do PWM
// }

// // CONTROLE DO LED
// int controle_led(){
//   while (1) {
//     joystick_read_axis(&valor_x, &valor_y); // Lê os valores dos eixos do joystick

//     // Mapeia o valor do eixo X para o brilho do LED (0-255)
//     uint8_t brilho = (uint8_t)((valor_x * 255) / 4095);
//     set_led_brightness(brilho); // Ajusta o brilho do LED

//     // Exibe os valores do joystick e o brilho do LED
//     printf("X: %d, Y: %d, Brilho: %d\n", valor_x, valor_y, brilho);

//     // Verifica se o botão do joystick foi pressionado
//     if (gpio_get(SW) == 0) {
//       printf("Botao pressionado!\n");
//       set_led_brightness(0); // Apaga o LED ao pressionar o botão
//     }

//     // Pequeno delay antes da próxima leitura
//     sleep_ms(100); // Espera 100 ms antes de repetir o ciclo
//   }
// }



// // Função principal
// int projeto() {
//   stdio_init_all();

//   // Inicializa o display 
//   i2c_init(i2c1, ssd1306_i2c_clock * 1000);
//   gpio_set_function(I2C_SDA_PIN, GPIO_FUNC_I2C);
//   gpio_set_function(I2C_SCL_PIN, GPIO_FUNC_I2C);
//   gpio_pull_up(I2C_SDA_PIN);
//   gpio_pull_up(I2C_SCL_PIN);
//   ssd1306_init();
  

//   struct render_area frame_area = {
//       start_column : 0,
//       end_column : ssd1306_width - 1,
//       start_page : 0,
//       end_page : ssd1306_n_pages - 1
//   };

//   calculate_render_area_buffer_length(&frame_area);

//   while (true){
//         // Inicializa entradas e saídas.
//         stdio_init_all();

//         // Inicializar o PWM no pino do buzzer
//         pwm_init_buzzer(BUZZER_PIN_A);
    
//         // Inicializa matriz de LEDs NeoPixel.
//         npInit(LED_PIN);
//         npClear();
//         // Inicializa os botões
//         gpio_get(BUTTON_A);
//         gpio_set_dir(BUTTON_A, GPIO_IN);
//         gpio_pull_up(BUTTON_A);
      
//         gpio_get(BUTTON_B);
//         gpio_set_dir(BUTTON_B, GPIO_IN);
//         gpio_pull_up(BUTTON_B);
      
//         npWrite(); // Escreve os dados nos LEDs.

//         setup();                 // Chama a função de configuração
//         printf("Joystick e LED\n"); // Exibe uma mensagem inicial via porta serial
        
      
//         while(true) {

//           // configuração display
//           uint8_t buf[ssd1306_buffer_length];
//           memset(buf, 0, ssd1306_buffer_length); // limpa o display
//           render_on_display(buf, &frame_area); // Renderiza
        
//           char *text[] = { //texto do display
//               "  Bem-Vindo  ",
//               "  Aperte A    ",
//               "    OU        ", 
//               "  APERTE B    "
//           };
        
//           int y = 0;
//           memset(buf, 0, ssd1306_buffer_length); // Garante que o buffer esteja limpo
        
//           for (uint i = 0; i < count_of(text); i++)
//           {
//               ssd1306_draw_string(buf, 5, y, text[i]);
//               y += 16; // Espaço maior entre as linhas
//           }
          
//           render_on_display(buf, &frame_area); // Renderiza apenas após desenhar tudo
//           if(gpio_get(BUTTON_A)==0){
//             uint8_t buf[ssd1306_buffer_length];
//             memset(buf, 0, ssd1306_buffer_length);
//             render_on_display(buf, &frame_area);


//           // --------- CONTAGEM DE 3 a 1 ---------
//           beep(BUZZER_PIN_A,200); // O BUZZER A FAZ UM BEEP

//          int matriz_3[5][5][3]={ // DESENHA O NUMERO 3
//           {{r, g, b}, {200, g, b}, {200, g, b}, {200, g, b}, {200, g, b}}, 
//           {{r, g, b}, {r, g, b}, {r, g, b}, {r, g, b}, {200, g, b}},
//           {{r, g, b}, {r, g, b}, {200, g, b}, {200, g, b}, {200, g, b}}, 
//           {{r, g, b}, {r, g, b}, {r, g, b}, {r, g, b}, {200, g, b}},    
//           {{r, g, b}, {200, g, b}, {200, g, b}, {200, g, b}, {200, g, b}}
//         };
//           for(int linha = 0; linha < 5; linha++){
//             for(int coluna = 0; coluna < 5; coluna++){
//               int posicao = getIndex(linha, coluna);
//               npSetLED(posicao, matriz_3[coluna][linha][0], matriz_3[coluna][linha][1], matriz_3[coluna][linha][2]);
//             }
//           }
//         npWrite();
//         sleep_ms(t);
//         npClear();
//         npWrite();
//         sleep_ms(t);
//         beep(BUZZER_PIN_A,200);
//         // 2
//           int matriz_2[5][5][3]={ // DESENHA O NUMERO 2
//               {{r, g, b}, {200, g, b}, {200, g, b}, {200, g, b}, {200, g, b}}, 
//               {{r, g, b}, {r, g, b}, {r, g, b}, {r, g, b}, {200, g, b}},
//               {{r, g, b}, {200, g, b}, {200, g, b}, {200, g, b}, {200, g, b}}, 
//               {{r, g, b}, {200, g, b}, {r, g, b}, {r, g, b}, {r, g, b}},    
//               {{r, g, b}, {200, g, b}, {200, g, b}, {200, g, b}, {200, g, b}}
//             };
//               for(int linha = 0; linha < 5; linha++){
//                 for(int coluna = 0; coluna < 5; coluna++){
//                   int posicao = getIndex(linha, coluna);
//                   npSetLED(posicao, matriz_2[coluna][linha][0], matriz_2[coluna][linha][1], matriz_2[coluna][linha][2]);
//                 }
//               }
//             npWrite();
//             sleep_ms(t);
//             npClear();
//             npWrite();
//             sleep_ms(t);
//             beep(BUZZER_PIN_A,200);
//             //1
//             int matriz_1[5][5][3]={  // DESENHA O NUMERO 1
//               {{r, g, b}, {r, g, b}, {200, g, b}, {r, g, b}, {r, g, b}}, 
//               {{r, g, b}, {200, g, b}, {200, g, b}, {r, g, b}, {r, g, b}},
//               {{r, g, b}, {r, g, b}, {200, g, b}, {r, g, b}, {r, g, b}}, 
//               {{r, g, b}, {r, g, b}, {200, g, b}, {r, g, b}, {r, g, b}},    
//               {{r, g, b}, {200, g, b}, {200, g, b}, {200, g, b}, {r, g, b}}
//             };
//               for(int linha = 0; linha < 5; linha++){
//                 for(int coluna = 0; coluna < 5; coluna++){
//                   int posicao = getIndex(linha, coluna);
//                   npSetLED(posicao, matriz_1[coluna][linha][0], matriz_1[coluna][linha][1], matriz_1[coluna][linha][2]);
//                 }
//               }
//             npWrite();
//             sleep_ms(t);
//             npClear();
//             npWrite();
//             sleep_ms(t);
//             //--------- FIM DA CONTAGEM --------


//             beep(BUZZER_PIN_A,200);
//             beep(BUZZER_PIN_B,500);


      
//         //--------- LED ACENDENDO COLUNA POR COLUNA -----------
//         for (int i = 0; i < 5; i++) {  
//               for (int j = 0; j < 5; j++) {
//                 int posicao = getIndex(i, j);
//                 npSetLED(posicao, 200, 0, 0); // Define a cor vermelha
//               }
//               npWrite();
//               sleep_ms(t); // Espera 1 segundo antes de acender o próximo conjunto
//           }
//             npClear();
//             npWrite();
//             sleep_ms(apagamento);
//           // ---------- FIM DA CONTAGEM ---------
              
          
      
//           // ------------- DESENHO DO CORAÇÂO ------------------
//             int matriz[5][5][3]={
//               {{r, g, b}, {200, 0, 0}, {r, g, b}, {200, 0, 0}, {r, g, b}}, 
//               {{200, 0, 0}, {r, g, b}, {200, 0, 0}, {r, g, b}, {200, 0, 0}},
//               {{200, 0, 0}, {r, g, b}, {r, g, b}, {r, g, b}, {200, 0, 0}}, 
//               {{r, g, b}, {200, 0, 0}, {r, g, b}, {200, 0, 0}, {r, g, b}},    
//               {{r, g, b}, {r, g, b}, {200, 0, 0}, {r, g, b}, {r, g, b}}
//             };
//             for (int linha = 0; linha < 5; linha++) {
//               for (int coluna = 0; coluna < 5; coluna++) {
//                   int posicao = getIndex(linha, coluna);
//                   npSetLED(posicao, matriz[coluna][linha][0], matriz[coluna][linha][1], matriz[coluna][linha][2]);
//               }
//           }
//           npWrite();
//           sleep_ms(t);
//           npClear();
//           npWrite();
//           sleep_ms(t);

          
//           // ------------- JOGO DE LED: DO CENTRO PARA AS BORDAS ------------------
//           for (int camada = 0; camada <= 2; camada++) {
//               for (int y = 2 - camada; y <= 2 + camada; y++) {
//                   for (int x = 2 - camada; x <= 2 + camada; x++) {
//                       if (x >= 0 && x < 5 && y >= 0 && y < 5) {
//                           npSetLED(getIndex(x, y), rand() % 256, rand() % 256, rand() % 256);
//                       }
//                   }
//               }
//               npWrite();
//               sleep_ms(200);
//           }
          
//           sleep_ms(500);
//           npClear();
//           npWrite();
//           sleep_ms(apagamento);
//         }
        

    
//     // CONDIÇÃO PARA ACESAR A INTERAÇÃO DO JOYSTICK COM O LED
//     if(gpio_get(BUTTON_B)==0){
//       memset(buf, 0, ssd1306_buffer_length);
//           render_on_display(buf, &frame_area);
//           while (true)
//           {
//             pontuacao = 0;
//             printf("Pontuação: %d\n", pontuacao);
          
//             while (1) {
//               // Limpa a matriz de LEDs
//               npClear_jogo();
          
//               // Desenha os obstáculos (LED vermelho)
//               for (int y = 0; y < 5; y++) {
//                 for (int x = 0; x < 5; x++) {
//                   if (obstaculos[y][x] == 1) {
//                     npSetLED_jogo(getIndex_jogo(x, y), 255, 0, 0); // LED vermelho para obstáculos
//                   } else if (pontos[y][x] == 1) {
//                     npSetLED_jogo(getIndex_jogo(x, y), 0, 255, 0); // LED verde para pontos
//                   }
//                 }
//               }
          
//               // Desenha o cursor (LED azul)
//               npSetLED_jogo(getIndex_jogo(cursor_x, cursor_y), 0, 0, 255);
//               memset(buf, 0, ssd1306_buffer_length);
//               render_on_display(buf, &frame_area);
          
//               // Atualiza a matriz de LEDs
//               npWrite_jogo();
          
//               // Lê o joystick
//               joystick_read_axis_jogo(&eixo_y, &eixo_x);
          
//               // Calcula a nova posição
//               int novo_x = cursor_x, novo_y = cursor_y;
          
//               if (eixo_x > 3000 && cursor_x < 4) novo_x++; // Esquerda
//               if (eixo_x < 1000 && cursor_x > 0) novo_x--; // Direita
//               if (eixo_y < 1000 && cursor_y > 0) novo_y++; // Baixo
//               if (eixo_y > 3000 && cursor_y < 4) novo_y--; // Cima
          
//               // Se colidir com obstáculo, encerra o jogo
//               if (obstaculos[novo_y][novo_x] == 1) {
//                 beep(BUZZER_PIN_A, 600);
//                 beep(BUZZER_PIN_B,600);
//                 char pontuacao_str[10];  // Buffer para armazenar o valor da pontuação como string
//                 sprintf(pontuacao_str, "  FINAL: %d", pontuacao); // Converte a pontuação para string
//                 memset(buf, 0, ssd1306_buffer_length);
//                 render_on_display(buf, &frame_area);
          
//             char *text[] = {
//                 " GAME OVER!  ",
//                 "  POTUAcaO   ",
//                  pontuacao_str
//             };
          
//             int y = 0;
//             memset(buf, 0, ssd1306_buffer_length); // Garante que o buffer esteja limpo
          
//             for (uint i = 0; i < count_of(text); i++)
//             {
//                 ssd1306_draw_string(buf, 5, y, text[i]);
//                 y += 16; // Espaço maior entre as linhas
//             }
            
//             render_on_display(buf, &frame_area); // Renderiza apenas após desenhar tudo
//                 printf("Game Over! Pontuação final: %d\n", pontuacao);
//                 npClear_jogo();
//                 npWrite_jogo();
//                 while (1); // Fica travado no Game Over
//               }
          
//               // Se pegar um ponto, aumenta a pontuação e remove o ponto do tabuleiro
//               if (pontos[novo_y][novo_x] == 1) {
//                 beep(BUZZER_PIN_A, 100);
//                 pontuacao += 10;
//                 pontos[novo_y][novo_x] = 0; // Remove o ponto coletado
//                 printf("Pontuação: %d\n", pontuacao);
//               }
          
//               // Verifica se todos os pontos foram coletados
//               int pontos_restantes = 0;
//               for (int y = 0; y < 5; y++) {
//                 for (int x = 0; x < 5; x++) {
//                   if (pontos[y][x] == 1) {
//                     pontos_restantes++;
//                   }
//                 }
//               }
          
//               if (pontos_restantes == 0) {
//                 npClear_jogo();
//                 beep(BUZZER_PIN_A,100);
//                 beep(BUZZER_PIN_B,100);
//                 memset(buf, 0, ssd1306_buffer_length);
//                 render_on_display(buf, &frame_area);
        
//           char *text[] = {
//               "  PARABENSS  ",
//               "    VOCE    ",
//               "   VENCEU      ", 
//           };
        
//           int y = 0;
//           memset(buf, 0, ssd1306_buffer_length); // Garante que o buffer esteja limpo
        
//           for (uint i = 0; i < count_of(text); i++)
//           {
//               ssd1306_draw_string(buf, 5, y, text[i]);
//               y += 16; // Espaço maior entre as linhas
//           }
          
//           render_on_display(buf, &frame_area); // Renderiza apenas após desenhar tudo
//                 beep(BUZZER_PIN_A,100);
//                 beep(BUZZER_PIN_B,100);
//                 beep(BUZZER_PIN_A,100);
//                 printf("Todos os pontos coletados! Fogos de artifício!\n");
//                 for (int camada = 0; camada <= 2; camada++) {
//                   npClear_jogo();
//                   for (int y = 2 - camada; y <= 2 + camada; y++) {
//                     for (int x = 2 - camada; x <= 2 + camada; x++) {
//                       if (x >= 0 && x < 5 && y >= 0 && y < 5) {
//                         npSetLED_jogo(getIndex_jogo(x, y), rand() % 255, rand() %1, rand() %1);
//                       }
//                     }
//                   }
//                   npWrite_jogo();
//                   sleep_ms(500);
//                 }
//                 sleep_ms(500);
//                 npClear_jogo();
//                 npWrite_jogo();
//                 sleep_ms(apagamento);
//               }
          
//               // Atualiza posição do cursor
//               cursor_x = novo_x;
//               cursor_y = novo_y;
          
//               // Pequeno delay para suavizar a movimentação
//               sleep_ms(100);
//             }
//           }
//         }
//       }
//     }
//   }