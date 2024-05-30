#include <stdio.h>

#include "raylib.h"

typedef struct EdgeCollision {
    float top;
    float right;
    float bottom;
    float left;
} EdgeCollision;

typedef struct Game {
    float screenWidth;
    float screenHeight;

    float ballSpeed;
    int ballRadius;
    EdgeCollision edges;
} Game;

typedef struct PlayerBall {
    Vector2 pos;
    Vector2 dir;
} PlayerBall;

void moveBall(Game *gameInfo, PlayerBall *ball) {
    float newX, newY;
    float diffrence = 1.0f;

    newX = (gameInfo->ballSpeed * ball->dir.x * GetFrameTime()) + ball->pos.x;
    newY = (gameInfo->ballSpeed * ball->dir.y * GetFrameTime()) + ball->pos.y;

    if (newX >= gameInfo->edges.right) {
        ball->dir.x *= -1;
        diffrence = (ball->pos.x - gameInfo->edges.right) / (ball->pos.x - newX);
        newY = ((newY - ball->pos.y) * diffrence) + ball->pos.y;
        newX = gameInfo->edges.right;
    } else if (newX <= gameInfo->edges.left) {
        ball->dir.x *= -1;
        diffrence = (ball->pos.x - gameInfo->edges.left) / (ball->pos.x - newX);
        newY = ((newY - ball->pos.y) * diffrence) + ball->pos.y;
        newX = gameInfo->edges.left;
    } else if (newY >= gameInfo->edges.bottom) {
        ball->dir.y *= -1;
        diffrence = (ball->pos.y - gameInfo->edges.bottom) / (ball->pos.y - newY);
        newX = ((newX - ball->pos.x) * diffrence) + ball->pos.x;
        newY = gameInfo->edges.bottom;
    } else if (newY <= gameInfo->edges.top) {
        ball->dir.y *= -1;
        diffrence = (ball->pos.y - gameInfo->edges.top) / (ball->pos.y - newY);
        newX = ((newX - ball->pos.x) * diffrence) + ball->pos.x;
        newY = gameInfo->edges.top;
    }

    ball->pos.x = newX;
    ball->pos.y = newY;

    // If edge movement is not perceivable, move again
    if (diffrence < 0.5f) {
        moveBall(gameInfo, ball);
    }
}

int main(void)
{
    Game gameInfo = {
        800.0f,
        450.0f,
        250.0f,
        50,
        {
            gameInfo.ballRadius,
            gameInfo.screenWidth - gameInfo.ballRadius,
            gameInfo.screenHeight - gameInfo.ballRadius,
            gameInfo.ballRadius
        }
    };

    InitWindow(gameInfo.screenWidth, gameInfo.screenHeight, "My raylib game");

    PlayerBall ball = {
        { (float) gameInfo.screenWidth/2, 99.0f },
        {1,1}
    };

    Rectangle box = { gameInfo.screenWidth/2 - 100, gameInfo.screenHeight/2, 15, 50 };

    bool boxCollision = false;
    // int variable = 60;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // variable++;
        // SetTargetFPS(variable % 20 + 10);

        // Input
        // if (IsKeyDown(KEY_SEMICOLON)) ballPos.x += movementSpeed;
        // if (IsKeyDown(KEY_J)) ballPos.x -= movementSpeed;
        // if (IsKeyDown(KEY_K)) ballPos.y += movementSpeed;
        // if (IsKeyDown(KEY_L)) ballPos.y -= movementSpeed;

        if (IsKeyPressed(KEY_J) || IsKeyDown(KEY_K)) {
            moveBall(&gameInfo, &ball);
        }

        boxCollision = CheckCollisionCircleRec(ball.pos, 50, box);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            if (boxCollision) {
                DrawText("Collision", GetScreenWidth()/2 - MeasureText("Collision", 20)/2, 10, 20, BLACK);
            }

            DrawRectangleRec(box, BLUE);
            DrawCircleV(ball.pos, 50, MAROON);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}