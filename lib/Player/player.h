#ifndef PLAYER_H
#define PLAYER_H
#include <stdint.h>

#ifndef DIRS
#define DIRS
#define UP_DIR 0
#define DOWN_DIR 1
#define LEFT_DIR 2
#define RIGHT_DIR 3
#endif

class Player {
private:
    int8_t currentX;
    int8_t currentY;
    int8_t previousX;
    int8_t previousY;
    bool flashlightOn;
    int8_t numberOfKeys;
    int8_t maxHp=10;
    int8_t hp;
    
public:
    Player(int startX, int startY, uint8_t startHp);
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
    void getPreviousCoordinates(int& x, int& y);
    void teleport(int newX, int newY);
    
    void takeDamage(int damage);
    void takeHeal(int heal);
    void setHp(uint8_t newHp);
//getters
    int8_t getNumberOfKeys();
    int8_t getHp();

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
    void takeKeys(uint8_t keys);
    void dropKeys(uint8_t keysToDrop);
//game over
    void gameOver();
};

#endif // PLAYER_H
