#ifndef DOOR_OBJ_H
#define DOOR_OBJ_H

#include "Map_obj.h"
// #include "Player.h"

class DoorMapObject : public MapObject {
private:
    uint8_t x, y;
    bool _exist;
    uint8_t _type;
public:
    DoorMapObject(uint8_t startX, uint8_t startY, bool exists, uint8_t type)
        : x(startX), y(startY), _exist(exists), _type(type) {}


    void collision(Player& player) override {
        // Реализация обработки столкновения с игроком
        // std::cout << "Collision with player!\n";
    }

    uint8_t setX(uint8_t newX) override {
        x = newX;
        return x;
    }

    uint8_t setY(uint8_t newY) override {
        y = newY;
        return y;
    }

    uint8_t getX() override {
        return x;
    }

    uint8_t getY() override {
        return y;
    }

    bool get_exist() override {
        return _exist ? 1 : 0;
    }

    void set_exist(bool newExist) override {
        _exist = newExist;
    }
    uint8_t get_type(){
        return _type;
    }
    ~DoorMapObject() override {
        // Дополнительные действия при уничтожении объекта, если необходимо
    }
};


#endif