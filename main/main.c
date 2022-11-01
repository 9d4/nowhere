#include "stdio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "uart_event.h"

static const char *TAG = "main";

void app_main()
{
    printf("Staring system...\n");

    xTaskCreate(uart_task_func(), "uart_event_task", 1024, NULL, 10, NULL);
}