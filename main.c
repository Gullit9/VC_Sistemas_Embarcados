#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "spi_flash_mmap.h"
#include "esp_heap_caps.h"
#include "driver/gpio.h"
#include "rom/gpio.h"

#define BT_PIN 21
#define LED_PIN_RED 25
#define LED_PIN_YELLOW 26
#define SENSOR_OP 27
#define SENSOR_CL 14

void app_main()
{
    esp_rom_gpio_pad_select_gpio(LED_PIN_RED);
    gpio_set_direction(LED_PIN_RED, GPIO_MODE_OUTPUT);

    esp_rom_gpio_pad_select_gpio(LED_PIN_YELLOW);
    gpio_set_direction(LED_PIN_YELLOW, GPIO_MODE_OUTPUT);

    gpio_pad_select_gpio(BT_PIN);
    gpio_set_direction(BT_PIN, GPIO_MODE_INPUT);

    gpio_pad_select_gpio(SENSOR_OP);
    gpio_set_direction(SENSOR_OP, GPIO_MODE_INPUT);

    gpio_pad_select_gpio(SENSOR_CL);
    gpio_set_direction(SENSOR_CL, GPIO_MODE_INPUT);

    bool status_b = 0;
    bool button = 0;
    bool last_button = 0;
    bool sensor_op = 1;
    bool sensor_cl = 1;

    while(1)
    {
        button = gpio_get_level(BT_PIN);
        sensor_op = gpio_get_level(SENSOR_OP);
        sensor_cl = gpio_get_level(SENSOR_CL);

        if (button && !last_button)
        {
            status_b = !status_b;
            if (status_b == 1)
            {
                gpio_set_level(LED_PIN_YELLOW, status_b);
                gpio_set_level(LED_PIN_RED, !status_b);
                //printf("Abrindo o portao...\n");
            }  
            if (status_b == 0)
            {
                gpio_set_level(LED_PIN_YELLOW, status_b);
                gpio_set_level(LED_PIN_RED, !status_b);
                //printf("Fechando o portao...\n");
            }
        }
        if ((status_b == 1) && (sensor_op == 0))
        {
            gpio_set_level(LED_PIN_YELLOW, status_b);
            gpio_set_level(LED_PIN_RED, status_b);
            //printf("Portão Aberto\n");
        }
        if ((status_b == 0) && (sensor_cl == 0))
        {
            gpio_set_level(LED_PIN_YELLOW, status_b);
            gpio_set_level(LED_PIN_RED, status_b);
            //printf("Portão Fechado\n");
        }
        last_button = button;
        vTaskDelay(10 / portTICK_PERIOD_MS);
    }
}
