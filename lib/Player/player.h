#ifndef PLAYER_H
#define PLAYER_H
#include <stdint.h>

enum {
    UP_DIR = 0,
    DOWN_DIR = 1,
    LEFT_DIR = 2,
    RIGHT_DIR = 3
};

class Player {
private:
    int8_t currentX;
    int8_t currentY;
    int8_t previousX;
    int8_t previousY;
    bool flashlightOn;
    int8_t numberOfKeys;
    int8_t hp;
    int8_t maxHp;
    
public:
    Player(int startX, int startY);
    void addKeys(int keysToAdd);
    void dropKeys(int keysToDrop);
    void toggleFlashlight();
    void move(int newX, int newY);
    void move(uint8_t dir);
    int8_t getCurrentX();
    int8_t getCurrentY();
    void setCurrentX(int newX);
    void setCurrentY(int newY);
    int8_t getPreviousX();
    int8_t getPreviousY();
    void getPreviousCoordinates(int& x, int& y) const;
    bool isFlashlightOn() const;
    int8_t getNumberOfKeys() const;
    int8_t getHp() const;
    void getDamage(int damage);
    void getHeal(int heal);
    void getKey(uint8_t keys);

};

#endif // PLAYER_H
