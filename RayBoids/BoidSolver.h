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

	static const int GRIDWIDTH = 48;
	static const int GRIDHEIGHT = 32;
	static const int GRIDCELLSIZE = 25;

	std::array<std::array<std::vector<BoidData*>, GRIDHEIGHT>, GRIDWIDTH> m_grid;

public:

	void Init(int numBoids, int sizeX, int sizeY);
	void Update(float deltaTime);

	Vector2 CalculateSeparation(BoidData& inBoid, std::vector<BoidData*>& closeBoids);
	Vector2 CalculateAlignment(BoidData& inBoid, std::vector<BoidData*>& closeBoids);
	Vector2 CalculateCohesion(BoidData& inBoid, std::vector<BoidData*>& closeBoids);
	Vector2 CalculateBorderAvoidance(BoidData& inBoid);
	Vector2 CalculateCursorAttract(BoidData& inBoid);

	void GetCloseBoids(Vector2 position, std::vector<BoidData*>& closeBoids);
	std::vector<BoidData>& GetBoidData() {	return m_boids; };

	Vector2 GetGridCoordinate(Vector2 position);
	void BuildGrid(std::array<std::array<std::vector<BoidData*>, GRIDHEIGHT>, GRIDWIDTH>& grid);

	void RenderGrid();
	void RenderTestPosition();
};

