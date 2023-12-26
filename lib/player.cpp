#include "player.h"
#include <Arduino.h>

Player::Player(int startX, int startY, bool initialFlashlightState, int initialNumberOfKeys) :
    currentX(startX),
    currentY(startY),
    previousX(startX),
    previousY(startY),
    flashlightOn(initialFlashlightState),
    numberOfKeys(initialNumberOfKeys) {}

void Player::move(int newX, int newY) {
    previousX = currentX;
    previousY = currentY;
    currentX = newX;
    currentY = newY;
}

void Player::toggleFlashlight() {
    flashlightOn = !flashlightOn;
}

void Player::addKeys(int keysToAdd) {
    numberOfKeys += keysToAdd;
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

int Player::getNumberOfKeys() const {
    return numberOfKeys;
}