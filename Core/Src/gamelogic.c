//
// Created by LNatit on 2022/5/8.
//

#include "gamelogic.h"

extern uint8_t bul_num;
extern uint8_t dirty_map[41][41];
extern uint8_t map[41][41];
extern uint8_t remain_enemy;
extern Tank player, enemy[ENEMY_MAX];
extern Bullet bullet[BULLET_NUM];
extern Level_Info *lvl_info;

uint8_t tank_id = 0;
uint8_t home_flag = 1;

void LGC_Init()
{
    remain_enemy = lvl_info->enemy_num;

    tank_id = 0;
    home_flag = 1;

    player.x = lvl_info->player_x;
    player.y = lvl_info->player_y;
    player.my = 1;
    player.CD = 0;
    player.alive = 1;
    player.revive = 3;
    player.direction = UP;
    tankMapTrans(&player, PLAYER, INITIAL);

    for (int i = 0; i < ENEMY_MAX; ++i)
    {
        enemy[i].my = 0;
        enemy[i].revive = 0;
        buildEnemy(enemy + i);
        tankMapTrans(enemy + i, ENEMY, INITIAL);
    }

    for (int i = 0; i < BULLET_NUM; ++i)
        bullet[i].exist = 0;
}

uint8_t LGC_Tick(uint8_t direction, uint8_t fire)
{
    //mark old pos as dirty
    markDirty();
    //check bomb hit first
    bulletFly();
    //player updates first, then enemies do
    playerTick(direction, fire);
    enemyTick();
    //culling dirtymap to prevent texture flickering
    dirtyCulling();
    //check whether game should go on
    return checkGoal();
}

uint8_t tankMov(Tank *tank, uint8_t direction)
{
    if (direction == STILL)
        return 0;

    tank->direction = direction;
    uint8_t flag = checkBarrier(tank, direction);
    if (flag)
        switch (direction)
        {
            case UP:
                (tank->y)--;
                return 1;
            case DOWN:
                (tank->y)++;
                return 1;
            case LEFT:
                (tank->x)--;
                return 1;
            case RIGHT:
                (tank->x)++;
                return 1;
            default:
                return 0;
        }
    return 0;
}

uint8_t checkBarrier(Tank *tank, uint8_t direction)
{
    switch (direction)
    {
        case UP:
            if (map[tank->y - 1][tank->x] == SAND && map[tank->y - 1][tank->x + 1] == SAND &&
                map[tank->y - 1][tank->x + 2] == SAND)
                return 1;
            else
                return 0;
        case DOWN:
            if (map[tank->y + 3][tank->x] == SAND && map[tank->y + 3][tank->x + 1] == SAND &&
                map[tank->y + 3][tank->x + 2] == SAND)
                return 1;
            else
                return 0;
        case LEFT:
            if (map[tank->y][tank->x - 1] == SAND && map[tank->y + 1][tank->x - 1] == SAND &&
                map[tank->y + 2][tank->x - 1] == SAND)
                return 1;
            else
                return 0;
        case RIGHT:
            if (map[tank->y][tank->x + 3] == SAND && map[tank->y + 1][tank->x + 3] == SAND &&
                map[tank->y + 2][tank->x + 3] == SAND)
                return 1;
            else
                return 0;
        default:
            return 0;
    }
}

void tankMapTrans(Tank *tank, uint8_t offset, uint8_t phrase)
{
    uint8_t x = tank->x;
    uint8_t y = tank->y;
    uint8_t block_index = SAND;

    switch (phrase)
    {
        case INIT:
        case POST:
            block_index = TANK + offset;
        case PRE:
            break;
        default:
            return;
    }

    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            map[y + j][x + i] = block_index;
}

void playerTick(uint8_t direction, uint8_t fire)
{
    if (player.alive)
    {
        if (player.CD > 0)
            (player.CD)--;

        if (fire && player.CD == 0)
        {
            (player.CD) = 6;
            buildBullet(&player);
        } else
        {
            tankMapTrans(&player, PLAYER, PRE);
            tankMov(&player, direction);
            tankMapTrans(&player, PLAYER, POST);
        }


//        if (!fire)
//        {
//            tankMapTrans(&player, PLAYER, PRE);
//            tankMov(&player, direction);
//            tankMapTrans(&player, PLAYER, POST);
//        }
//
//        if (player.CD != 0)
//            (player.CD)--;
//        else if (fire)
//        {
//            (player.CD) = 6;
//            buildBullet(&player);
//        }
    } else if (player.CD == 0)
    {
        playerRevive();
        tankMapTrans(&player, PLAYER, INITIAL);
    } else player.CD--;
}

void playerRevive(void)
{
    player.x = lvl_info->player_x;
    player.y = lvl_info->player_y;
    player.direction = UP;
    player.alive = 1;
    player.revive--;
}

