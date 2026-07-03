#include "Ball.h"

#include <cmath>

Ball::Ball(float x, float y, float radius, float speed)
    : position{x, y}, velocity{speed * 0.55f, -speed}, radius(radius), speed(speed)
{
    normalizeVelocity();
}

void Ball::update(float deltaTime)
{
    // ボールの位置を更新する
    position.x += velocity.x * deltaTime;
    position.y += velocity.y * deltaTime;
}

void Ball::draw() const
{
    DrawCircleV(position, radius, Color{255, 236, 153, 255});
    DrawCircleLines(static_cast<int>(position.x), static_cast<int>(position.y), radius, Color{255, 255, 230, 255});
}

void Ball::reset(float x, float y)
{
    position = {x, y};
    velocity = {speed * 0.55f, -speed};
    normalizeVelocity();
}

void Ball::reverseX()
{
    velocity.x *= -1.0f;
}

void Ball::reverseY()
{
    velocity.y *= -1.0f;
}

void Ball::normalizeVelocity()
{
    const float length = std::sqrt(velocity.x * velocity.x + velocity.y * velocity.y);

    if (length == 0.0f)
    {
        velocity = {speed * 0.55f, -speed};
        return;
    }

    velocity.x = velocity.x / length * speed;
    velocity.y = velocity.y / length * speed;
}

void Ball::multiplySpeed(float factor)
{
    // チート効果としてボールの速度を変更する
    speed *= factor;
    normalizeVelocity();
}

void Ball::setPosition(float x, float y)
{
    position = {x, y};
}

void Ball::setVelocity(float x, float y)
{
    velocity = {x, y};
}

Vector2 Ball::getPosition() const
{
    return position;
}

Vector2 Ball::getVelocity() const
{
    return velocity;
}

float Ball::getRadius() const
{
    return radius;
}

float Ball::getSpeed() const
{
    return speed;
}
