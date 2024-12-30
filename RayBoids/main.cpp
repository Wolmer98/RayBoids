#include <iostream>

#include "raylib.h"
#include "BoidSolver.h"

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Dots");

    SetTargetFPS(300);

    BoidSolver* solver = new BoidSolver();
    solver->Init(100, screenWidth, screenHeight);
    AttractPoint centerPoint = { { screenWidth / 2.0f, screenHeight / 2.0f } };
    solver->AddAttractPoint(centerPoint);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        if (!IsKeyDown(KEY_SPACE))
            solver->Update(GetFrameTime());

        if (IsMouseButtonPressed(0))
        {
            AttractPoint point = { GetMousePosition() };
            solver->AddAttractPoint(point);
        }

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleGradientV(0, 0, screenWidth, screenHeight, DARKGRAY, BLACK);

        solver->RenderAttractPoints();
        solver->RenderBoids();

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