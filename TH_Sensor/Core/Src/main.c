/* Includes */
#include "main.h"
#include "dht11.h"
#include <stdio.h>   // <<-- sprintf 함수를 위해 추가
#include <string.h>  // <<-- strlen 함수를 위해 추가

/* Private variables */
TIM_HandleTypeDef htim1;
UART_HandleTypeDef huart2;

DHT11_Data_TypeDef dht_data;
char uart_buf; // <<-- 단일 char에서 크기가 100인 char 배열로 수정

/* main function */
int main(void)
{
    HAL_Init();
    SystemClock_Config();
    MX_GPIO_Init();
    MX_TIM6_Init();
    MX_USART2_UART_Init();

    DHT11_Init(&htim6);
    HAL_TIM_Base_Start(&htim6);

    while (1)
    {
        if(DHT11_ReadData(&dht_data) == HAL_OK)
        {
            // uart_buf가 포인터 타입이 되도록 수정
            sprintf(uart_buf, "Temperature: %.1f C, Humidity: %.1f %%\r\n",
                    dht_data.Temperature, dht_data.Humidity);
            HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);
        }
        else
        {
            sprintf(uart_buf, "Failed to read from DHT sensor!\r\n");
            HAL_UART_Transmit(&huart2, (uint8_t*)uart_buf, strlen(uart_buf), HAL_MAX_DELAY);
        }

        HAL_Delay(2000);
    }
}
