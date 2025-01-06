#include "BoidSolver.h"
#include "raymath.h"

#include <cmath>
#include <algorithm>
#include <iostream>

void BoidSolver::Init(std::size_t numBoids, std::size_t sizeX, std::size_t sizeY)
{
	m_sizeX = sizeX;
	m_sizeY = sizeY;

	m_boids.clear();
	for (std::size_t i = 0; i < numBoids; ++i)
		m_boids.push_back({{(float)(std::rand() % (sizeX - 50)), (float)(std::rand() % (sizeY - 50))}, {0.0f,0.0f}});
		//m_boids.push_back({{(float)(std::rand() % (sizeX - 50)), (float)(std::rand() % (sizeY - 50))}, {(float)(std::rand() % 100),(float)(std::rand() % 200)}});
}

void BoidSolver::Update(float deltaTime)
{
	BuildGrid(m_grid);

	constexpr std::size_t THREADS = 1;
	const std::size_t boidChunkSize = GetBoidData().size() / THREADS;
	for (std::size_t i = 0; i < THREADS; ++i)
	{
		std::size_t startIndex = i * boidChunkSize;
		/*std::jthread worker = std::jthread([&]()
		{*/
			std::vector<BoidData*> closeBoids;
			for (std::size_t boidIndex = startIndex; boidIndex < startIndex + boidChunkSize; ++boidIndex)
			{
				auto& boid = GetBoidData()[boidIndex];

				if (boid.turnedIn)
					continue;

				if (ShouldBoidTurnIn(boid))
				{
					boid.turnedIn = true;
					continue;
				}

				closeBoids.clear();
				GetCloseBoids(boid.position, closeBoids);

				Vector2 separationResult = CalculateSeparation(boid, closeBoids);
				Vector2 alignmentResult = CalculateAlignment(boid, closeBoids);
				Vector2 cohesionResult = CalculateCohesion(boid, closeBoids);
				Vector2 cursorResult = CalculateAttractPoints(boid);
				Vector2 borderResult = CalculateBorderAvoidance(boid);

				boid.velocity += separationResult + alignmentResult + cohesionResult + borderResult + cursorResult;
				boid.velocity = Vector2ClampValue(boid.velocity, -300, 300);
				//boid.velocity *= 0.98f;
				boid.position += boid.velocity * deltaTime;
			}
		//});
	}
}

void BoidSolver::SetAttractPoints(std::vector<AttractPoint>& points)
{
	m_attractPoints = points;
}

void BoidSolver::SetTurnInPoints(std::vector<AttractPoint>& points)
{
	m_turnInPoints = points;
}

Vector2 BoidSolver::CalculateSeparation(BoidData& inBoid, std::vector<BoidData*>& closeBoids)
{
	Vector2 result = { 0,0 };
	for (auto& boid : closeBoids)
		result += inBoid.position - boid->position;
	return result * 0.3f;
}

Vector2 BoidSolver::CalculateAlignment(BoidData& inBoid, std::vector<BoidData*>& closeBoids)
{
	Vector2 result = { 0,0 };

	for (auto& boid : closeBoids)
		result += boid->velocity;
	result /= (float)closeBoids.size();
	return result * 0.001f;
}

Vector2 BoidSolver::CalculateCohesion(BoidData& inBoid, std::vector<BoidData*>& closeBoids)
{
	Vector2 result = { 0,0 };
	
	for (auto& boid : closeBoids)
		result += boid->position - inBoid.position;
	result /= (float)closeBoids.size();
	return (result - inBoid.position) * 0.0001f;
}

