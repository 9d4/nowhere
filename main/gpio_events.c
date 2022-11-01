#include "gpio_events.h"

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

#include "driver/gpio.h"

#include "esp_log.h"

#define GPIO_INPUT_BTN_0 GPIO_NUM_4

static const char *TAG = "gpio";
static QueueHandle_t comm0_queue;

static void comm0_isr_handler(void *arg)
{
    uint32_t gpio_num = (uint32_t)arg;
    xQueueSendFromISR(comm0_queue, &gpio_num, NULL);
}

static void comm0_event_task(void *arg)
{
    uint32_t io_num;

    while (1)
    {
        if (xQueueReceive(comm0_queue, &io_num, portMAX_DELAY))
        {
            ESP_LOGI(TAG, "GPIO[%d] intr, val: %d\n", io_num, gpio_get_level(io_num));
        }
    }
}

void comm_gpio()
{
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_ANYEDGE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = GPIO_INPUT_BTN_0;

    gpio_config(&io_conf);
    gpio_set_intr_type(GPIO_INPUT_BTN_0, GPIO_INTR_ANYEDGE);

    // initialize queue
    comm0_queue = xQueueCreate(10, sizeof(uint32_t));
    // start task
    xTaskCreate(comm0_event_task, "comm_event_task", 1024, NULL, 9, NULL);

    // gpio_isr_register(comm0_isr_handler, NULL, 0, NULL);
    gpio_install_isr_service(0);
    gpio_isr_handler_add(GPIO_INPUT_BTN_0, comm0_isr_handler, (void *)GPIO_INPUT_BTN_0);
}