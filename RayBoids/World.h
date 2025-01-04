#pragma once

#include <memory>
#include <vector>
#include "raylib.h"

#include "TurnInActor.h"
#include "AttractorActor.h"
#include "PlayerActor.h"
#include "BoidSolver.h"
#include "WallActor.h"

enum WorldProgressState
{
    Ongoing,
    Completed,
    Failed
};

class World
{
private:

    std::vector<std::unique_ptr<TurnInActor>> m_turnInActors;
    std::vector<std::unique_ptr<AttractorActor>> m_attractorActor;
    std::vector<std::unique_ptr<WallActor>> m_wallActors;
    std::unique_ptr<PlayerActor> m_player;
    std::unique_ptr<BoidSolver> m_solver;

public:

    World();

    void SpawnTurnInActor(Vector2 position, float radius);
    void SpawnAttractorActor(Vector2 position, float radius, float force);
    void SpawnPlayer(Vector2 position);
    void SpawnWall(Vector2 position);

    void Update();
    void Render();

    
    WorldProgressState GetWorldProgressState();
};

