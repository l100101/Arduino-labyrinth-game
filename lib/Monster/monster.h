#ifndef MONSTER_H
#define MONSTER_H
#include <stdint.h>

enum {
    HORIZONTAL = 0,
    VERTICAL = 1,
    DIAGONAL = 2,
    SQUARE = 3
};

#ifndef DIRS
#define DIRS  
#define UP_DIR 0
#define DOWN_DIR 1
#define LEFT_DIR 2
#define RIGHT_DIR 3
#endif

class Monster {
private:
    int8_t currentX;
    int8_t currentY;
    int8_t previousX;
    int8_t previousY;
    int8_t maxHp;
    int8_t xLimit1;
    int8_t xLimit2;
    int8_t yLimit1;
    int8_t yLimit2;
public:
    Monster(int startX, int startY);
    int8_t hp;
    int8_t dir;
    int8_t orientation;
    //координаты
    void move(int newX, int newY);
    void move(uint8_t dir);
    void autoStep(uint8_t _orient);
    int8_t getCurrentX();
    int8_t getCurrentY();
    void setCurrentX(int newX);
    void setCurrentY(int newY);
    void setCurrentXY(int newX, int newY);
    int8_t getPreviousX();
    int8_t getPreviousY();
    void getPreviousCoordinates(int& x, int& y);
    void teleport(int newX, int newY);
    void setFieldMoving(int8_t xLim1, int8_t xLim2, int8_t yLim1, int8_t yLim2 );
    int8_t getHp();
//takers
    void takeDamage(int damage);
    void takeHeal(int heal);
};

#endif // PLAYER_H