void enemyTick()
{
    for (int i = 0; i < ENEMY_MAX; ++i)
    {
        if (enemy[i].alive)
        {
            uint8_t fire = getEnemyFire(enemy + i);

            if (enemy[i].CD > 0)
                (enemy[i].CD)--;

            // TODO: optimize logic
            if (fire && enemy[i].CD == 0)
            {
                (enemy[i].CD) = 6;
                buildBullet(enemy + i);
            } else
            {
                uint8_t dir = STILL;

                if (!(rand() & 0b01))
                    dir = enemy[i].direction;

                if (!(rand() & 0b1111))
                    dir = rand() & 0b11;

                tankMapTrans(enemy + i, ENEMY, PRE);

                if (!tankMov(enemy + i, dir))
                    if (!(rand() & 0b11) || checkBarrier(enemy + i, dir))
                        enemy[i].direction = rand() & 0b11;

                tankMapTrans(enemy + i, ENEMY, POST);
            }
        } else if (enemy[i].CD == 0 && remain_enemy >= ENEMY_MAX)
        {
            buildEnemy(enemy + i);
            tankMapTrans(enemy + i, ENEMY, INITIAL);
        } else (enemy + i)->CD--;
    }
}

void buildEnemy(Tank *enmptr)
{
    enmptr->x = lvl_info->x[tank_id];
    enmptr->y = lvl_info->y[tank_id];
    enmptr->CD = 3;
    enmptr->alive = 1;
    enmptr->direction = lvl_info->direction[tank_id];

    tank_id++;
    if (tank_id >= ENEMY_MAX)
        tank_id = 0;
}

uint8_t getEnemyFire(Tank *enmptr)
{
    //TODO
//    uint8_t infront = checkfront(enmptr, enmptr->direction);
    uint8_t step = (enmptr->direction == UP || enmptr->direction == LEFT) ? -1 : 1;
    uint8_t flag = (enmptr->direction == UP || enmptr->direction == DOWN);
    uint16_t start_x = enmptr->x + 1, start_y = enmptr->y + 1;

    for (int pos = flag ? (start_y + 2 * step) : (start_x + 2 * step), i = 0; pos < 40 && pos > 0; i++, pos += step)
    {
        uint8_t block_index;

        if (flag)
            block_index = map[pos][start_x];
        else
            block_index = map[start_y][pos];

        if (block_index == SAND)
            continue;
        else if (block_index == (TANK + PLAYER) || (block_index == BRICK && i == 0))
            return enmptr->CD == 0;
        else if (block_index >= TANK)
            return 0;
        break;
    }

    if (enmptr->CD == 0)
        return !(rand() & 0b0111);

    return 0;

//    if (enmptr->CD >= 5)
//    {
//
//        if (enmptr->y == 8)//走到底格时向家开火
//        {
//            if (enmptr->x < 20)
//            {
//                if (enmptr->direction == RIGHT)
//                {
//                    return 1;
//                }
//            } else if (enmptr->direction == LEFT)
//            {
//                return 1;
//            }
//        } else if (enmptr->x == player.x + 1 || enmptr->x == player.x || enmptr->x == player.x - 1)//面对玩家且之间无障碍时开火
//        {
//            if ((enmptr->direction == DOWN && player.y > enmptr->y) ||
//                (enmptr->direction == UP && player.y < enmptr->y))
//            {
//                int big = player.y, smal = enmptr->y, i;
//                if (player.y < enmptr->y)
//                {
//                    big = enmptr->y;
//                    smal = player.y;
//                }
//                for (i = smal + 2; i <= big - 2; i++)
//                    if (map[i][enmptr->x] != 0 || map[i][enmptr->x] != 5)
//                        break;
//                if (i == big - 1)
//                {
//                    return 1;
//                }
//            }
//        } else if (enmptr->y == player.y + 1 || enmptr->y == player.y || enmptr->y == player.y - 1)
//        {
//
//            if ((enmptr->direction == RIGHT && player.x > enmptr->x) ||
//                (enmptr->direction == LEFT && player.x < enmptr->x))
//            {
//                int big = player.y, smal = player.y, i;
//                if (player.x < enmptr->x)
//                {
//                    big = enmptr->x;
//                    smal = player.x;
//                }
//                for (i = smal + 2; i <= big - 2; i++)
//                    if (map[enmptr->y][i] != 0 || map[enmptr->y][i] != 5)
//                        break;
//                if (i == big - 1)
//                {
//                    return 1;
//                }
//            }
//        } else if (infront)//若前方有砖块则开火破坏
//        {
//            return 1;
//        }
//    }
}

