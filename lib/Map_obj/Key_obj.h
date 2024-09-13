#ifndef KEY_OBJ_H
#define KEY_OBJ_H

#include "Map_obj.h"
#include "Player.h"

class KeyMapObject : public MapObject {
private:
    uint8_t x, y;
    bool exist;

public:
    KeyMapObject(uint8_t startX, uint8_t startY, bool exists)
        : x(startX), y(startY), exist(exists) {}

    void draw(LiquidCrystal_I2C& lcd) override {
        // Реализация метода отрисовки объекта
        // std::cout << "Drawing object at (" << static_cast<int>(x) << ", " << static_cast<int>(y) << ")\n";
        // lcd.setCursor(x, y);
        // lcd.write(SKIN_KEY);
    }

    void update(LiquidCrystal_I2C& lcd) override {
        // Реализация метода обновления объекта
        // std::cout << "Updating object at (" << static_cast<int>(x) << ", " << static_cast<int>(y) << ")\n";
    }

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
        return exist ? 1 : 0;
    }

    void set_exist(bool newExist) override {
        exist = newExist;
    }
    uint8_t get_type(){
        return TYPE_KEY_OBJECT;
    }
    ~KeyMapObject() override {
        // Дополнительные действия при уничтожении объекта, если необходимо
    }
};


#endif