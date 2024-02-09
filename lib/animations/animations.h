#ifndef ANIMATIONS_H
#define ANIMATIONS_H
#include <stdint.h>

class Monster {
private:
    int8_t currentX;
    int8_t currentY;
    int8_t previousX;
    int8_t previousY;
    bool flashlightOn;
    int8_t numberOfKeys;
    int8_t maxHp;
    
public:
    play_anim_opening();
    play_anim_ending();

    Monster(int startX, int startY);
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
};

#endif // PLAYER_H