void buildBullet(Tank *tank)
{
    switch (tank->direction)
    {
        case UP    :
            bullet[bul_num].x = tank->x + 1;
            bullet[bul_num].y = tank->y - 1;
            bullet[bul_num].direction = UP;
            break;
        case DOWN :
            bullet[bul_num].x = tank->x + 1;
            bullet[bul_num].y = tank->y + 3;
            bullet[bul_num].direction = DOWN;
            break;
        case LEFT  :
            bullet[bul_num].x = tank->x - 1;
            bullet[bul_num].y = tank->y + 1;
            bullet[bul_num].direction = LEFT;
            break;
        case RIGHT :
            bullet[bul_num].x = tank->x + 3;
            bullet[bul_num].y = tank->y + 1;
            bullet[bul_num].direction = RIGHT;
            break;
        default:
            return;
    }

    if (bullet[bul_num].x == 0 || bullet[bul_num].x == 40 || bullet[bul_num].y == 0 || bullet[bul_num].y == 40)
        return;

    bullet[bul_num].exist = 1;
    bullet[bul_num].initial = 1;
    bullet[bul_num].my = tank->my;

    bul_num++;
    if (bul_num == BULLET_NUM)
        bul_num = 0;
}

void bulletFly()
{
    for (int i = 0; i < BULLET_NUM; i++)
    {
        if (bullet[i].exist)
        {
            if (bullet[i].initial == 0)
            {
                switch (bullet[i].direction)
                {
                    case UP    :
                        (bullet[i].y)--;
                        break;
                    case DOWN  :
                        (bullet[i].y)++;
                        break;
                    case LEFT  :
                        (bullet[i].x)--;
                        break;
                    case RIGHT :
                        (bullet[i].x)++;
                        break;
                }
            }

            if (!bulletCheck(bullet[i].x, bullet[i].y))
                bulletHit(bullet + i);

            if (bullet[i].initial)
                bullet[i].initial = 0;

            for (int j = i + 1; j < BULLET_NUM; ++j)
            {
                if (bullet[j].exist && bullet[i].x == bullet[j].x && bullet[i].y == bullet[j].y)
                {
                    bullet[i].exist = 0;
                    bullet[j].exist = 0;
                    break;
                }
            }
        }
    }
}

void bulletHit(Bullet *bulptr)
{
    uint8_t x = bulptr->x;
    uint8_t y = bulptr->y;
    uint8_t block_index = map[y][x];
    uint8_t dir;
    int i = 0;

    switch (block_index)
    {
        case BRICK:
            dir = bulptr->direction;
            if (dir == UP || dir == DOWN)
                for (i = -1; i <= 1; i++)
                    if (map[y][x + i] == BRICK)
                    {
                        map[y][x + i] = SAND;
                        dirty_map[y][x + i] = 1;
                    }
            if (dir == LEFT || dir == RIGHT)
                for (i = -1; i <= 1; i++)
                    if (map[y + i][x] == BRICK)
                    {
                        map[y + i][x] = SAND;
                        dirty_map[y + i][x] = 1;
                    }
            bulptr->exist = 0;
            break;
        case ROCK:
        case BORDER:
            bulptr->exist = 0;
            break;
        case HOME:
            bulptr->exist = 0;
            home_flag = 0;
            break;
        case (TANK + PLAYER):
            bulptr->exist = 0;
            if (!(bulptr->my))
            {
                player.alive = 0;
                tankMapTrans(&player, PLAYER, DEATH);
            }
            break;
        case (TANK + 3):
            i++;
        case (TANK + 2):
            i++;
        case (TANK + 1):
            i++;
        case (TANK + 0):
            bulptr->exist = 0;
            if (bulptr->my)
            {
                enemy[i].alive = 0;
                remain_enemy--;
                tankMapTrans(enemy + i, ENEMY, DEATH);
            }
            break;
    }
}

int bulletCheck(int x, int y)
{
    if (map[y][x] == SAND)
        return 1;
    else if (map[y][x] == OCEAN)
        return 2;
    else
        return 0;
}

uint8_t checkGoal()
{
    int i;

    if ((player.revive == 0) || !home_flag)
        return LOSE;

    if (remain_enemy == 0)
        return WIN;

    return PASS;
}

void markDirty()
{
    for (int y = player.y; y < player.y + 3; ++y)
        for (int x = player.x; x < player.x + 3; ++x)
            dirty_map[y][x] = 1;

    for (int i = 0; i < ENEMY_MAX; ++i)
        if (enemy[i].alive)
            for (int y = enemy[i].y; y < enemy[i].y + 3; ++y)
                for (int x = enemy[i].x; x < enemy[i].x + 3; ++x)
                    dirty_map[y][x] = 1;

    for (int i = 0; i < BULLET_NUM; ++i)
        if (bullet[i].exist)
            dirty_map[bullet[i].y][bullet[i].x] = 1;
}

void dirtyCulling()
{
    for (int y = player.y; y < player.y + 3; ++y)
        for (int x = player.x; x < player.x + 3; ++x)
            dirty_map[y][x] = 0;

    for (int i = 0; i < ENEMY_MAX; ++i)
        if (enemy[i].alive)
            for (int y = enemy[i].y; y < enemy[i].y + 3; ++y)
                for (int x = enemy[i].x; x < enemy[i].x + 3; ++x)
                    dirty_map[y][x] = 0;
}
