#include "gpio_actuator.h"
#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/task.h"

#include "gpio_def.h"

static QueueHandle_t buzzer_queue;

static void buzzer_beep_task(void *arg)
{
    uint32_t item = (uint32_t)arg;

    for (;;)
    {
        if (xQueueReceive(buzzer_queue, (void *)&item, (portTickType)portMAX_DELAY))
        {
            gpio_set_level(GPIO_OUTPUT_BUZZER, GPIO_BUZZER_ON);
            vTaskDelay(10 / portTICK_RATE_MS);
            gpio_set_level(GPIO_OUTPUT_BUZZER, GPIO_BUZZER_OFF);
        }
    }
}

void * buzzer_func()
{
    gpio_set_direction(GPIO_OUTPUT_BUZZER, GPIO_MODE_OUTPUT);
    gpio_set_level(GPIO_OUTPUT_BUZZER, GPIO_BUZZER_OFF);

    buzzer_queue = xQueueCreate(5, sizeof(uint32_t));

    return buzzer_beep_task;
}

void buzz()
{
    xQueueSend(&buzzer_queue, (void *)1UL, 0);
    printf("buzzed");
}
