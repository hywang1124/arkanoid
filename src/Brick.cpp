#include "Brick.h"

Brick::Brick(float x, float y, float width, float height, int value, int colorIndex)
    : bounds{x, y, width, height}, active(true), value(value), color{255, 255, 255, 255}
{
    const Color colors[] = {
        Color{255, 107, 107, 255},
        Color{255, 176, 84, 255},
        Color{255, 236, 153, 255},
        Color{116, 241, 164, 255},
        Color{120, 196, 255, 255},
    };

    color = colors[colorIndex % 5];
}

void Brick::draw() const
{
    if (!active)
    {
        return;
    }

    DrawRectangleRounded(bounds, 0.18f, 8, color);
    DrawRectangleRoundedLines(bounds, 0.18f, 8, Color{255, 255, 255, 90});
}

void Brick::hit()
{
    // ブロックを破壊済みにする
    active = false;
}

Rectangle Brick::getBounds() const
{
    return bounds;
}

bool Brick::isActive() const
{
    return active;
}

int Brick::getValue() const
{
    return value;
}

