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
} Game;

typedef struct EntityBall {
    Vector2 pos;
    Vector2 dir;

    float speed;
    int radius;

    EdgeCollision edges;
} EntityBall;

typedef struct PlayerPetal {
    Rectangle rect;
    float speed;
} PlayerPetal;

void moveBall(Game *gameInfo, EntityBall *ball) {
    float newX, newY;
    float diffrence = 1.0f;

    newX = (ball->speed * ball->dir.x * GetFrameTime()) + ball->pos.x;
    newY = (ball->speed * ball->dir.y * GetFrameTime()) + ball->pos.y;

    if (newX >= ball->edges.right) {
        ball->dir.x *= -1;
        diffrence = (ball->pos.x - ball->edges.right) / (ball->pos.x - newX);
        newY = ((newY - ball->pos.y) * diffrence) + ball->pos.y;
        newX = ball->edges.right;
    } else if (newX <= ball->edges.left) {
        ball->dir.x *= -1;
        diffrence = (ball->pos.x - ball->edges.left) / (ball->pos.x - newX);
        newY = ((newY - ball->pos.y) * diffrence) + ball->pos.y;
        newX = ball->edges.left;
    } else if (newY >= ball->edges.bottom) {
        ball->dir.y *= -1;
        diffrence = (ball->pos.y - ball->edges.bottom) / (ball->pos.y - newY);
        newX = ((newX - ball->pos.x) * diffrence) + ball->pos.x;
        newY = ball->edges.bottom;
    } else if (newY <= ball->edges.top) {
        ball->dir.y *= -1;
        diffrence = (ball->pos.y - ball->edges.top) / (ball->pos.y - newY);
        newX = ((newX - ball->pos.x) * diffrence) + ball->pos.x;
        newY = ball->edges.top;
    }

    ball->pos.x = newX;
    ball->pos.y = newY;

    // If edge movement is not perceivable, move again
    if (diffrence < 0.5f) {
        moveBall(gameInfo, ball);
    }
}

void movePetal(Game *gameInfo, PlayerPetal *petal, float dir) {
    float yPos = petal->rect.y + petal->speed * dir * GetFrameTime();
    if (yPos < 0) {
        petal->rect.y = 0;
    } else if (yPos > (gameInfo->screenHeight - petal->rect.height)) {
        petal->rect.y = gameInfo->screenHeight - petal->rect.height;
    } else {
        petal->rect.y = yPos;
    }
}

int main(void)
{
    Game gameInfo = {
        800.0f,
        450.0f,
    };

    InitWindow(gameInfo.screenWidth, gameInfo.screenHeight, "My raylib game");

    EntityBall ball = {
        { (float) gameInfo.screenWidth/2, 99.0f },
        {1,1},
        250.0f,
        20,
        {
            ball.radius,
            gameInfo.screenWidth - ball.radius,
            gameInfo.screenHeight - ball.radius,
            ball.radius
        }
    };

    PlayerPetal petal = {
        { 100, gameInfo.screenHeight/2, 15, 100 },
        500
    };

    bool boxCollision = false;
    // int variable = 60;

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        // variable++;
        // SetTargetFPS(variable % 20 + 10);

        // Player movement
        if (IsKeyDown(KEY_J)) movePetal(&gameInfo, &petal, 1);
        if (IsKeyDown(KEY_K)) movePetal(&gameInfo, &petal, -1);
        // Ball movement
        moveBall(&gameInfo, &ball);

        boxCollision = CheckCollisionCircleRec(ball.pos, 50, petal.rect);

        BeginDrawing();
            ClearBackground(RAYWHITE);

            if (boxCollision) {
                DrawText("Collision", GetScreenWidth()/2 - MeasureText("Collision", 20)/2, 10, 20, BLACK);
            }

            DrawRectangleRec(petal.rect, BLUE);
            DrawCircleV(ball.pos, ball.radius, MAROON);
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
