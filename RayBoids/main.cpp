#include <iostream>

#include "raylib.h"
#include "BoidSolver.h"

int main()
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - basic window");

    SetTargetFPS(120);

    BoidSolver* solver = new BoidSolver();
    solver->Init(3000, screenWidth, screenHeight);

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        solver->Update(GetFrameTime());

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        //solver->RenderGrid(); // Debug

        for (auto& boid : solver->GetBoidData())
        {
            DrawCircle(boid.position.x, boid.position.y, 2, BLUE);
            //DrawLine(boid.position.x, boid.position.y, boid.position.x + boid.velocity.x, boid.position.y + boid.velocity.y, DARKBROWN); // Render velocities
        }
        
        //solver->RenderTestPosition();

        DrawText(std::format("FPS: {}", GetFPS()).c_str(), 0, 0, 10, RED);
        EndDrawing();
    }

    CloseWindow();
}