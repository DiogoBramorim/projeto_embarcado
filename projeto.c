// <-------------   DESENVOLVIDO POR DIOGO BRUNO ;)-------------------->
// <--------------        DATA 02/2025             -------------------->
// <--------------     PROJETO EMBARCATECH         -------------------->

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "hardware/pwm.h"
#include "hardware/clocks.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "ws2818b.pio.h"
#include "montagem.c"
// #include "jogo.c"

int main(){
  while (true){
    projeto();
  }
}


// Justifiquei alguns topicos para uma melhor compreensão 
// Espero que tenha sido util, volte sempre.
// Obrigado pela visita!! ;)
