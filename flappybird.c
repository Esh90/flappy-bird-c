#include <raylib.h>
#include <stdlib.h>
#include <time.h>

#define xSize       32 
#define ySize       16 
#define pipeCount   3
#define qKey        KEY_Q
#define PIPE_WIDTH  55
#define PIPE_SPACING 200

typedef struct
{
    int x;
    int y;
} PIX;

PIX bird;
PIX pipes[3];

int score = 0;
int gameOver = 0; // Variable to track game over state

Texture2D birdTexture;   // Texture for the bird
Texture2D backgroundTexture; // Texture for the background

void Draw();
void Pipes();
void HitTest();
void DrawScore();
void DrawGameOverScreen();
void SetGameOver(int value);
void RestartGame();

void DrawScore()
{
    DrawText(TextFormat("Score: %d", score), 10, 10, 20, RED);
}

void DrawGameOverScreen()
{
    DrawText("Game Over", GetScreenWidth() / 2 - MeasureText("Game Over", 40) / 2, GetScreenHeight() / 2 - 40, 40, RED);
    DrawText(TextFormat("Final Score: %d", score), GetScreenWidth() / 2 - MeasureText(TextFormat("Final Score: %d", score), 20) / 2, GetScreenHeight() / 2, 20, RED);
    DrawText("Press 'R' to play again", GetScreenWidth() / 2 - MeasureText("Press 'R' to play again", 20) / 2, GetScreenHeight() / 2 + 40, 20, RED);
}

void Draw()
{
    BeginDrawing();

    // Draw the background first
    DrawTexture(backgroundTexture, 0, 0, WHITE);

    if (!gameOver)
    {
        for (int i = 0; i < pipeCount; i++)
        {
            DrawRectangle(pipes[i].x, 0, PIPE_WIDTH, pipes[i].y, GREEN);
            DrawRectangle(pipes[i].x, pipes[i].y + PIPE_SPACING, PIPE_WIDTH, GetScreenHeight() - pipes[i].y - PIPE_SPACING, GREEN);
        }

        // Draw the bird using the birdTexture
        DrawTexture(birdTexture, bird.x, bird.y, WHITE);

        DrawScore();
    }
    else
    {
        DrawGameOverScreen();
    }

    EndDrawing();
}

void Pipes()
{
    for (int i = 0; i < pipeCount; i++)
    {
        if (pipes[i].x <= -PIPE_WIDTH)
        {
            pipes[i].x = pipes[(i + pipeCount - 1) % pipeCount].x + PIPE_SPACING;
            pipes[i].y = (rand() % (GetScreenHeight() - PIPE_SPACING - 10)) + 5;
            score++; // Increment score when a new pipe is generated
        }
    }
}

void HitTest()
{
    if (bird.y >= GetScreenHeight() - 20 || bird.y <= 0)
    {
        // Game over
        SetGameOver(1);
    }

    for (int i = 0; i < pipeCount; i++)
    {
        if ((bird.x + 30 > pipes[i].x && bird.x < pipes[i].x + PIPE_WIDTH) &&
            (bird.y < pipes[i].y || bird.y + 20 > pipes[i].y + PIPE_SPACING))
        {
            // Game over
            SetGameOver(1);
        }
    }
}

void SetGameOver(int value)
{
    gameOver = value;
}

void RestartGame()
{
    // Reset game state
    bird.y = 200;
    score = 0;
    gameOver = 0;

    // Reset pipes
    for (int i = 0; i < pipeCount; i++)
    {
        pipes[i].x = 400 + i * PIPE_SPACING;
        pipes[i].y = (rand() % 7) + 5;
    }
}

int main()
{
    srand(time(NULL));
    const int screenWidth = 1500;
    const int screenHeight = 900;

    InitWindow(screenWidth, screenHeight, "Flappy Bird");

    bird.x = 100;
    bird.y = 200;

    // Load bird texture from image file
    birdTexture = LoadTexture("C:/Users/BEST BUY COMPUTERS/Documents/Raylib darling/bird.png");

    // Load background texture from image file
    backgroundTexture = LoadTexture("C:/Users/BEST BUY COMPUTERS/Documents/Raylib darling/background.png");

    for (int i = 0; i < pipeCount; i++)
    {
        pipes[i].x = 400 + i * PIPE_SPACING;
        pipes[i].y = (rand() % 7) + 5;
    }

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        if (!gameOver)
        {
            if (IsKeyDown(KEY_UP))
            {
                bird.y -= 5;
            }
            else if (IsKeyDown(KEY_DOWN))
            {
                bird.y += 5;
            }

            bird.y++;

            for (int i = 0; i < 3; i++)
            {
                pipes[i].x--;
            }

            HitTest();
            Pipes();
            Draw();
        }
        else
        {
            // If game over, display the game over screen
            Draw();

            if (IsKeyDown(KEY_R))
            {
                RestartGame();
            }
        }
    }

    // Unload textures when no longer needed
    UnloadTexture(birdTexture);
    UnloadTexture(backgroundTexture);

    CloseWindow();

    return 0;
}