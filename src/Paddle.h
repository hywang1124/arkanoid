#ifndef PADDLE_H
#define PADDLE_H

#include "raylib.h"

class Paddle
{
public:
    Paddle(float x, float y, float width, float height, float speed);

    void update(float deltaTime, int screenWidth);
    void draw() const;
    void reset(float x, float y);
    void setWidth(float width, int screenWidth);

    Rectangle getBounds() const;
    Vector2 getSize() const;

private:
    Vector2 position;
    Vector2 size;
    float speed;
};

#endif
