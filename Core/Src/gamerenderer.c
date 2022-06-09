//
// Created by LNatit on 2022/5/8.
//

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
 *    ADMIRE THE ELDER TOAD OF ETERNITY
 *           TOAD THE DEBUGGER
 *
 */
#include "gamerenderer.h"

extern uint16_t palsand[8][8];
extern uint16_t palbrick[8][8];
extern uint16_t palrock[8][8];
extern uint16_t palocean[8][8];
extern uint16_t palshell[8][8];
extern uint16_t palhome[24][24];
extern uint16_t paltankup[24][24];
extern uint16_t paltankdown[24][24];
extern uint16_t paltankleft[24][24];
extern uint16_t paltankright[24][24];
extern uint16_t palenemystankdown[24][24];
extern uint16_t palenemystankleft[24][24];
extern uint16_t palenemystankright[24][24];
extern uint16_t palenemystankup[24][24];

extern uint8_t dirty_map[41][41];
extern Tank player, enemy[ENEMY_MAX];
extern Bullet bullet[BULLET_NUM];
extern uint8_t map[41][41];

// TODO: modify logic, using memo locating
void RDR_Render(uint8_t init)
{
    RDR_PrintMap(!init);
    printTank(player);

    for (int i = 0; i < ENEMY_MAX; ++i)
        if (enemy[i].alive)
            printTank(enemy[i]);

    if (!init)
        for (int i = 0; i < BULLET_NUM; ++i)
            if (bullet[i].exist)
//    && bullet[i].x != 0 && bullet[i].x != 40 && bullet[i].y != 0 && bullet[i].y != 40
                printBullet(bullet[i]);
}

void RDR_PrintMap(uint8_t fast)
{
    for (uint16_t ytick = 1; ytick <= 39; ytick++)
    {
        for (uint16_t xtick = 1; xtick <= 39; xtick++)
        {
            if (fast == 0)
            {
                printBlock(ytick, xtick);
                printHome();
            } else if (dirty_map[ytick][xtick])
            {
                printBlock(ytick, xtick);
                dirty_map[ytick][xtick] = 0;
            }
        }
    }
}

void RDR_PrintScreen(uint8_t flag2go)
{
    LCD_Clear(WHITE);
    POINT_COLOR = RED;
}

void printPixel(uint16_t ylocate, uint16_t xlocate, uint16_t colour)
{
    LCD_Fast_DrawPoint(xlocate, ylocate, colour);
}

void printSand(uint16_t y, uint16_t x)/*0*/
{
    uint16_t ypallete = 0;
    uint16_t xpallete = 0;
    for (uint16_t yscreentick = 8 * y + 76; yscreentick <= 8 * y + 83; yscreentick++)
    {
        for (uint16_t xscreentick = 8 * x - 4; xscreentick <= 8 * x + 3; xscreentick++)
        {
            printPixel(yscreentick, xscreentick, palsand[ypallete][xpallete]);
            xpallete++;
        }
        xpallete = 0;
        ypallete++;
    }
}

void printBrick(uint16_t y, uint16_t x)/*1*/
{
    uint16_t ypallete = 0;
    uint16_t xpallete = 0;
    for (uint16_t yscreentick = 8 * y + 76; yscreentick <= 8 * y + 83; yscreentick++)
    {
        for (uint16_t xscreentick = 8 * x - 4; xscreentick <= 8 * x + 3; xscreentick++)
        {
            printPixel(yscreentick, xscreentick, palbrick[ypallete][xpallete]);
            xpallete++;
        }
        xpallete = 0;
        ypallete++;
    }
}

void printRock(uint16_t y, uint16_t x)/*3*/
{
    uint16_t ypallete = 0;
    uint16_t xpallete = 0;
    for (uint16_t yscreentick = 8 * y + 76; yscreentick <= 8 * y + 83; yscreentick++)
    {
        for (uint16_t xscreentick = 8 * x - 4; xscreentick <= 8 * x + 3; xscreentick++)
        {
            printPixel(yscreentick, xscreentick, palrock[ypallete][xpallete]);
            xpallete++;
        }
        xpallete = 0;
        ypallete++;
    }
}

void printOcean(uint16_t y, uint16_t x)/*4 OR 5*/
{
    uint16_t ypallete = 0;
    uint16_t xpallete = 0;
    for (uint16_t yscreentick = 8 * y + 76; yscreentick <= 8 * y + 83; yscreentick++)
    {
        for (uint16_t xscreentick = 8 * x - 4; xscreentick <= 8 * x + 3; xscreentick++)
        {
            printPixel(yscreentick, xscreentick, palocean[ypallete][xpallete]);
            xpallete++;
        }
        xpallete = 0;
        ypallete++;
    }
}

void printHome()
{
    uint16_t ypallete = 0;
    uint16_t xpallete = 0;
    for (uint16_t yscreentick = 372; yscreentick <= 395; yscreentick++)
    {
        for (uint16_t xscreentick = 148; xscreentick <= 171; xscreentick++)
        {
            printPixel(yscreentick, xscreentick, palhome[ypallete][xpallete]);
            xpallete++;
        }
        xpallete = 0;
        ypallete++;
    }
}

