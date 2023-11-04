
#include "main.h"

extern uint8_t id;
extern uint8_t status;
extern uint16_t x_result, y_result, z_result;

#if (FREERTOS_FEATURE == FREERTOS_ENABLE)
TaskHandle_t blink_led1_task;
TaskHandle_t blink_led2_task;
TaskHandle_t print_log_task;
TaskHandle_t get_voltage_task;
TaskHandle_t get_cpu_temp_task;
TaskHandle_t get_gyro_value_task;
TaskHandle_t card_rfid_read_task;

QueueHandle_t voltage_queue;
QueueHandle_t cpu_temp_queue;
QueueHandle_t x_result_queue;
QueueHandle_t y_result_queue;
QueueHandle_t z_result_queue;

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
#if (PROGRAM_SELECT == VOLTAGE_ADC)
        int voltage_adc = 0;
        xQueueReceive(voltage_queue, &voltage_adc, portMAX_DELAY);
        custom_printf("Voltage level: %dmV\r\n", voltage_adc);
        vTaskDelay(3000);
#elif (PROGRAM_SELECT == CPU_TEMP)
        int cpu_temp = 0;
        xQueueReceive(cpu_temp_queue, &cpu_temp, portMAX_DELAY);
        custom_printf("CPU temperature: %d°C\r\n", cpu_temp);
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
        int voltage = adc1_vin_measure();
        xQueueSend(voltage_queue, &voltage, portMAX_DELAY);
        vTaskDelay(500);
    }
}

void get_cpu_temp(void *param)
{
    for (;;)
    {
        int cpu_temp = adc1_get_temp();
        xQueueSend(cpu_temp_queue, &cpu_temp, portMAX_DELAY);
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

void card_rfid_read(void *param)
{
    for (;;)
    {
        MFRC522_ReadDemo();
        vTaskDelay(1);
    }
}
#endif

int main(void)
{
#if (FREERTOS_FEATURE == FREERTOS_ENABLE)
  LED_init();
  tim1_init();
  uart1_init(); 

  xTaskCreate(print_log,
              "printLog",
              1024,
              NULL,
              0,
              &print_log_task);

#if (PROGRAM_SELECT != CARD_READ)
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
#endif

#if (PROGRAM_SELECT == VOLTAGE_ADC)
  adc1_init();
  xTaskCreate(get_voltage_level,
              "getVolLev",
              1024,
              NULL,
              0,
              &get_voltage_task);

  voltage_queue = xQueueCreate(16, sizeof(int));
#elif (PROGRAM_SELECT == CPU_TEMP)
  adc1_init();
  xTaskCreate(get_cpu_temp,
              "getCPUt",
              1024,
              NULL,
              0,
              &get_cpu_temp_task);

  cpu_temp_queue = xQueueCreate(16, sizeof(int));
#elif (PROGRAM_SELECT == GYRO_READ)
  spi1_init();
  xTaskCreate(get_gyro_value,
              "getGyro",
              1024,
              NULL,
              0,
              &get_gyro_value_task);

  x_result_queue = xQueueCreate(16, sizeof(int16_t));
  y_result_queue = xQueueCreate(16, sizeof(int16_t));
  z_result_queue = xQueueCreate(16, sizeof(int16_t));              
#elif (PROGRAM_SELECT == CARD_READ)
  MFRC522_Init();
  xTaskCreate(card_rfid_read,
              "CardRead",
              1024,
              NULL,
              0,
              &card_rfid_read_task);
#endif

  vTaskStartScheduler();

#else // start normal code without FreeRTOS
  LED_init();
  tim1_init();
#endif

  while (1)
  {

  }
}

// Stub functions
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
