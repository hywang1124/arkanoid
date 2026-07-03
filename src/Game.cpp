#include "Game.h"

#include "raylib.h"

#include <algorithm>
#include <cmath>
#include <cctype>
#include <iostream>

namespace
{
constexpr const char *cheatCode = "thereisnospoon";
constexpr float matrixOverlaySeconds = 3.5f;
constexpr float widePaddleSeconds = 12.0f;
constexpr float matrixShieldSeconds = 10.0f;
constexpr int maxBalls = 8;

const char *matrixSpoonArt[] = {
    "        ___________________",
    "     .-'                   '-.",
    "   .'   NEO: there is no      '.",
    "  /          spoon              \\",
    " |                               |",
    " |        O                      |",
    " |       /|\\        ______       |",
    " |       / \\      _/      \\__    |",
    " |              _/   .--.    \\_  |",
    " |            _/    /    \\     ) |",
    " |           /_____/      \\___/  |",
    " |                               |",
    "  \\   010010  MIND OVER METAL   /",
    "   '.                           .'",
    "     '-._____________________.-'",
};

std::string getReadableKeyName(int key)
{
    if (key >= KEY_A && key <= KEY_Z)
    {
        return std::string(1, static_cast<char>('A' + key - KEY_A));
    }

    if (key >= KEY_ZERO && key <= KEY_NINE)
    {
        return std::string(1, static_cast<char>('0' + key - KEY_ZERO));
    }

    if (key >= KEY_F1 && key <= KEY_F12)
    {
        return "F" + std::to_string(key - KEY_F1 + 1);
    }

    if (key >= KEY_KP_0 && key <= KEY_KP_9)
    {
        return "KEYPAD_" + std::to_string(key - KEY_KP_0);
    }

    switch (key)
    {
    case KEY_SPACE:
        return "SPACE";
    case KEY_ESCAPE:
        return "ESCAPE";
    case KEY_ENTER:
        return "ENTER";
    case KEY_TAB:
        return "TAB";
    case KEY_BACKSPACE:
        return "BACKSPACE";
    case KEY_INSERT:
        return "INSERT";
    case KEY_DELETE:
        return "DELETE";
    case KEY_RIGHT:
        return "RIGHT";
    case KEY_LEFT:
        return "LEFT";
    case KEY_DOWN:
        return "DOWN";
    case KEY_UP:
        return "UP";
    case KEY_PAGE_UP:
        return "PAGE_UP";
    case KEY_PAGE_DOWN:
        return "PAGE_DOWN";
    case KEY_HOME:
        return "HOME";
    case KEY_END:
        return "END";
    case KEY_CAPS_LOCK:
        return "CAPS_LOCK";
    case KEY_SCROLL_LOCK:
        return "SCROLL_LOCK";
    case KEY_NUM_LOCK:
        return "NUM_LOCK";
    case KEY_PRINT_SCREEN:
        return "PRINT_SCREEN";
    case KEY_PAUSE:
        return "PAUSE";
    case KEY_LEFT_SHIFT:
        return "LEFT_SHIFT";
    case KEY_LEFT_CONTROL:
        return "LEFT_CONTROL";
    case KEY_LEFT_ALT:
        return "LEFT_ALT";
    case KEY_LEFT_SUPER:
        return "LEFT_SUPER";
    case KEY_RIGHT_SHIFT:
        return "RIGHT_SHIFT";
    case KEY_RIGHT_CONTROL:
        return "RIGHT_CONTROL";
    case KEY_RIGHT_ALT:
        return "RIGHT_ALT";
    case KEY_RIGHT_SUPER:
        return "RIGHT_SUPER";
    case KEY_MENU:
        return "MENU";
    case KEY_APOSTROPHE:
        return "APOSTROPHE";
    case KEY_COMMA:
        return "COMMA";
    case KEY_MINUS:
        return "MINUS";
    case KEY_PERIOD:
        return "PERIOD";
    case KEY_SLASH:
        return "SLASH";
    case KEY_SEMICOLON:
        return "SEMICOLON";
    case KEY_EQUAL:
        return "EQUAL";
    case KEY_LEFT_BRACKET:
        return "LEFT_BRACKET";
    case KEY_BACKSLASH:
        return "BACKSLASH";
    case KEY_RIGHT_BRACKET:
        return "RIGHT_BRACKET";
    case KEY_GRAVE:
        return "GRAVE";
    case KEY_KP_DECIMAL:
        return "KEYPAD_DECIMAL";
    case KEY_KP_DIVIDE:
        return "KEYPAD_DIVIDE";
    case KEY_KP_MULTIPLY:
        return "KEYPAD_MULTIPLY";
    case KEY_KP_SUBTRACT:
        return "KEYPAD_SUBTRACT";
    case KEY_KP_ADD:
        return "KEYPAD_ADD";
    case KEY_KP_ENTER:
        return "KEYPAD_ENTER";
    case KEY_KP_EQUAL:
        return "KEYPAD_EQUAL";
    default:
        return "KEY_" + std::to_string(key);
    }
}
}

