/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
/**
 * Macro that stands for
 * different map materials
 */
#define SAND    0
#define BRICK   1
#define ROCK    2
#define HOME    3
#define BORDER  4
#define OCEAN   5
#define TANK    10

#define UP      0
#define DOWN    1
#define LEFT    2
#define RIGHT   3
#define STILL   4


#define PASS    0
#define WIN     1
#define LOSE    2
#define TITLE   3
#define INIT    4
#define ACK     5

#define LVL_NUM     8
#define BULLET_NUM  12
#define ENEMY_MAX   4

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
typedef struct      //坦克结构�???
{
    uint16_t x, y;       //坦克坐标（左上角�???
    uint8_t direction;  //坦克方向
    uint8_t revive;     //坦克重生次数
    uint8_t CD;         //发射子弹冷却计时
    uint8_t my;         //是否为玩家坦克参�???
    uint8_t alive;      //是否存活
} Tank;

typedef struct      //子弹结构�???
{
    uint16_t x, y;       //子弹坐标
    uint8_t direction;  //子弹方向
    uint8_t my;
    uint8_t exist;      //子弹存在与否
    uint8_t initial;    //子弹是否处于初始状�?�的�???
} Bullet;

typedef struct
{
    uint16_t player_x, player_y;
    uint16_t x[ENEMY_MAX], y[ENEMY_MAX];
    uint8_t direction[4];
    uint8_t enemy_num;
} Level_Info;
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
