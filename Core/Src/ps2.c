//
// Created by LNatit on 2022/5/18.
//

#include "ps2.h"

ADC_HandleTypeDef *ADCx;
uint16_t buffer[2] = {0, 0};
uint8_t fire = 0;

void PS2_Init(ADC_HandleTypeDef *hadc)
{
    ADCx = hadc;
    HAL_ADCEx_Calibration_Start(hadc);    //ADC 校准
}

uint8_t PS2_GetDir(void)
{
    uint8_t direction = 0, flag = 0;

    uint16_t adc[4] = {0, buffer[0], 0, buffer[1]};

    adc[0] = ADC_MAX - adc[1];
    adc[2] = ADC_MAX - adc[3];

    for (uint8_t i = 0; i < 4; ++i)
    {
        if (adc[i] < DIR_THRESHOLD)
            flag++;
        else if (adc[i] > adc[direction])
            direction = i;
    }

    return flag == 4 ? STILL : direction;
}

uint8_t PS2_GetFire(void)
{
    if (fire)
    {
        fire = 0;
        HAL_NVIC_ClearPendingIRQ(EXTI9_5_IRQn);
        HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
        return 1;
    }

    return 0;
}

void PS2_StartSampling(void)
{
    HAL_ADC_Start(ADCx);
    HAL_ADC_PollForConversion(ADCx, 40);
    buffer[0] = HAL_ADC_GetValue(ADCx);

    HAL_ADC_Start(ADCx);
    HAL_ADC_PollForConversion(ADCx, 40);
    buffer[1] = HAL_ADC_GetValue(ADCx);

    HAL_ADC_Stop(ADCx);
}

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (!fire && GPIO_Pin == GPIO_PIN_5)
    {
        fire = 1;
        HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);
    }
}