Game::Game()
    : state(GameState::Start),
      paddle(static_cast<float>(screenWidth) / 2.0f - 65.0f, static_cast<float>(screenHeight) - 55.0f, 130.0f, 18.0f, 620.0f),
      score(0),
      lives(startingLives),
      cheatsUnlocked(false),
      matrixOverlayTimer(0.0f),
      widePaddleTimer(0.0f),
      matrixShieldTimer(0.0f)
{
    createBricks();
    resetBallAndPaddle();
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
    cheatsUnlocked = false;
    cheatBuffer.clear();
    matrixOverlayTimer = 0.0f;
    widePaddleTimer = 0.0f;
    matrixShieldTimer = 0.0f;
    createBricks();
    resetBallAndPaddle();
}

void Game::resetBallAndPaddle()
{
    paddle.reset(static_cast<float>(screenWidth) / 2.0f - paddle.getSize().x / 2.0f, static_cast<float>(screenHeight) - 55.0f);
    if (widePaddleTimer > 0.0f)
    {
        paddle.setWidth(220.0f, screenWidth);
    }
    balls.clear();
    spawnBall(static_cast<float>(screenWidth) / 2.0f, static_cast<float>(screenHeight) - 85.0f, 0.55f);
}

void Game::spawnBall(float x, float y, float directionX)
{
    if (static_cast<int>(balls.size()) >= maxBalls)
    {
        return;
    }

    balls.emplace_back(x, y, 9.0f, 360.0f);
    balls.back().setVelocity(360.0f * directionX, -360.0f);
    balls.back().normalizeVelocity();
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
    const float deltaTime = GetFrameTime();

    handleInput();

    if (matrixOverlayTimer > 0.0f)
    {
        matrixOverlayTimer = std::max(0.0f, matrixOverlayTimer - deltaTime);
    }

    if (widePaddleTimer > 0.0f)
    {
        widePaddleTimer = std::max(0.0f, widePaddleTimer - deltaTime);
        if (widePaddleTimer == 0.0f)
        {
            paddle.setWidth(130.0f, screenWidth);
        }
    }

    if (matrixShieldTimer > 0.0f)
    {
        matrixShieldTimer = std::max(0.0f, matrixShieldTimer - deltaTime);
    }

    if (state == GameState::Playing)
    {
        updatePlaying();
    }
}

