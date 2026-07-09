#include "Brick.h"

#include <algorithm>

namespace
{
Color fadeColor(Color color, float factor)
{
    return Color{
        static_cast<unsigned char>(std::clamp(static_cast<int>(color.r * factor), 0, 255)),
        static_cast<unsigned char>(std::clamp(static_cast<int>(color.g * factor), 0, 255)),
        static_cast<unsigned char>(std::clamp(static_cast<int>(color.b * factor), 0, 255)),
        color.a};
}
}

Brick::Brick(float x, float y, float width, float height, int value, int colorIndex, int durability)
    : bounds{x, y, width, height},
      active(true),
      value(value),
      color{255, 255, 255, 255},
      durability(std::max(1, durability)),
      maxDurability(std::max(1, durability))
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

    const float damageRatio = static_cast<float>(durability) / static_cast<float>(maxDurability);
    const Color currentColor = fadeColor(color, 0.55f + damageRatio * 0.45f);

    DrawRectangleRounded(bounds, 0.18f, 8, currentColor);
    DrawRectangleRoundedLines(bounds, 0.18f, 8, Color{255, 255, 255, 90});

    if (maxDurability > 1)
    {
        DrawText(TextFormat("%d", durability), static_cast<int>(bounds.x + bounds.width / 2.0f - 4.0f), static_cast<int>(bounds.y + 4.0f), 14, Color{35, 38, 50, 230});
    }
}

bool Brick::hit()
{
    if (!active)
    {
        return false;
    }

    // 耐久度を減らし、0になったら破壊済みにする
    --durability;
    if (durability <= 0)
    {
        active = false;
        return true;
    }

    return false;
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

int Brick::getDurability() const
{
    return durability;
}
