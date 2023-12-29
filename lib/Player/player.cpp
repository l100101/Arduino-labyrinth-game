#include "player.h"
#include <Arduino.h>


Player::Player(int startX, int startY) :
    currentX(startX),
    currentY(startY),
    previousX(startX),
    previousY(startY),
    flashlightOn(0),
    numberOfKeys(0),
    hp(0)
     {}

void Player::teleport(int newX, int newY) {
    previousX = currentX;
    previousY = currentY;
    currentX = newX;
    currentY = newY;
}

int8_t Player::getCurrentX(){
    return currentX;    
}

int8_t Player::getCurrentY(){
    return currentY;
}

int8_t Player::getPreviousX(){
    return previousX;
}
int8_t Player::getPreviousY(){
    return previousY;
}
 void Player::setCurrentX(int newX) {
     currentX = newX; 
 }
 void Player::setCurrentY(int newY) {
     currentY = newY;
 }
void Player::setCurrentXY(int newX, int newY){
    currentX = newX;
    currentY = newY;
}
void Player::move(uint8_t dir) {
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


void Player::getPreviousCoordinates(int& x, int& y) const {
    x = previousX;
    y = previousY;
}

void Player::flashlight(bool enabled){
    if (enabled)
    {
        flashlightOn =1;
        fieldOfViewStart = getCurrentX() - flashlightRadius;
        fieldOfViewEnd = getCurrentX() + flashlightRadius;
    }
    else{
        flashlightOn =0;
        fieldOfViewStart = 0;
        fieldOfViewEnd = 19;
    }
}

void Player::setFieldOfView(){
    if (flashlightOn)
    {
        fieldOfViewStart = getCurrentX() - flashlightRadius;
        fieldOfViewEnd = getCurrentX() + flashlightRadius;
    }
    else{
        fieldOfViewStart = 0;
        fieldOfViewEnd = 19;
    }
    if ( fieldOfViewStart < 0) fieldOfViewStart = 0;
    if ( fieldOfViewEnd > 19) fieldOfViewEnd = 19; 
}
void Player::getHeal(int heal) {
    hp+=heal;
if (hp > maxHp) hp = maxHp;    
}
void Player::getDamage(int damage) {
    hp+=damage;
if (hp < 0) hp = 0;
}
int8_t Player::getHp() const {
    return hp;
}
int8_t Player::getNumberOfKeys() const {
    return numberOfKeys;
}
void Player::getKey(uint8_t keys) {
    numberOfKeys++;
}
bool Player::isFlashlightOn() const {
    return flashlightOn;
}
void Player::toggleFlashlight() {
    flashlightOn = !flashlightOn;
}

void Player::addKeys(int keysToAdd) {
    numberOfKeys += keysToAdd;
}
void Player::dropKeys(int keysToDrop) {
    numberOfKeys -= keysToDrop;
}