#include "BoidSolver.h"
#include "raymath.h"

#include <cmath>
#include <algorithm>
#include <iostream>

void BoidSolver::Init(int numBoids, int sizeX, int sizeY)
{
	m_sizeX = sizeX;
	m_sizeY = sizeY;

	for (int i = 0; i < numBoids; ++i)
		m_boids.push_back({{(float)(std::rand() % (sizeX - 50)), (float)(std::rand() % (sizeY - 50))}, {0.0f,0.0f}});
		//m_boids.push_back({{(float)(std::rand() % (sizeX - 50)), (float)(std::rand() % (sizeY - 50))}, {(float)(std::rand() % 100),(float)(std::rand() % 200)}});
}

void BoidSolver::Update(float deltaTime)
{
	auto startTime = std::chrono::system_clock::now();
	BuildGrid(m_grid);
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime);
	//std::cout << std::format("Build Grid: {}\n", duration.count());

	startTime = std::chrono::system_clock::now();
	std::vector<BoidData*> closeBoids;
	for (auto& boid : GetBoidData())
	{
		closeBoids.clear();
		GetCloseBoids(boid.position, closeBoids);

		Vector2 separationResult = CalculateSeparation(boid, closeBoids);
		Vector2 alignmentResult = CalculateAlignment(boid, closeBoids);
		Vector2 cohesionResult = CalculateCohesion(boid, closeBoids);
		Vector2 borderResult = ApplyBorderAvoidance(boid);

		boid.velocity += separationResult + alignmentResult + cohesionResult + borderResult;
		boid.velocity = Vector2ClampValue(boid.velocity, -100, 100);
		//boid.velocity *= 0.98f;
		boid.position += boid.velocity * deltaTime;
	}
	duration = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - startTime);
	std::cout << std::format("Boid solver: {}\n", duration.count());
}

Vector2 BoidSolver::CalculateSeparation(BoidData& inBoid, std::vector<BoidData*>& closeBoids)
{
	Vector2 result = { 0,0 };
	for (auto& boid : closeBoids)
		result += inBoid.position - boid->position;
	return result * 0.07f;
}

Vector2 BoidSolver::CalculateAlignment(BoidData& inBoid, std::vector<BoidData*>& closeBoids)
{
	Vector2 result = { 0,0 };

	for (auto& boid : closeBoids)
		result += boid->velocity;
	result /= (float)closeBoids.size();
	return result * 0.0001f;
}

Vector2 BoidSolver::CalculateCohesion(BoidData& inBoid, std::vector<BoidData*>& closeBoids)
{
	Vector2 result = { 0,0 };
	
	for (auto& boid : closeBoids)
		result += boid->position - inBoid.position;
	result /= (float)closeBoids.size();
	return (result - inBoid.position) * 0.0001f;
}

Vector2 BoidSolver::ApplyBorderAvoidance(BoidData& inBoid)
{
	constexpr float TURNFACTOR = 20.0f;
	constexpr float MARGIN = 50.0f;

	Vector2 result = { 0,0 };
	if (inBoid.position.x < MARGIN)
		result.x += TURNFACTOR;
	if (inBoid.position.x > m_sizeX - MARGIN)
		result.x -= TURNFACTOR;
	if (inBoid.position.y < MARGIN)
		result.y += TURNFACTOR;
	if (inBoid.position.y > m_sizeY - MARGIN)
		result.y -= TURNFACTOR;

	return result;
}

void BoidSolver::GetCloseBoids(Vector2 position, std::vector<BoidData*>& closeBoids)
{
	auto coordinate = GetGridCoordinate(position);
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			int xCoord = std::get<0>(coordinate) + x;
			int yCoord = std::get<1>(coordinate) + y;

			if (xCoord < 0 || yCoord < 0 || xCoord >= GRIDWIDTH || yCoord >= GRIDHEIGHT)
				continue;

			for (auto& boid : m_grid[xCoord][yCoord])
			{
				float distance = Vector2Distance(position, boid->position);
				if (distance > m_radius)
					continue;

				closeBoids.push_back(boid);
			}
		}
	}
}

std::tuple<int, int> BoidSolver::GetGridCoordinate(Vector2 position)
{
	int xCoord = std::clamp((int)(position.x / GRIDCELLSIZE), 0, GRIDWIDTH - 1);
	int yCoord = std::clamp((int)(position.y / GRIDCELLSIZE), 0, GRIDHEIGHT - 1);
	return { xCoord, yCoord };
}

void BoidSolver::BuildGrid(std::array<std::array<std::vector<BoidData*>, GRIDHEIGHT>, GRIDWIDTH>& grid)
{
	// TODO: Remove if grid is moved back to stack mem from heap.
	for (auto& row : grid)
		for (auto& cell : row)
			cell.clear();

	//std::array<std::array<int, GRIDHEIGHT>, GRIDWIDTH> count;
	for (auto& boid : GetBoidData())
	{
		auto coordinate = GetGridCoordinate(boid.position);
		
		//++count[xCoord][yCoord];
		grid[std::get<0>(coordinate)][std::get<1>(coordinate)].push_back(&boid);
	}

	//for (int y = 0; y < GRIDHEIGHT; ++y)
	//{
	//	for (int x = 0; x < GRIDWIDTH; ++x)
	//	{
	//		grid[x][y].push_back()
	//	}
	//}
}

void BoidSolver::RenderGrid()
{
	for (int y = 0; y < GRIDHEIGHT; ++y)
	{
		for (int x = 0; x < GRIDWIDTH; ++x)
		{
			DrawRectangleLines(x * GRIDCELLSIZE, y * GRIDCELLSIZE, GRIDCELLSIZE - 1, GRIDCELLSIZE - 1, m_grid[x][y].size() > 0 ? PURPLE : BLACK);
			DrawText(std::format("{}", m_grid[x][y].size()).c_str(), x * GRIDCELLSIZE, y * GRIDCELLSIZE, 14, DARKGREEN);
		}
	}
}

void BoidSolver::RenderTestPosition()
{
	std::vector<BoidData*> closeBoids;
	Vector2 testPosition = GetBoidData()[0].position;
	GetCloseBoids(testPosition, closeBoids);

	for (auto& boid : closeBoids)
		DrawCircle(boid->position.x, boid->position.y, 2, RED);

	DrawCircle(testPosition.x, testPosition.y, 3, BLACK);
}
