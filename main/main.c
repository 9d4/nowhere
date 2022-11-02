#include "stdio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#include "driver/uart.h"

#include "esp_log.h"

static const char *TAG = "main";

void app_main()
{
    uart_set_baudrate(UART_NUM_0, 115200);

    ESP_LOGI(TAG, "Starting system...\n");
}
