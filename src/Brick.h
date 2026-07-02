#ifndef BRICK_H
#define BRICK_H

#include "raylib.h"

class Brick
{
public:
    Brick(float x, float y, float width, float height, int value, int colorIndex);

    void draw() const;
    void hit();

    Rectangle getBounds() const;
    bool isActive() const;
    int getValue() const;

private:
    Rectangle bounds;
    bool active;
    int value;
    Color color;
};

#endif