void Game::updatePlaying()
{
    const float deltaTime = GetFrameTime();

    paddle.update(deltaTime, screenWidth);
    for (Ball &activeBall : balls)
    {
        activeBall.update(deltaTime);
        handleCollisions(activeBall);
    }

    for (Ball &activeBall : balls)
    {
        if (matrixShieldTimer > 0.0f && activeBall.getPosition().y + activeBall.getRadius() >= static_cast<float>(screenHeight))
        {
            activeBall.setPosition(activeBall.getPosition().x, static_cast<float>(screenHeight) - activeBall.getRadius() - 1.0f);
            activeBall.reverseY();
        }
    }

    balls.erase(std::remove_if(balls.begin(), balls.end(), [](const Ball &activeBall) {
                    return activeBall.getPosition().y - activeBall.getRadius() > static_cast<float>(screenHeight);
                }),
                balls.end());

    if (balls.empty())
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
    logPressedKeys();
    handleCheatCodeInput();
    handleCheatHotkeys();

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

void Game::logPressedKeys() const
{
    for (int key = KEY_SPACE; key <= KEY_KB_MENU; ++key)
    {
        if (IsKeyPressed(key))
        {
            std::cout << "[Input] Key pressed: " << getReadableKeyName(key) << " (" << key << ")" << std::endl;
        }
    }
}

void Game::handleCheatCodeInput()
{
    int key = GetCharPressed();
    while (key > 0)
    {
        if (std::isalpha(key))
        {
            cheatBuffer.push_back(static_cast<char>(std::tolower(key)));

            if (cheatBuffer.size() > std::char_traits<char>::length(cheatCode))
            {
                cheatBuffer.erase(cheatBuffer.begin());
            }

            if (cheatBuffer == cheatCode)
            {
                cheatsUnlocked = true;
                matrixOverlayTimer = matrixOverlaySeconds;
                cheatBuffer.clear();
            }
        }

        key = GetCharPressed();
    }
}

void Game::handleCheatHotkeys()
{
    if (!cheatsUnlocked)
    {
        return;
    }

    if (IsKeyPressed(KEY_ONE))
    {
        for (Ball &activeBall : balls)
        {
            activeBall.multiplySpeed(1.18f);
        }
    }

    if (IsKeyPressed(KEY_TWO) && !balls.empty())
    {
        const Ball &sourceBall = balls.front();
        const float directionX = balls.size() % 2 == 0 ? -0.75f : 0.75f;
        spawnBall(sourceBall.getPosition().x, sourceBall.getPosition().y, directionX);
    }

    if (IsKeyPressed(KEY_THREE))
    {
        widePaddleTimer = widePaddleSeconds;
        paddle.setWidth(220.0f, screenWidth);
    }

    if (IsKeyPressed(KEY_FOUR))
    {
        ++lives;
    }

    if (IsKeyPressed(KEY_FIVE))
    {
        matrixShieldTimer = matrixShieldSeconds;
    }

    if (IsKeyPressed(KEY_SIX))
    {
        applySpoonWave();
    }
}

void Game::handleCollisions(Ball &activeBall)
{
    Vector2 position = activeBall.getPosition();
    Vector2 velocity = activeBall.getVelocity();
    const float radius = activeBall.getRadius();

    if (position.x - radius <= 0.0f && velocity.x < 0.0f)
    {
        activeBall.reverseX();
    }

    if (position.x + radius >= static_cast<float>(screenWidth) && velocity.x > 0.0f)
    {
        activeBall.reverseX();
    }

    if (position.y - radius <= 0.0f && velocity.y < 0.0f)
    {
        activeBall.reverseY();
    }

    if (CheckCollisionCircleRec(activeBall.getPosition(), radius, paddle.getBounds()) && activeBall.getVelocity().y > 0.0f)
    {
        const Rectangle bounds = paddle.getBounds();
        const float paddleCenter = bounds.x + bounds.width / 2.0f;
        const float hitRatio = (activeBall.getPosition().x - paddleCenter) / (bounds.width / 2.0f);
        const float clampedRatio = std::clamp(hitRatio, -1.0f, 1.0f);

        activeBall.setPosition(activeBall.getPosition().x, bounds.y - radius - 1.0f);
        activeBall.setVelocity(clampedRatio * activeBall.getSpeed(), -std::abs(activeBall.getVelocity().y));
        activeBall.normalizeVelocity();
    }

    for (Brick &brick : bricks)
    {
        if (!brick.isActive())
        {
            continue;
        }

        if (CheckCollisionCircleRec(activeBall.getPosition(), radius, brick.getBounds()))
        {
            brick.hit();
            score += brick.getValue();

            const Rectangle bounds = brick.getBounds();
            const float ballX = activeBall.getPosition().x;
            const float ballY = activeBall.getPosition().y;
            const float overlapLeft = std::abs((ballX + radius) - bounds.x);
            const float overlapRight = std::abs((bounds.x + bounds.width) - (ballX - radius));
            const float overlapTop = std::abs((ballY + radius) - bounds.y);
            const float overlapBottom = std::abs((bounds.y + bounds.height) - (ballY - radius));
            const float minOverlap = std::min(std::min(overlapLeft, overlapRight), std::min(overlapTop, overlapBottom));

            if (minOverlap == overlapLeft || minOverlap == overlapRight)
            {
                activeBall.reverseX();
            }
            else
            {
                activeBall.reverseY();
            }

            break;
        }
    }
}

void Game::applySpoonWave()
{
    int destroyedCount = 0;

    for (Brick &brick : bricks)
    {
        if (brick.isActive())
        {
            brick.hit();
            score += brick.getValue();
            ++destroyedCount;
        }

        if (destroyedCount >= 5)
        {
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
    for (const Ball &activeBall : balls)
    {
        activeBall.draw();
    }
    drawHud();
    drawCheatPanel();

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

    if (matrixOverlayTimer > 0.0f)
    {
        drawMatrixOverlay();
    }

    EndDrawing();
}

void Game::drawHud() const
{
    DrawText(TextFormat("Score: %d", score), 24, 22, 22, RAYWHITE);
    DrawText(TextFormat("Lives: %d", lives), screenWidth - 125, 22, 22, RAYWHITE);
    DrawText("A/D or Arrows: Move   P: Pause   ESC: Quit   Code: thereisnospoon", 160, screenHeight - 28, 18, Color{160, 170, 190, 255});
}

void Game::drawCheatPanel() const
{
    if (!cheatsUnlocked)
    {
        return;
    }

    DrawRectangle(18, 50, 864, 34, Color{5, 18, 12, 210});
    DrawRectangleLines(18, 50, 864, 34, Color{95, 255, 145, 160});
    DrawText("CHEATS: 1 Speed+  2 Spawn Ball  3 Wide Paddle  4 +Life  5 Matrix Shield  6 Spoon Wave", 30, 59, 16, Color{140, 255, 170, 255});

    if (widePaddleTimer > 0.0f)
    {
        DrawText(TextFormat("Wide %.0fs", widePaddleTimer), 30, 90, 16, Color{180, 255, 190, 255});
    }

    if (matrixShieldTimer > 0.0f)
    {
        DrawText(TextFormat("Shield %.0fs", matrixShieldTimer), 125, 90, 16, Color{180, 255, 190, 255});
    }
}

void Game::drawMatrixOverlay() const
{
    DrawRectangle(0, 0, screenWidth, screenHeight, Color{0, 0, 0, 230});

    const int fontSize = 18;
    const int lineHeight = 22;
    const int lineCount = static_cast<int>(sizeof(matrixSpoonArt) / sizeof(matrixSpoonArt[0]));
    const int startY = screenHeight / 2 - lineCount * lineHeight / 2;

    for (int i = 0; i < lineCount; ++i)
    {
        const char *line = matrixSpoonArt[i];
        const int x = screenWidth / 2 - MeasureText(line, fontSize) / 2;
        DrawText(line, x, startY + i * lineHeight, fontSize, Color{95, 255, 145, 255});
    }

    drawCenteredText("CHEAT SYSTEM UNLOCKED", startY + lineCount * lineHeight + 18, 24, Color{180, 255, 190, 255});
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
