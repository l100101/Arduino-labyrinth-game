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

public:
    Player(int startX, int startY, bool initialFlashlightState, int initialNumberOfKeys);

    void move(int newX, int newY);
    void toggleFlashlight();
    void addKeys(int keysToAdd);
    void getCurrentCoordinates(int& x, int& y) const;
    void getPreviousCoordinates(int& x, int& y) const;
    bool isFlashlightOn() const;
    int getNumberOfKeys() const;
};

#endif // PLAYER_H
