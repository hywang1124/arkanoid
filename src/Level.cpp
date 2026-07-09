#include "Level.h"

Level::Level()
    : currentLevel(1)
{
}

void Level::reset()
{
    currentLevel = 1;
}

bool Level::advance()
{
    if (currentLevel >= totalLevels)
    {
        return false;
    }

    ++currentLevel;
    return true;
}

std::vector<Brick> Level::createBricks(int screenWidth) const
{
    std::vector<Brick> bricks;

    const int rows = 4 + currentLevel;
    const float marginX = 45.0f;
    const float startY = 76.0f;
    const float gap = 8.0f;
    const float brickWidth = (static_cast<float>(screenWidth) - marginX * 2.0f - gap * (brickColumns - 1)) / brickColumns;
    const float brickHeight = 24.0f;

    for (int row = 0; row < rows; ++row)
    {
        for (int column = 0; column < brickColumns; ++column)
        {
            if (!shouldPlaceBrick(row, column))
            {
                continue;
            }

            const float x = marginX + column * (brickWidth + gap);
            const float y = startY + row * (brickHeight + gap);
            const int value = (rows - row) * 10 + currentLevel * 5;
            bricks.emplace_back(x, y, brickWidth, brickHeight, value, row + currentLevel);
        }
    }

    return bricks;
}

int Level::getCurrentLevel() const
{
    return currentLevel;
}

int Level::getTotalLevels() const
{
    return totalLevels;
}

float Level::getBallSpeed() const
{
    return 340.0f + static_cast<float>(currentLevel) * 35.0f;
}

bool Level::isFinalLevel() const
{
    return currentLevel >= totalLevels;
}

bool Level::shouldPlaceBrick(int row, int column) const
{
    if (currentLevel == 1)
    {
        return true;
    }

    if (currentLevel == 2)
    {
        // 偶数行にすき間を作り、ボールの通り道を変える
        return row % 2 == 1 || column % 2 == 0;
    }

    // 最終レベルでは中央を厚くして難度を上げる
    return column >= row % 4 && column < brickColumns - row % 4;
}
