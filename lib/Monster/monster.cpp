#include "monster.h"
#include <Arduino.h>

#define DEBUG
Monster::Monster(int startX, int startY) : currentX(startX),
                                           currentY(startY),
                                           previousX(startX),
                                           previousY(startY),
                                           dir(0),
                                           orientation(0),
                                           hp(0),
                                           xLimit1(0),
                                           xLimit2(20),
                                           yLimit1(0),
                                           yLimit2(3)
{
}

void Monster::teleport(int newX, int newY)
{
    previousX = currentX;
    previousY = currentY;
    currentX = newX;
    currentY = newY;
}
void Monster::setFieldMoving(int8_t xLim1, int8_t xLim2, int8_t yLim1, int8_t yLim2)
{
    xLimit1 = xLim1;
    xLimit2 = xLim2;
    yLimit1 = yLim1;
    yLimit2 = yLim2;
}

void Monster::check_step()
{
    previousX = currentX;
    previousY = currentY;
    if (currentX <= xLimit1 || currentX >= xLimit2)
    {
        currentX = previousX;
        dir = !dir;
    }
    if (currentY < yLimit1 || currentY > yLimit2)
    {
        currentY = previousY;
        dir = !dir;
    }
#ifdef DEBUG
    Serial.println("check_step X1:" + String(xLimit1) + " X2" + String(xLimit2) + " Y1" + String(yLimit1) + " Y2" + String(yLimit2));
#endif
}
void Monster::autoStep(uint8_t _orient) // 0 - horizontal, 1 - vertical, 2 - diagonal, 3 - square
{
    orientation = _orient;

    switch (orientation)
    {
    case HORIZONTAL:
        if (dir)
            currentX++;
        else
            currentX--;
        break;
    case VERTICAL:
        if (dir)
            currentY++;
        else
            currentY--;
        break;
    case DIAGONAL:
        if (dir)
        {
            currentX++;
            currentY++;
        }
        else
        {
            currentX--;
            currentY--;
        }
        break;
    }
    check_step();
}

int8_t Monster::getCurrentX()
{
    return currentX;
}

int8_t Monster::getCurrentY()
{
    return currentY;
}

int8_t Monster::getPreviousX()
{
    return previousX;
}
int8_t Monster::getPreviousY()
{
    return previousY;
}
void Monster::setCurrentX(int newX)
{
    currentX = newX;
}
void Monster::setCurrentY(int newY)
{
    currentY = newY;
}
void Monster::setCurrentXY(int newX, int newY)
{
    currentX = newX;
    currentY = newY;
}
void Monster::move(uint8_t dir)
{
    previousX = currentX;
    previousY = currentY;
    switch (dir)
    {
    case UP_DIR:
        currentY--;
        break;
    case DOWN_DIR:
        currentY++;
        break;
    case LEFT_DIR:
        currentX--;
        break;
    case RIGHT_DIR:
        currentX++;
        break;

    default:
        break;
    }
}

void Monster::getPreviousCoordinates(int &x, int &y)
{
    x = previousX;
    y = previousY;
}

void Monster::takeHeal(int heal)
{
    hp += heal;
    if (hp > maxHp)
        hp = maxHp;
}
void Monster::takeDamage(int damage)
{
    hp += damage;
    if (hp < 0)
        hp = 0;
}
int8_t Monster::getHp()
{
    return hp;
}
