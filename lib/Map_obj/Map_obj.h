#ifndef MAP_OBJ_H
#define MAP_OBJ_H
#include "LiquidCrystal_I2C.h"
#include "Player.h"

class MapObject {
  public:
    virtual void draw(LiquidCrystal_I2C& lcd) = 0;
    virtual void update(LiquidCrystal_I2C& lcd) = 0;
    virtual void collision(Player& player) = 0;
    virtual uint8_t setX(uint8_t x) = 0;
    virtual uint8_t setY(uint8_t y) = 0;
    virtual uint8_t getX() = 0;
    virtual uint8_t getY() = 0;
    virtual bool get_exist() = 0;
    virtual bool set_exist() = 0;

    virtual ~MapObject() {}
};


#endif