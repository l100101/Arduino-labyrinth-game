#ifndef PLAYER_H
#define PLAYER_H
#include <stdint.h>

#define DIR_UP (uint8_t)0
#define DIR_DOWN (uint8_t)1
#define DIR_LEFT (uint8_t)2
#define DIR_RIGHT (uint8_t)3
class Player {
private:
    uint8_t currentX;
    uint8_t currentY;
    uint8_t previousX;
    uint8_t previousY;
    bool flashlightOn;
    uint8_t numberOfKeys;
    uint8_t hp;
    uint8_t maxHp;
    
public:
    Player(int startX, int startY, bool initialFlashlightState, int initialNumberOfKeys);
    void move(int newX, int newY);
    void move(uint8_t dir);
    void toggleFlashlight();
    void addKeys(int keysToAdd);
    void dropKeys(int keysToDrop);
    void getCurrentCoordinates(int& x, int& y) const;
    uint8_t getCurrentX();
    uint8_t getCurrentY();
    uint8_t getPreviousX();
    uint8_t getPreviousY();
    void getPreviousCoordinates(int& x, int& y) const;
    bool isFlashlightOn() const;
    uint8_t getNumberOfKeys() const;
    uint8_t getHp() const;
    void getDamage(int damage);
    void getHeal(int heal);

};

#endif // PLAYER_H
