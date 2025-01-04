#include "Level.h"
#include <iostream>
#include <fstream>

std::unique_ptr<World> Level::LoadLevelByIndex(int levelIndex)
{
    std::string levelPath = std::format("Levels/Level{}.txt", levelIndex);
    return Level::LoadLevel(levelPath);
}

std::unique_ptr<World> Level::LoadLevel(std::string& path)
{
    std::ifstream file(path);
    if (!file.is_open())
        return nullptr;

    std::string line;
    std::unique_ptr<World> world = std::make_unique<World>();

    float cellSize = 25.0f; // TODO

    int row = 0;
    while (std::getline(file, line))
    {
        int column = 0;
        for (auto c : line)
        {
            Vector2 position = { cellSize * column, cellSize * row };
            if (c == '#')
                world->SpawnWall(position);
            if (c == 'P')
                world->SpawnPlayer(position);
            if (c == 'T')
                world->SpawnTurnInActor(position, 100.0f);
            if (c == 'A')
                world->SpawnAttractorActor(position, 100.0f, 60.0f);

            ++column;
        }
        ++row;
    }
    return world;
}
