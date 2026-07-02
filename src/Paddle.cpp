#include "Paddle.h"

#include <algorithm>

Paddle::Paddle(float x, float y, float width, float height, float speed)
    : position{x, y}, size{width, height}, speed(speed)
{
}

void Paddle::update(float deltaTime, int screenWidth)
{
    float direction = 0.0f;

    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
    {
        direction -= 1.0f;
    }

    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
    {
        direction += 1.0f;
    }

    position.x += direction * speed * deltaTime;

    // パドルが画面外に出ないように制限する
    position.x = std::clamp(position.x, 0.0f, static_cast<float>(screenWidth) - size.x);
}

void Paddle::draw() const
{
    DrawRectangleRounded(getBounds(), 0.35f, 10, Color{120, 196, 255, 255});
    DrawRectangleRoundedLines(getBounds(), 0.35f, 10, Color{220, 245, 255, 255});
}

void Paddle::reset(float x, float y)
{
    position = {x, y};
}

Rectangle Paddle::getBounds() const
{
    return Rectangle{position.x, position.y, size.x, size.y};
}

Vector2 Paddle::getSize() const
{
    return size;
}

