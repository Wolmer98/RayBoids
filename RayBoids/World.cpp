#include "World.h"

World::World()
{
    m_solver = std::make_unique<BoidSolver>();
    m_solver->Init(1000, GetScreenWidth(), GetScreenHeight());
}

void World::SpawnTurnInActor(Vector2 position, float radius)
{
    m_turnInActors.push_back(std::make_unique<TurnInActor>(position, radius));
}

void World::SpawnAttractorActor(Vector2 position, float radius, float force)
{
    m_attractorActor.push_back(std::make_unique<AttractorActor>(position, radius, force));
}

void World::SpawnPlayer(Vector2 position)
{
    m_player = std::make_unique<PlayerActor>(position);
}

void World::SpawnWall(Vector2 position)
{
    m_wallActors.push_back(std::make_unique<WallActor>(position, 25.0f, 25.0f));
}

void World::Update()
{
    m_player->ProcessInput();

    // TODO: This update is doing a shit-ton of allocations. Maybe optimize later...

    std::vector<Rectangle> blockingBoxes;
    for (auto& wall : m_wallActors)
    {
        Rectangle box = Rectangle{ wall->GetPosition().x, wall->GetPosition().y, wall->GetRadius(), wall->GetRadius() };
        blockingBoxes.push_back(box);
    }
    m_player->TryMove(blockingBoxes);

    std::vector<AttractPoint> attractPoints;
    for (auto& attractorActor : m_attractorActor)
    {
        AttractPoint attractorPoint = { attractorActor->GetPosition(), attractorActor->GetRadius(), attractorActor->GetForce() };
        attractPoints.push_back(attractorPoint);
    }
    for (auto& wallActor : m_wallActors)
    {
        AttractPoint attractorPoint = { wallActor->GetPosition(), wallActor->GetRadius(), -1000.0f };
        attractPoints.push_back(attractorPoint);
    }

    AttractPoint playerAttractPoint = { m_player->GetPosition(), m_player->GetAttractRadius(), 200.0f };
    attractPoints.push_back(playerAttractPoint);
    m_solver->SetAttractPoints(attractPoints);

    std::vector<AttractPoint> turnInPoints;
    for (auto& turnInActor : m_turnInActors)
    {
        AttractPoint turnInPoint = { turnInActor->GetPosition(), turnInActor->GetRadius(), 0.0f };
        turnInPoints.push_back(turnInPoint);
    }
    m_solver->SetTurnInPoints(turnInPoints);

    m_solver->Update(GetFrameTime());
}

void World::Render()
{
    for (auto& turnInActor : m_turnInActors)
        turnInActor->Render();

    for (auto& attractorActor : m_attractorActor)
        attractorActor->Render();

    for (auto& wallActor : m_wallActors)
        wallActor->Render();

    m_solver->RenderBoids();
    m_player->Render();

    // -- Debug -- //
    if (IsKeyDown(KEY_G))
        m_solver->RenderGrid();

    if (IsKeyDown(KEY_T))
        m_solver->RenderTestPosition();
}

WorldProgressState World::GetWorldProgressState()
{
    if (m_solver->GetTurnedInNumBoids() >= m_solver->GetBoidData().size())
        return WorldProgressState::Completed;

    return WorldProgressState::Ongoing;
}
