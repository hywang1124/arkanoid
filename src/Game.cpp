#include "Game.h"

#include "raylib.h"

#include <algorithm>
#include <cmath>

Game::Game()
    : state(GameState::Start),
      paddle(static_cast<float>(screenWidth) / 2.0f - 65.0f, static_cast<float>(screenHeight) - 55.0f, 130.0f, 18.0f, 620.0f),
      ball(static_cast<float>(screenWidth) / 2.0f, static_cast<float>(screenHeight) - 85.0f, 9.0f, 360.0f),
      score(0),
      lives(startingLives)
{
    createBricks();
}

void Game::run()
{
    InitWindow(screenWidth, screenHeight, "Arkanoid C++ Final Project");
    SetTargetFPS(60);

    resetGame();

    while (!WindowShouldClose())
    {
        update();
        draw();
    }

    CloseWindow();
}

void Game::resetGame()
{
    score = 0;
    lives = startingLives;
    state = GameState::Start;
    createBricks();
    resetBallAndPaddle();
}

void Game::resetBallAndPaddle()
{
    paddle.reset(static_cast<float>(screenWidth) / 2.0f - paddle.getSize().x / 2.0f, static_cast<float>(screenHeight) - 55.0f);
    ball.reset(static_cast<float>(screenWidth) / 2.0f, static_cast<float>(screenHeight) - 85.0f);
}

void Game::createBricks()
{
    bricks.clear();

    const float marginX = 45.0f;
    const float startY = 80.0f;
    const float gap = 8.0f;
    const float brickWidth = (static_cast<float>(screenWidth) - marginX * 2.0f - gap * (brickColumns - 1)) / brickColumns;
    const float brickHeight = 26.0f;

    for (int row = 0; row < brickRows; ++row)
    {
        for (int column = 0; column < brickColumns; ++column)
        {
            const float x = marginX + column * (brickWidth + gap);
            const float y = startY + row * (brickHeight + gap);
            const int value = (brickRows - row) * 10;
            bricks.emplace_back(x, y, brickWidth, brickHeight, value, row);
        }
    }
}

void Game::update()
{
    handleInput();

    if (state == GameState::Playing)
    {
        updatePlaying();
    }
}

void Game::updatePlaying()
{
    const float deltaTime = GetFrameTime();

    paddle.update(deltaTime, screenWidth);
    ball.update(deltaTime);
    handleCollisions();

    if (ball.getPosition().y - ball.getRadius() > static_cast<float>(screenHeight))
    {
        --lives;

        if (lives <= 0)
        {
            state = GameState::GameOver;
        }
        else
        {
            resetBallAndPaddle();
            state = GameState::Start;
        }
    }

    if (allBricksDestroyed())
    {
        state = GameState::Win;
    }
}

void Game::handleInput()
{
    if (IsKeyPressed(KEY_SPACE))
    {
        if (state == GameState::Start)
        {
            state = GameState::Playing;
        }
        else if (state == GameState::Win || state == GameState::GameOver)
        {
            resetGame();
            state = GameState::Playing;
        }
    }

    if (IsKeyPressed(KEY_P))
    {
        if (state == GameState::Playing)
        {
            state = GameState::Paused;
        }
        else if (state == GameState::Paused)
        {
            state = GameState::Playing;
        }
    }
}

void Game::handleCollisions()
{
    Vector2 position = ball.getPosition();
    Vector2 velocity = ball.getVelocity();
    const float radius = ball.getRadius();

    if (position.x - radius <= 0.0f && velocity.x < 0.0f)
    {
        ball.reverseX();
    }

    if (position.x + radius >= static_cast<float>(screenWidth) && velocity.x > 0.0f)
    {
        ball.reverseX();
    }

    if (position.y - radius <= 0.0f && velocity.y < 0.0f)
    {
        ball.reverseY();
    }

    if (CheckCollisionCircleRec(ball.getPosition(), radius, paddle.getBounds()) && ball.getVelocity().y > 0.0f)
    {
        const Rectangle bounds = paddle.getBounds();
        const float paddleCenter = bounds.x + bounds.width / 2.0f;
        const float hitRatio = (ball.getPosition().x - paddleCenter) / (bounds.width / 2.0f);
        const float clampedRatio = std::clamp(hitRatio, -1.0f, 1.0f);

        ball.setPosition(ball.getPosition().x, bounds.y - radius - 1.0f);
        ball.setVelocity(clampedRatio * ball.getSpeed(), -std::abs(ball.getVelocity().y));
        ball.normalizeVelocity();
    }

    for (Brick &brick : bricks)
    {
        if (!brick.isActive())
        {
            continue;
        }

        if (CheckCollisionCircleRec(ball.getPosition(), radius, brick.getBounds()))
        {
            brick.hit();
            score += brick.getValue();

            const Rectangle bounds = brick.getBounds();
            const float ballX = ball.getPosition().x;
            const float ballY = ball.getPosition().y;
            const float overlapLeft = std::abs((ballX + radius) - bounds.x);
            const float overlapRight = std::abs((bounds.x + bounds.width) - (ballX - radius));
            const float overlapTop = std::abs((ballY + radius) - bounds.y);
            const float overlapBottom = std::abs((bounds.y + bounds.height) - (ballY - radius));
            const float minOverlap = std::min(std::min(overlapLeft, overlapRight), std::min(overlapTop, overlapBottom));

            if (minOverlap == overlapLeft || minOverlap == overlapRight)
            {
                ball.reverseX();
            }
            else
            {
                ball.reverseY();
            }

            break;
        }
    }
}

void Game::draw() const
{
    BeginDrawing();
    ClearBackground(Color{18, 22, 33, 255});

    for (const Brick &brick : bricks)
    {
        brick.draw();
    }

    paddle.draw();
    ball.draw();
    drawHud();

    if (state == GameState::Start)
    {
        drawCenteredText("ARKANOID", 260, 48, RAYWHITE);
        drawCenteredText("Press SPACE to start", 320, 24, Color{180, 220, 255, 255});
    }
    else if (state == GameState::Paused)
    {
        drawCenteredText("PAUSED", 280, 44, RAYWHITE);
        drawCenteredText("Press P to continue", 335, 24, Color{180, 220, 255, 255});
    }
    else if (state == GameState::Win)
    {
        drawCenteredText("YOU WIN!", 270, 48, Color{116, 241, 164, 255});
        drawCenteredText("Press SPACE to play again", 330, 24, RAYWHITE);
    }
    else if (state == GameState::GameOver)
    {
        drawCenteredText("GAME OVER", 270, 48, Color{255, 107, 107, 255});
        drawCenteredText("Press SPACE to retry", 330, 24, RAYWHITE);
    }

    EndDrawing();
}

void Game::drawHud() const
{
    DrawText(TextFormat("Score: %d", score), 24, 22, 22, RAYWHITE);
    DrawText(TextFormat("Lives: %d", lives), screenWidth - 125, 22, 22, RAYWHITE);
    DrawText("A/D or Arrows: Move   P: Pause   ESC: Quit", 245, screenHeight - 28, 18, Color{160, 170, 190, 255});
}

void Game::drawCenteredText(const char *text, int y, int fontSize, Color color) const
{
    const int textWidth = MeasureText(text, fontSize);
    DrawText(text, screenWidth / 2 - textWidth / 2, y, fontSize, color);
}

bool Game::allBricksDestroyed() const
{
    return std::all_of(bricks.begin(), bricks.end(), [](const Brick &brick) {
        return !brick.isActive();
    });
}

