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
    int8_t maxHp;
    
public:
    Player(int startX, int startY);
    int8_t hp;
    //координаты
    void move(int newX, int newY);
    void move(uint8_t dir);
    int8_t getCurrentX();
    int8_t getCurrentY();
    void setCurrentX(int newX);
    void setCurrentY(int newY);
    void setCurrentXY(int newX, int newY);
    int8_t getPreviousX();
    int8_t getPreviousY();
    void getPreviousCoordinates(int& x, int& y) const;
    void teleport(int newX, int newY);

//getters
    int8_t getNumberOfKeys() const;
    int8_t getHp() const;
    void getDamage(int damage);
    void getHeal(int heal);
    void getKey(uint8_t keys);

//область видимости
    int8_t fieldOfViewStart=0;//стандартное 
    int8_t fieldOfViewEnd=19;// поле видимости
    int8_t flashlightRadius=2;//поле видимости фонаря
    void setFieldOfView();
//фонарь
    bool isFlashlightOn() const;
    void flashlight(bool enabled);
    void toggleFlashlight();
//ключи
    void addKeys(int keysToAdd);
    void dropKeys(int keysToDrop);
};

#endif // PLAYER_H
