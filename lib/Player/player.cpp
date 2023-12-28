#include "player.h"
#include <Arduino.h>


Player::Player(int startX, int startY, bool initialFlashlightState, int initialNumberOfKeys) :
    currentX(startX),
    currentY(startY),
    previousX(startX),
    previousY(startY),
    flashlightOn(initialFlashlightState),
    numberOfKeys(initialNumberOfKeys) {}

<<<<<<< HEAD:lib/Player/player.cpp
// void Player::move(int newX, int newY) {
//     previousX = currentX;
//     previousY = currentY;
//     currentX = newX;
//     currentY = newY;
// }
void Player::move(uint8_t dir) {
=======
void Player::move(int newX, int newY) { //add animation
>>>>>>> 00139799c876e334529e8ca606091ba24ae5e85f:lib/player.cpp
    previousX = currentX;
    previousY = currentY;
    switch (dir)
    {
    case 0:
    currentY++;
        break;
    case 1:
    currentY--;
        break;
    case DIR_LEFT:
    currentX--;
        break;
    case DIR_RIGHT:
    currentX++;
        break;
    
    default:
        break;
    }
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

void Player::getCurrentCoordinates(int& x, int& y) const {
    x = currentX;
    y = currentY;
}

void Player::getPreviousCoordinates(int& x, int& y) const {
    x = previousX;
    y = previousY;
}

bool Player::isFlashlightOn() const {
    return flashlightOn;
}

uint8_t Player::getNumberOfKeys() const {
    return numberOfKeys;
}

uint8_t Player::getHp() const {
    return hp;
}

void Player::getDamage(int damage) {
    hp+=damage;
if (hp < 0) hp = 0;
}

void Player::getHeal(int heal) {
    hp+=heal;
if (hp > maxHp) hp = maxHp;    
}

<<<<<<< HEAD:lib/Player/player.cpp
uint8_t Player::getCurrentX()
{
    return currentX;    
}

uint8_t Player::getCurrentY()
{
    return currentY;
}
=======

>>>>>>> 00139799c876e334529e8ca606091ba24ae5e85f:lib/player.cpp
