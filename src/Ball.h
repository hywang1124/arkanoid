#ifndef BALL_H
#define BALL_H

#include "raylib.h"

class Ball
{
public:
    Ball(float x, float y, float radius, float speed);

    void update(float deltaTime);
    void draw() const;
    void reset(float x, float y);
    void reverseX();
    void reverseY();
    void normalizeVelocity();
    void multiplySpeed(float factor);
    void setPosition(float x, float y);
    void setVelocity(float x, float y);

    Vector2 getPosition() const;
    Vector2 getVelocity() const;
    float getRadius() const;
    float getSpeed() const;

private:
    Vector2 position;
    Vector2 velocity;
    float radius;
    float speed;
};

#endif