void printBullet(Bullet bul)
{
    uint16_t ypallete = 0;
    uint16_t xpallete = 0;
    for (uint16_t yscreentick = 8 * (bul.y) + 76; yscreentick <= 8 * (bul.y) + 83; yscreentick++)
    {
        for (uint16_t xscreentick = 8 * (bul.x) - 4; xscreentick <= 8 * (bul.x) + 3; xscreentick++)
        {
            printPixel(yscreentick, xscreentick, palshell[ypallete][xpallete]);
            xpallete++;
        }
        xpallete = 0;
        ypallete++;
    }
}

void printTank(Tank tank)
{
    uint16_t ypallete = 0;
    uint16_t xpallete = 0;

    if (tank.my)
    {
        if (tank.direction == UP)
        {
            for (uint16_t yscreentick = 8 * (tank.y) + 76; yscreentick <= 8 * (tank.y) + 99; yscreentick++)
            {
                for (uint16_t xscreentick = 8 * (tank.x) - 4; xscreentick <= 8 * (tank.x) + 19; xscreentick++)
                {
                    printPixel(yscreentick, xscreentick, paltankup[ypallete][xpallete]);
                    xpallete++;
                }
                xpallete = 0;
                ypallete++;
            }
        } else if (tank.direction == DOWN)
        {
            for (uint16_t yscreentick = 8 * (tank.y) + 76; yscreentick <= 8 * (tank.y) + 99; yscreentick++)
            {
                for (uint16_t xscreentick = 8 * (tank.x) - 4; xscreentick <= 8 * (tank.x) + 19; xscreentick++)
                {
                    printPixel(yscreentick, xscreentick, paltankdown[ypallete][xpallete]);
                    xpallete++;
                }
                xpallete = 0;
                ypallete++;
            }
        } else if (tank.direction == LEFT)
        {
            for (uint16_t yscreentick = 8 * (tank.y) + 76; yscreentick <= 8 * (tank.y) + 99; yscreentick++)
            {
                for (uint16_t xscreentick = 8 * (tank.x) - 4; xscreentick <= 8 * (tank.x) + 19; xscreentick++)
                {
                    printPixel(yscreentick, xscreentick, paltankleft[ypallete][xpallete]);
                    xpallete++;
                }
                xpallete = 0;
                ypallete++;
            }
        } else if (tank.direction == RIGHT)
        {
            for (uint16_t yscreentick = 8 * (tank.y) + 76; yscreentick <= 8 * (tank.y) + 99; yscreentick++)
            {
                for (uint16_t xscreentick = 8 * (tank.x) - 4; xscreentick <= 8 * (tank.x) + 19; xscreentick++)
                {
                    printPixel(yscreentick, xscreentick, paltankright[ypallete][xpallete]);
                    xpallete++;
                }
                xpallete = 0;
                ypallete++;
            }
        }

    } else
    {
        if (tank.direction == 0)
        {
            for (uint16_t yscreentick = 8 * (tank.y) + 76; yscreentick <= 8 * (tank.y) + 99; yscreentick++)
            {
                for (uint16_t xscreentick = 8 * (tank.x) - 4; xscreentick <= 8 * (tank.x) + 19; xscreentick++)
                {
                    printPixel(yscreentick, xscreentick, palenemystankup[ypallete][xpallete]);
                    xpallete++;
                }
                xpallete = 0;
                ypallete++;
            }
        } else if (tank.direction == 1)
        {
            for (uint16_t yscreentick = 8 * (tank.y) + 76; yscreentick <= 8 * (tank.y) + 99; yscreentick++)
            {
                for (uint16_t xscreentick = 8 * (tank.x) - 4; xscreentick <= 8 * (tank.x) + 19; xscreentick++)
                {
                    printPixel(yscreentick, xscreentick, palenemystankdown[ypallete][xpallete]);
                    xpallete++;
                }
                xpallete = 0;
                ypallete++;
            }
        } else if (tank.direction == 2)
        {
            for (uint16_t yscreentick = 8 * (tank.y) + 76; yscreentick <= 8 * (tank.y) + 99; yscreentick++)
            {
                for (uint16_t xscreentick = 8 * (tank.x) - 4; xscreentick <= 8 * (tank.x) + 19; xscreentick++)
                {
                    printPixel(yscreentick, xscreentick, palenemystankleft[ypallete][xpallete]);
                    xpallete++;
                }
                xpallete = 0;
                ypallete++;
            }
        } else if (tank.direction == 3)
        {
            for (uint16_t yscreentick = 8 * (tank.y) + 76; yscreentick <= 8 * (tank.y) + 99; yscreentick++)
            {
                for (uint16_t xscreentick = 8 * (tank.x) - 4; xscreentick <= 8 * (tank.x) + 19; xscreentick++)
                {
                    printPixel(yscreentick, xscreentick, palenemystankright[ypallete][xpallete]);
                    xpallete++;
                }
                xpallete = 0;
                ypallete++;
            }
        }
    }
}

void printBlock(uint16_t ytick, uint16_t xtick)
{
    switch (map[ytick][xtick])
    {
        case SAND:
            printSand(ytick, xtick);
            break;
        case BRICK:
            printBrick(ytick, xtick);
            break;
        case ROCK:
            printRock(ytick, xtick);
            break;
        case OCEAN:
        case BORDER:
            printOcean(ytick, xtick);
            break;
        default:
            return;
    }
}
