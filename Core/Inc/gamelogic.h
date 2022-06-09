/**
 * Created by LNatit on 2022/5/8.
 */
#include "main.h"
#include "stdlib.h"

#ifndef ARMEXP_GAMELOGIC_H
#define ARMEXP_GAMELOGIC_H

// Phrase macros for tankMapTrans()
#define INITIAL 0
#define PRE     1
#define DEATH   1
#define POST    2

// Offset macros for tankMapTrans()
#define PLAYER  -1
#define ENEMY   i

//游戏主循环，返回游戏是否结束
void LGC_Init();
uint8_t LGC_Tick(uint8_t direction, uint8_t fire);

//functions below should not be called externally
uint8_t tankMov(Tank *tank, uint8_t direction);
uint8_t checkBarrier(Tank *tank, uint8_t direction);
void tankMapTrans(Tank *tank, uint8_t offset, uint8_t phrase);
void playerTick(uint8_t direction, uint8_t fire);
void playerRevive(void);
void enemyTick(void);
void buildEnemy(Tank *enmptr);
uint8_t getEnemyFire(Tank *enmptr);
void buildBullet(Tank *tank);
void bulletFly(void);
void bulletHit(Bullet* bulptr);
int bulletCheck(int x, int y);
uint8_t checkGoal(void);
//should be called before main logic!
void markDirty(void);
//should be called after tank moves!
void dirtyCulling(void);

#endif //ARMEXP_GAMELOGIC_H
