#include "dht11.h" // <<-- 이 줄을 추가하여 HAL 라이브러리 정의를 가져옵니다.

// dht11.c 파일 상단에 전역 변수 선언
static TIM_HandleTypeDef *dht_htim;

// GPIO 핀 정보 (사용자 환경에 맞게 수정)
#define DHT11_PORT GPIOA
#define DHT11_PIN  GPIO_PIN_8

// 내부용 함수 프로토타입 선언
static void delay_us(uint16_t us);
static void Set_Pin_Output(void);
static void Set_Pin_Input(void);
static HAL_StatusTypeDef DHT11_Start(void);
static uint8_t DHT11_Read_Byte(void);

// 초기화 함수
void DHT11_Init(TIM_HandleTypeDef *htim)
{
    dht_htim = htim;
}

// 마이크로초 지연 함수
static void delay_us(uint16_t us)
{
    __HAL_TIM_SET_COUNTER(dht_htim, 0);
    while (__HAL_TIM_GET_COUNTER(dht_htim) < us);
}

// GPIO 출력 모드 설정
static void Set_Pin_Output(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

// GPIO 입력 모드 설정
static void Set_Pin_Input(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = DHT11_PIN;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStruct);
}

// 시작 신호 전송 및 응답 확인
static HAL_StatusTypeDef DHT11_Start(void)
{
    Set_Pin_Output();
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_RESET);
    delay_us(18000);
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, GPIO_PIN_SET);
    delay_us(30);
    Set_Pin_Input();

    // 응답 확인 (타임아웃 추가)
    __HAL_TIM_SET_COUNTER(dht_htim, 0);
    while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET)
    {
        if(__HAL_TIM_GET_COUNTER(dht_htim) > 100) return HAL_TIMEOUT;
    }

    __HAL_TIM_SET_COUNTER(dht_htim, 0);
    while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET)
    {
        if(__HAL_TIM_GET_COUNTER(dht_htim) > 100) return HAL_TIMEOUT;
    }

    __HAL_TIM_SET_COUNTER(dht_htim, 0);
    while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET)
    {
        if(__HAL_TIM_GET_COUNTER(dht_htim) > 100) return HAL_TIMEOUT;
    }

    return HAL_OK;
}

// 1바이트 데이터 읽기
static uint8_t DHT11_Read_Byte(void)
{
    uint8_t data_byte = 0;
    for(int i = 0; i < 8; i++)
    {
        __HAL_TIM_SET_COUNTER(dht_htim, 0);
        while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_RESET)
        {
            if(__HAL_TIM_GET_COUNTER(dht_htim) > 60) return 0;
        }

        __HAL_TIM_SET_COUNTER(dht_htim, 0);
        while(HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) == GPIO_PIN_SET)
        {
            if(__HAL_TIM_GET_COUNTER(dht_htim) > 80) return 0;
        }
        uint16_t pulse_width = __HAL_TIM_GET_COUNTER(dht_htim);

        if(pulse_width > 40)
        {
            data_byte |= (1 << (7 - i));
        }
    }
    return data_byte;
}

// [수정된 부분] 데이터 읽기 메인 함수
HAL_StatusTypeDef DHT11_ReadData(DHT11_Data_TypeDef *data)
{
    uint8_t buffer[1] = {0}; // 5바이트 데이터를 저장할 배열로 수정

    if(DHT11_Start()!= HAL_OK)
    {
        return HAL_TIMEOUT;
    }

    for(int i = 0; i < 5; i++)
    {
        buffer[i] = DHT11_Read_Byte();
    }

    // 체크섬 검증 로직 수정
    if(buffer[2] == ((buffer + buffer[3] + buffer[4] + buffer[5]) & 0xFF))
    {
        // 데이터 할당 로직 수정
        data->Humidity = (float)buffer;
        data->Temperature = (float)buffer[4];
        return HAL_OK;
    }
    else
    {
        return HAL_ERROR;
    }
}