Vector2 BoidSolver::CalculateBorderAvoidance(BoidData& inBoid)
{
	constexpr float TURNFACTOR = 20.0f;
	constexpr float MARGIN = 10.0f;

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

Vector2 BoidSolver::CalculateAttractPoints(BoidData& inBoid)
{
	if (m_attractPoints.size() <= 0)
		return { 0,0 };

	Vector2 result = { 0,0 };
	for (auto& point : m_attractPoints)
	{
		Vector2 pointVector = (point.position - inBoid.position);
		if (Vector2Distance(point.position, inBoid.position) - point.radius > m_radius)
			continue;

		result += Vector2Normalize(pointVector) * point.force;
	}

	return result * 0.1f;
}

bool BoidSolver::ShouldBoidTurnIn(BoidData& inBoid)
{
	if (inBoid.position.x < 0 || inBoid.position.y < 0)
		return true;

	for (auto& point : m_turnInPoints)
	{
		if (Vector2Distance(point.position, inBoid.position) - point.radius <= m_radius)
			return true;
	}
	return false;
}

void BoidSolver::GetCloseBoids(Vector2 position, std::vector<BoidData*>& closeBoids)
{
	auto coordinate = GetGridCoordinate(position);
	float radiusSquared = m_radius * m_radius;
	for (int x = -1; x <= 1; ++x)
	{
		for (int y = -1; y <= 1; ++y)
		{
			int xCoord = coordinate.x + x;
			int yCoord = coordinate.y + y;

			if (xCoord < 0 || yCoord < 0 || xCoord >= GRIDWIDTH || yCoord >= GRIDHEIGHT)
				continue;

			for (auto& boid : m_grid[xCoord][yCoord])
			{
				if (boid->turnedIn)
					continue;

				float distance = Vector2DistanceSqr(position, boid->position);
				if (distance > radiusSquared)
					continue;

				closeBoids.push_back(boid);
			}
		}
	}
}

std::size_t BoidSolver::GetTurnedInNumBoids()
{
	std::size_t result = 0;
	for (auto& boid : GetBoidData())
		result += boid.turnedIn;

	return result;
}

Vector2 BoidSolver::GetGridCoordinate(Vector2 position)
{
	float xCoord = std::clamp((int)(position.x / GRIDCELLSIZE), 0, GRIDWIDTH - 1);
	float yCoord = std::clamp((int)(position.y / GRIDCELLSIZE), 0, GRIDHEIGHT - 1);
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
		grid[coordinate.x][coordinate.y].push_back(&boid);
	}

	//for (std::size_t y = 0; y < GRIDHEIGHT; ++y)
	//{
	//	for (std::size_t x = 0; x < GRIDWIDTH; ++x)
	//	{
	//		grid[x][y].push_back()
	//	}
	//}
}

void BoidSolver::RenderBoids()
{
	for (auto& boid : GetBoidData())
	{
		if (boid.turnedIn)
			continue;

		//DrawCircle(boid.position.x, boid.position.y, 1, YELLOW);
		DrawCircleGradient(boid.position.x, boid.position.y, 5.0f, YELLOW, {253 / 2, 249 / 2, 0, 0});
		//DrawLine(boid.position.x, boid.position.y, boid.position.x + boid.velocity.x, boid.position.y + boid.velocity.y, DARKBROWN); // Render velocities
	}
}

void BoidSolver::RenderAttractPoints()
{
	if (m_attractPoints.size() <= 0)
		return;

	for (auto& point : m_attractPoints)
		DrawCircle(point.position.x, point.position.y, 10, RAYWHITE);
}

void BoidSolver::RenderGrid()
{
	for (std::size_t y = 0; y < GRIDHEIGHT; ++y)
	{
		for (std::size_t x = 0; x < GRIDWIDTH; ++x)
		{
			DrawRectangleLines(x * GRIDCELLSIZE, y * GRIDCELLSIZE, GRIDCELLSIZE - 1, GRIDCELLSIZE - 1, m_grid[x][y].size() > 0 ? PURPLE : DARKGRAY);
			DrawText(std::format("{}", m_grid[x][y].size()).c_str(), x * GRIDCELLSIZE, y * GRIDCELLSIZE, 12, LIGHTGRAY);
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
