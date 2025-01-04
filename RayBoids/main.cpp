#include <iostream>

#include "raylib.h"
#include "World.h"
#include "Level.h"
#include "Resources.h"

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Fireflies");
    SetTargetFPS(300);

    Resources::GetResources()->LoadResources();
    
    bool gameFinished = false;
    int levelIndex = -1;
    std::unique_ptr<World> world = nullptr;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // -- Update -- //
        if (!gameFinished)
        {
            // Load next level
            if (world == nullptr || world->GetWorldProgressState() == WorldProgressState::Completed)
            {
                ++levelIndex;
                world = Level::LoadLevelByIndex(levelIndex);
                if (world == nullptr)
                {
                    gameFinished = true;
                    continue;
                }
            }
            world->Update();
        }

        // -- Render -- //
        BeginDrawing();
        ClearBackground(BLACK);
        DrawTexture(Resources::GetResources()->backgroundTexture, 0, 0, {130,130,130,255});

        if (!gameFinished)
            world->Render();
        else
        {
            DrawText("GAME FINISHED", screenWidth / 4.5f, screenHeight / 2.5, 80, RAYWHITE);
        }

        if (IsKeyDown(KEY_F))
            DrawText(std::format("FPS: {}", GetFPS()).c_str(), 0, 0, 24, RED);

        EndDrawing();
    }

    CloseWindow();
}