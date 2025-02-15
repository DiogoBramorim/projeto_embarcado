#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "inc/ssd1306.h"
#include "hardware/i2c.h"
#include "buzzer.c"

const uint I2C_SDA_PIN = 14;
const uint I2C_SCL_PIN = 15;

int display()
{
    stdio_init_all();
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

    // Limpa o buffer antes de renderizar
    uint8_t buf[ssd1306_buffer_length];
    memset(buf, 0, ssd1306_buffer_length);
    render_on_display(buf, &frame_area);

    char *text[] = {
        "  Bem-Vindo  ",
        "     ao       ",
        "  PROJETO     ", 
        "   FINAL      "
    };

    int y = 0;
    memset(buf, 0, ssd1306_buffer_length); // Garante que o buffer esteja limpo

    for (uint i = 0; i < count_of(text); i++)
    {
        ssd1306_draw_string(buf, 5, y, text[i]);
        y += 16; // Espaço maior entre as linhas
    }
    

    render_on_display(buf, &frame_area); // Renderiza apenas após desenhar tudo
}