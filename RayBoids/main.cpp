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
    solver->Init(3000, screenWidth, screenHeight);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        if (!IsKeyDown(KEY_SPACE))
            solver->Update(GetFrameTime());

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawRectangleGradientV(0, 0, screenWidth, screenHeight, DARKGRAY, DARKGREEN);

        for (auto& boid : solver->GetBoidData())
        {
            DrawCircle(boid.position.x, boid.position.y, 2, WHITE);
            //DrawLine(boid.position.x, boid.position.y, boid.position.x + boid.velocity.x, boid.position.y + boid.velocity.y, DARKBROWN); // Render velocities
        }
        
        DrawCircle(GetMousePosition().x, GetMousePosition().y, 10, BLACK);

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