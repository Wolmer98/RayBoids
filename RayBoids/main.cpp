#include <iostream>

#include "raylib.h"
#include "World.h"
#include "Level.h"
#include "Resources.h"

int main()
{
    const std::size_t screenWidth = 1200;
    const std::size_t screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Fireflies");
    SetTargetFPS(0);

    InitAudioDevice();
    Resources::GetResources()->LoadResources();
    
    PlayMusicStream(Resources::GetResources()->music);

    bool gameFinished = false;
    std::size_t levelIndex = -1;
    std::unique_ptr<World> world = nullptr;

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // -- Update -- //
        UpdateMusicStream(Resources::GetResources()->music);

        if (IsKeyPressed(KEY_R))
            world = Level::LoadLevelByIndex(levelIndex);

        if (!gameFinished)
        {
            // Load next level
            if (world == nullptr || world->GetWorldProgressState() == WorldProgressState::Completed || IsKeyPressed(KEY_N))
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
            DrawText(std::format("FPS: {} \n Num boids: {}", GetFPS(), world->GetNumBoids()).c_str(), 30, 30, 24, RED);

        EndDrawing();
    }

    CloseAudioDevice();
    CloseWindow();
}