/**
 * Created by LNatit on 2022/6/2.
 */
#include "utils.h"

TIM_HandleTypeDef *delay_tim;
ADC_HandleTypeDef *random_adc;

void UTILS_Init(TIM_HandleTypeDef *tim, ADC_HandleTypeDef *hadc)
{
    delay_tim = tim;
    random_adc = hadc;
}

uint16_t UTILS_GetRNGSeed()
{
    HAL_ADC_Start(random_adc);
    HAL_ADC_PollForConversion(random_adc, 40);
    uint16_t seed = HAL_ADC_GetValue(random_adc);
    HAL_ADC_Stop(random_adc);

    return seed;
}

void Delay_us(const uint32_t us)
{
    uint32_t t = 0;
            __HAL_TIM_SetCounter(delay_tim, t);
    HAL_TIM_Base_Start(delay_tim);
    while (t < us)
        t = __HAL_TIM_GET_COUNTER(delay_tim);
    HAL_TIM_Base_Stop(delay_tim);
}

void Delay_ms(uint32_t ms)
{
    HAL_Delay(ms);
}
