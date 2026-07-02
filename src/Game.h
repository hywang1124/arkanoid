#ifndef GAME_H
#define GAME_H

#include "Ball.h"
#include "Brick.h"
#include "Paddle.h"

#include <vector>

enum class GameState
{
    Start,
    Playing,
    Paused,
    Win,
    GameOver
};

class Game
{
public:
    Game();
    void run();

private:
    static constexpr int screenWidth = 900;
    static constexpr int screenHeight = 650;
    static constexpr int brickRows = 5;
    static constexpr int brickColumns = 10;
    static constexpr int startingLives = 3;

    GameState state;
    Paddle paddle;
    Ball ball;
    std::vector<Brick> bricks;
    int score;
    int lives;

    void resetGame();
    void resetBallAndPaddle();
    void createBricks();
    void update();
    void updatePlaying();
    void handleInput();
    void handleCollisions();
    void draw() const;
    void drawHud() const;
    void drawCenteredText(const char *text, int y, int fontSize, Color color) const;
    bool allBricksDestroyed() const;
};

#endif

