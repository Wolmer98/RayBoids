#pragma once

#include <vector>
#include <array>

#include "raylib.h"

struct BoidData
{
	Vector2 position;
	Vector2 velocity;
};

class BoidSolver
{
private:

	std::vector<BoidData> m_boids;
	float m_radius = 20.0f;
	float m_sizeX;
	float m_sizeY;

	static const int GRIDWIDTH = 16;
	static const int GRIDHEIGHT = 9;
	static const int GRIDCELLSIZE = 50;

	std::array<std::array<std::vector<BoidData*>, GRIDHEIGHT>, GRIDWIDTH> m_grid;

public:

	void Init(int numBoids, int sizeX, int sizeY);
	void Update(float deltaTime);

	Vector2 CalculateSeparation(BoidData& inBoid, std::vector<BoidData*>& closeBoids);
	Vector2 CalculateAlignment(BoidData& inBoid, std::vector<BoidData*>& closeBoids);
	Vector2 CalculateCohesion(BoidData& inBoid, std::vector<BoidData*>& closeBoids);
	Vector2 ApplyBorderAvoidance(BoidData& inBoid);

	void GetCloseBoids(Vector2 position, std::vector<BoidData*>& closeBoids);
	std::vector<BoidData>& GetBoidData() {	return m_boids; };

	std::tuple<int, int> GetGridCoordinate(Vector2 position);
	void BuildGrid(std::array<std::array<std::vector<BoidData*>, GRIDHEIGHT>, GRIDWIDTH>& grid);

	void RenderGrid();
	void RenderTestPosition();
};

