#include <iostream>

#include "raylib.h"
#include "World.h"
#include "Level.h"

int main()
{
    const int screenWidth = 1200;
    const int screenHeight = 800;

    InitWindow(screenWidth, screenHeight, "Fireflies");

    SetTargetFPS(300);

    // TODO: Move to map load functions.
    /*auto world = std::make_unique<World>();
    world->SpawnTurnInActor(Vector2{ screenWidth - 50.0f, screenHeight - 50.0f }, 100.0f);
    world->SpawnAttractorActor(Vector2{ screenWidth / 4.0f, screenHeight / 4.0f }, 50.0f, 100.0f);
    world->SpawnPlayer(Vector2{ 100.0f, 500.0f });
    
    for (int i = 0; i < 10; ++i)
    {
        world->SpawnWall({ 25.0f * i, 25.0f * i });
    }*/
    std::string levelPath = "C:/Users/Arvid/source/repos/RayBoids/RayBoids/Levels/LevelTemplate.txt";
    auto world = Level::LoadLevel(levelPath);

    // Main game loop
    while (!WindowShouldClose()) // Detect window close button or ESC key
    {
        // -- Update -- //
        world->Update();

        // -- Render -- //
        BeginDrawing();
        ClearBackground(BLACK);

        world->Render();

        if (IsKeyDown(KEY_F))
            DrawText(std::format("FPS: {}", GetFPS()).c_str(), 0, 0, 24, RED);

        EndDrawing();
    }

    CloseWindow();
}