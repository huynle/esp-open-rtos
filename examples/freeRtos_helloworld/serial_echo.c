/* Extremely simple example that just reads from stdin and echoes back on stdout
 *
 * Has an easter egg, which is if you type "QUACK" it'll quack 3 times back at you.
 *
 * This example code is in the public domain
 */
#include "espressif/esp_common.h"
#include <esp/uart.h>
#include <string.h>

/* void user_init(void) */
/* { */
/*     uart_set_baud(0, 115200); */
/*     printf("SDK version:%s\n", sdk_system_get_sdk_version()); */
/*     printf("Going into echo mode...\n"); */

/*     while(1) { */
/*         int c = getchar(); */
/*         if(c != EOF) */
/*             putchar(c); */

/*         /1* Easter egg: check for a quack! *1/ */
/*         static int quack; */
/*         if(c == "QUACK"[quack]) { */
/*             quack++; */
/*             if(quack == strlen("QUACK")) { */
/*                 printf("\nQUACK\nQUACK\n"); */
/*                 quack = 0; */
/*             } */
/*         } else { */
/*             quack = 0; */
/*         } */
/*     } */
/* } */
/* Very basic example that just demonstrates we can run at all!
 */
#include "espressif/esp_common.h"
#include "esp/uart.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"

void task1(void *pvParameters)
{
    QueueHandle_t *queue = (QueueHandle_t *)pvParameters;
    printf("Hello from task1!\r\n");
    uint32_t count = 0;
    while(1) {
        vTaskDelay(100);
        xQueueSend(*queue, &count, 0);
        count++;
    }
}

void task2(void *pvParameters)
{
    printf("Hello from task 2!\r\n");
    QueueHandle_t *queue = (QueueHandle_t *)pvParameters;
    while(1) {
        uint32_t count;
        if(xQueueReceive(*queue, &count, 1000)) {
            printf("Got %u\n", count);
        } else {
            printf("No msg :(\n");
        }
    }
}

static QueueHandle_t mainqueue;

void user_init(void)
{
    uart_set_baud(0, 115200);
    printf("SDK version:%s\n", sdk_system_get_sdk_version());
    mainqueue = xQueueCreate(10, sizeof(uint32_t));
    xTaskCreate(task1, "tsk1", 256, &mainqueue, 2, NULL);
    xTaskCreate(task2, "tsk2", 256, &mainqueue, 2, NULL);
}
