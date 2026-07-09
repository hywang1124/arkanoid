#ifndef LEVEL_H
#define LEVEL_H

#include "Brick.h"

#include <vector>

class Level
{
public:
    Level();

    void reset();
    bool advance();
    std::vector<Brick> createBricks(int screenWidth) const;

    int getCurrentLevel() const;
    int getTotalLevels() const;
    float getBallSpeed() const;
    bool isFinalLevel() const;

private:
    static constexpr int totalLevels = 3;
    static constexpr int brickColumns = 10;

    int currentLevel;

    bool shouldPlaceBrick(int row, int column) const;
};

#endif
