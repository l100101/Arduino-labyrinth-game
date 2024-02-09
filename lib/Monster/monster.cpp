#include "monster.h"
#include <Arduino.h>


Monster::Monster(int startX, int startY) :
    currentX(startX),
    currentY(startY),
    previousX(startX),
    previousY(startY),
    flashlightOn(0),
    numberOfKeys(0),
    hp(0)
     {}

void Monster::teleport(int newX, int newY) {
    previousX = currentX;
    previousY = currentY;
    currentX = newX;
    currentY = newY;
}

int8_t Monster::getCurrentX(){
    return currentX;    
}

int8_t Monster::getCurrentY(){
    return currentY;
}

int8_t Monster::getPreviousX(){
    return previousX;
}
int8_t Monster::getPreviousY(){
    return previousY;
}
 void Monster::setCurrentX(int newX) {
     currentX = newX; 
 }
 void Monster::setCurrentY(int newY) {
     currentY = newY;
 }
void Monster::setCurrentXY(int newX, int newY){
    currentX = newX;
    currentY = newY;
}
void Monster::move(uint8_t dir) {
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

void Monster::getPreviousCoordinates(int& x, int& y) const {
    x = previousX;
    y = previousY;
}

void Monster::getHeal(int heal) {
    hp+=heal;
if (hp > maxHp) hp = maxHp;    
}
void Monster::getDamage(int damage) {
    hp+=damage;
if (hp < 0) hp = 0;
}
int8_t Monster::getHp() const {
    return hp;
}

