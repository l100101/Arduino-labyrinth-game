#ifndef PLAYER_H
#define PLAYER_H

class Player {
private:
    int currentX;
    int currentY;
    int previousX;
    int previousY;
    bool flashlightOn;
    int numberOfKeys;
    int hp;
    int maxHp;
    
public:
    Player(int startX, int startY, bool initialFlashlightState, int initialNumberOfKeys);

    void move(int newX, int newY);
    void toggleFlashlight();
    void addKeys(int keysToAdd);
    void getCurrentCoordinates(int& x, int& y) const;
    void getPreviousCoordinates(int& x, int& y) const;
    bool isFlashlightOn() const;
    int getNumberOfKeys() const;
    int getHp() const;
    void getDamage(int damage);
    void getHeal(int heal);

};

#endif // PLAYER_H
