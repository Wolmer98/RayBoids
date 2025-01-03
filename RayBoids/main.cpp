#include <iostream>

#include "raylib.h"
#include "BoidSolver.h"
#include "Player.h"

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Fireflies");

    SetTargetFPS(300);

    auto solver = std::make_unique<BoidSolver>();
    solver->Init(1000, screenWidth, screenHeight);
    std::vector<AttractPoint> attractPoints;
    std::vector<AttractPoint> turnInPoints;

    AttractPoint testTurnIn = { screenWidth - 50.0f, screenHeight - 50.0f, 0.0f, 100.0f };
    turnInPoints.push_back(testTurnIn);
    solver->SetTurnInPoints(turnInPoints);

    auto player = std::make_unique<Player>();

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        if (!IsKeyDown(KEY_SPACE))
            solver->Update(GetFrameTime());

        player->ProcessInput();

        AttractPoint centerPoint = { { screenWidth / 4.0f, screenHeight / 4.0f }, 100.0f, 50.0f };
        attractPoints.push_back(centerPoint);

        AttractPoint playerAttractPoint = { player->GetPosition(), 200.0f, player->GetAttractRadius()};
        attractPoints.push_back(playerAttractPoint);

        solver->SetAttractPoints(attractPoints);
        attractPoints.clear();

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleGradientV(0, 0, screenWidth, screenHeight, DARKGRAY, BLACK);

        //solver->RenderAttractPoints();
        solver->RenderBoids();
        player->Render();

        // Debug
        if (IsKeyDown(KEY_G))
            solver->RenderGrid(); 

        if (IsKeyDown(KEY_T))
            solver->RenderTestPosition();

        if (IsKeyDown(KEY_F))
            DrawText(std::format("FPS: {}", GetFPS()).c_str(), 0, 0, 24, RED);

        EndDrawing();
    }

    CloseWindow();
}