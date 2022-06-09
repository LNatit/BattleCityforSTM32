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
extern Spawn *lvl_spawn;

uint8_t tank_id = 0;
uint8_t home_flag = 1;

void LGC_Init()
{
    // TODO: move to AST_Init()
    remain_enemy = 4;

    tank_id = 0;
    home_flag = 1;

    player.x = lvl_spawn->player_x;
    player.y = lvl_spawn->player_y;
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
            block_index = offset + TANK;
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
        if (!fire)
        {
            tankMapTrans(&player, PLAYER, PRE);
            tankMov(&player, direction);
            tankMapTrans(&player, PLAYER, POST);
        }

        if (player.CD != 0)
            (player.CD)--;
        else if (fire)
        {
            (player.CD) = 6;
            buildBullet(&player);
        }
    } else if (player.CD == 0)
    {
        playerRevive();
        tankMapTrans(&player, PLAYER, INITIAL);
    } else player.CD--;
}

void playerRevive(void)
{
    player.x = 25;
    player.y = 37;
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

            // TODO: optimize logic
            if (!fire)
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

            if (enemy[i].CD != 0)
                (enemy[i].CD)--;
            else if (fire)
            {
                (enemy[i].CD) = 6;
                buildBullet(enemy + i);
            }

        } else if (enemy[i].CD == 0 && remain_enemy > ENEMY_MAX)
        {
            buildEnemy(enemy + i);
            tankMapTrans(enemy + i, ENEMY, INITIAL);
        } else (enemy + i)->CD--;
    }
}

void buildEnemy(Tank *enmptr)
{
    enmptr->x = lvl_spawn->x[tank_id];
    enmptr->y = lvl_spawn->y[tank_id];
    enmptr->CD = 3;
    enmptr->alive = 1;
    enmptr->direction = lvl_spawn->direction[tank_id];

    if (tank_id > 3)
        tank_id = 0;
    else
        tank_id++;
}

uint8_t getEnemyFire(Tank *enmptr)
{
    //TODO
    return 0;
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
    }

    if (bullet[bul_num].x == 0 || bullet[bul_num].x == 40 || bullet[bul_num].y == 0 || bullet[bul_num].y == 40)
        return;

    bullet[bul_num].exist = 1;
    bullet[bul_num].initial = 1;

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
        player.alive = 0;
        tankMapTrans(&player, PLAYER, DEATH);
            break;
        case (TANK + 3):
            i++;
        case (TANK + 2):
            i++;
        case (TANK + 1):
            i++;
        case (TANK + 0):
            bulptr->exist = 0;
            enemy[i].alive = 0;
            remain_enemy--;
            tankMapTrans(enemy + i, ENEMY, DEATH);
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
    if ((!player.alive && player.revive == 0) || !home_flag)
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
