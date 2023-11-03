
#include "main.h"

int voltage;
extern uint8_t id;
extern uint16_t x_result, y_result, z_result;

TaskHandle_t blink_led1_task;
TaskHandle_t blink_led2_task;
TaskHandle_t print_log_task;
TaskHandle_t get_voltage_task;
TaskHandle_t get_gyro_value_task;

QueueHandle_t voltage_queue;
QueueHandle_t x_result_queue;
QueueHandle_t y_result_queue;
QueueHandle_t z_result_queue;

void SystemInit(void)
{

}

void vApplicationMallocFailedHook()
{

}

void vApplicationTickHook()
{

}

void vApplicationStackOverflowHook(TaskHandle_t xTask, char * pcTaskName)
{

}

void vApplicationIdleHook()
{

}

void led1_blink_func()
{
    for( ;; )
    {
        LED_toggle(LED_BLUE);
        vTaskDelay(1000);
    }
}

void led2_blink_func()
{
    for( ;; )
    {
        LED_toggle(LED_GREEN);
        vTaskDelay(1500);
    }
}

void print_log(void *param)
{
    for (;;)
    {
#if (PROGRAM_SELECT == VOLTAGE_TEMP)
        int voltage_temp = 0;
        xQueueReceive(voltage_queue, &voltage_temp, portMAX_DELAY);
        custom_printf("Voltage level from queue: %d\r\n", voltage_temp);
        vTaskDelay(3000);
#elif (PROGRAM_SELECT == GYRO_READ)
        uint16_t x_receive = 0;
        uint16_t y_receive = 0;
        uint16_t z_receive = 0;
        xQueueReceive(x_result_queue, &x_receive, portMAX_DELAY);
        xQueueReceive(y_result_queue, &y_receive, portMAX_DELAY);
        xQueueReceive(z_result_queue, &z_receive, portMAX_DELAY);
        custom_printf("[x: %d\t, y: %d\t, z: %d]\r\n", x_receive, y_receive, z_receive);
        vTaskDelay(3000);
#endif
    }
}

void get_voltage_level(void *param)
{
    for (;;)
    {
        voltage = adc1_vin_measure();
        xQueueSend(voltage_queue, &voltage, portMAX_DELAY);
        vTaskDelay(500);
    }
}

void get_gyro_value(void *param)
{
    for (;;)
    {
        read_gyro_through_spi();
        xQueueSend(x_result_queue, &x_result, portMAX_DELAY);
        xQueueSend(y_result_queue, &y_result, portMAX_DELAY);
        xQueueSend(z_result_queue, &z_result, portMAX_DELAY);
        vTaskDelay(100);
    }
}

int main(void)
{
  LED_init();
  tim1_init();
  uart1_init();
  adc1_init();
  spi1_init();

  xTaskCreate(led1_blink_func,
              "blinkLed1",
              1024,
              NULL,
              1,
              &blink_led1_task);

  xTaskCreate(led2_blink_func,
              "blinkLed2",
              1024,
              NULL,
              1,
              &blink_led2_task);

  xTaskCreate(print_log,
              "printLog",
              1024,
              NULL,
              0,
              &print_log_task);

  xTaskCreate(get_voltage_level,
              "getVolLev",
              1024,
              NULL,
              0,
              &get_voltage_task);

  xTaskCreate(get_gyro_value,
              "getGyro",
              1024,
              NULL,
              0,
              &get_gyro_value_task);

  voltage_queue = xQueueCreate(16, sizeof(int));
  x_result_queue = xQueueCreate(16, sizeof(int16_t));
  y_result_queue = xQueueCreate(16, sizeof(int16_t));
  z_result_queue = xQueueCreate(16, sizeof(int16_t));

  vTaskStartScheduler();

  while (1)
  {

  }
}