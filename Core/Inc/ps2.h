//
// Created by LNatit on 2022/5/18.
//

#ifndef ARMPRJ_PS2_H
#define ARMPRJ_PS2_H

#include "main.h"

#define ADC_MAX         ((0b01)<<12)
#define ADC_MIDDLE      ((0b01)<<11)
#define DIR_THRESHOLD   ((0b11)<<10)
//#define FIRE_THRESHOLD  ((0b01)<<8)

void PS2_Init(ADC_HandleTypeDef* hadc);
void PS2_StartSampling(void);
uint8_t PS2_GetDir(void);
uint8_t PS2_GetPressed(void);
void PS2_ClearFlag(void);

#endif //ARMPRJ_PS2_H
