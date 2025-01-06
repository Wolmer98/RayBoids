#pragma once

#include <vector>
#include <array>
#include <queue>

#include "raylib.h"

struct BoidData
{
	Vector2 position;
	Vector2 velocity;
	bool turnedIn;
};

struct AttractPoint
{
	Vector2 position;
	float radius;
	float force;
};

class BoidSolver
{
private:

	std::vector<BoidData> m_boids;
	float m_radius = 20.0f;
	float m_sizeX;
	float m_sizeY;

	static const int GRIDWIDTH = 48;
	static const int GRIDHEIGHT = 32;
	static const int GRIDCELLSIZE = 25;
	std::array<std::array<std::vector<BoidData*>, GRIDHEIGHT>, GRIDWIDTH> m_grid;
	
	std::vector<AttractPoint> m_attractPoints;
	std::vector<AttractPoint> m_turnInPoints;

public:

	void Init(std::size_t numBoids, std::size_t sizeX, std::size_t sizeY);
	void Update(float deltaTime);

	void SetAttractPoints(std::vector<AttractPoint>& points);
	void SetTurnInPoints(std::vector<AttractPoint>& points);

	Vector2 CalculateSeparation(BoidData& inBoid, std::vector<BoidData*>& closeBoids);
	Vector2 CalculateAlignment(BoidData& inBoid, std::vector<BoidData*>& closeBoids);
	Vector2 CalculateCohesion(BoidData& inBoid, std::vector<BoidData*>& closeBoids);
	Vector2 CalculateBorderAvoidance(BoidData& inBoid);
	Vector2 CalculateAttractPoints(BoidData& inBoid);

	bool ShouldBoidTurnIn(BoidData& inBoid);

	void GetCloseBoids(Vector2 position, std::vector<BoidData*>& closeBoids);
	std::vector<BoidData>& GetBoidData() {	return m_boids; };
	std::size_t GetTurnedInNumBoids();

	Vector2 GetGridCoordinate(Vector2 position);
	void BuildGrid(std::array<std::array<std::vector<BoidData*>, GRIDHEIGHT>, GRIDWIDTH>& grid);

	void RenderBoids();
	void RenderAttractPoints();
	void RenderGrid();
	void RenderTestPosition();
};

