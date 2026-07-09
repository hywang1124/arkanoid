#ifndef GAME_H
#define GAME_H

#include "Ball.h"
#include "Brick.h"
#include "Level.h"
#include "Paddle.h"

#include <string>
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
    static constexpr int startingLives = 3;

    GameState state;
    Level level;
    Paddle paddle;
    std::vector<Ball> balls;
    std::vector<Brick> bricks;
    std::string cheatBuffer;
    int score;
    int lives;
    bool cheatsUnlocked;
    float matrixOverlayTimer;
    float widePaddleTimer;
    float matrixShieldTimer;

    void resetGame();
    void resetBallAndPaddle();
    void spawnBall(float x, float y, float directionX);
    void createBricks();
    void advanceLevelOrWin();
    void update();
    void updatePlaying();
    void handleInput();
    void logPressedKeys() const;
    void handleCheatCodeInput();
    void handleCheatHotkeys();
    void handleCollisions(Ball &activeBall);
    void applySpoonWave();
    void draw() const;
    void drawHud() const;
    void drawCheatPanel() const;
    void drawMatrixOverlay() const;
    void drawCenteredText(const char *text, int y, int fontSize, Color color) const;
    bool allBricksDestroyed() const;
};

#endif
