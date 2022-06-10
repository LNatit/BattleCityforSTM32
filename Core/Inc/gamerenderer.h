/**
 * Created by LNatit on 2022/5/8.
 * Maintained by wzy i.e. Animus_Sagittarii
 */

 /*
VERSION 0.1.0 UPDATED ON 14/5/2022
VERSION 0.2.0 UPDATED ON 14/5/2022
VERSION 0.3.0 UPDATED ON 17/5/2022
 */

 /*
 *
 *              1926 0817
 *
 *
 *            -==[ ]=[ ]==-
 *
 *                 o o
 *             __       __
 *               --___--
 *
 *
 *          TOAD THE DEBUGGER
 *    ADMIRE THE ELDER TOAD OF ETERNITY
 *
 */
#include "main.h"
#include "assets.h"
#include "lcd.h"

#ifndef ARMEXP_GAMERENDERER_H
#define ARMEXP_GAMERENDERER_H

/*PALLETE SETTINGS*/
//------------------------------------------------------------------------------//

//------------------------------------------------------------------------------//

//Global function of GameRenderer
void RDR_Render(uint8_t init);
void RDR_PrintMap(uint8_t fast);
void RDR_PrintScreen();

//functions below should not be called externally
void printPixel(uint16_t ylocate, uint16_t xlocate, uint16_t colour);

void printSand(uint16_t y, uint16_t x);
void printBrick(uint16_t y, uint16_t x);
void printRock(uint16_t y, uint16_t x);
void printOcean(uint16_t y, uint16_t x);

void printBlock(uint16_t ytick, uint16_t xtick);
void printBullet(Bullet bul);
void printTank(Tank tank);
void printHome(void);

#endif //ARMEXP_GAMERENDERER_H
