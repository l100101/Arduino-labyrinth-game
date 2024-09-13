#ifndef MAP_OBJ_H
#define MAP_OBJ_H
#include "LiquidCrystal_I2C.h"
#include "Player.h"

#define TYPE_DOOR_OBJECT        1
#define TYPE_KEY_OBJECT         2
#define TYPE_FAKE_DOOR_OBJECT   3
#define TYPE_GG_DOOR_OBJECT     4
#define TYPE_RESTART_DOOR_OBJECT 5
#define TYPE_HEART_OBJECT       6
#define TYPE_TRAP_OBJECT        7
#define TYPE_OM_DOOR_OBJECT     8


class MapObject {
  public:
    virtual void collision(Player& player) = 0;
    virtual uint8_t setX(uint8_t x) = 0;
    virtual uint8_t setY(uint8_t y) = 0;
    virtual uint8_t getX() = 0;
    virtual uint8_t getY() = 0;
    virtual void set_exist(bool newExist) = 0;
    virtual bool get_exist() = 0;
    virtual uint8_t get_type() = 0;
    virtual ~MapObject() {}
};


#endif