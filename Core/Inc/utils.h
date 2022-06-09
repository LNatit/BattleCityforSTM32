/**
 * Created by LNatit on 2022/6/2.
 */
#include "main.h"

#ifndef ARMPRJ_UTILS_H
#define ARMPRJ_UTILS_H

void UTILS_Init(TIM_HandleTypeDef *tim, ADC_HandleTypeDef *hadc);
uint16_t UTILS_GetRNGSeed();
void Delay_us(uint32_t us);
void Delay_ms(uint32_t ms);

#endif //ARMPRJ_UTILS_H
