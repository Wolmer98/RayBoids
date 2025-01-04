#pragma once

#include "raylib.h"
#include <vector>

class PlayerActor
{

private:

	Vector2 m_position;
	float m_movementSpeed = 400.0f;
	float m_playerSize = 30.0f;
	float m_attractRadius = 200.0f;

	Vector2 m_direction;

public:

	PlayerActor(Vector2 InPosition);

	const Vector2& GetPosition() { return m_position; };
	const float& GetAttractRadius() { return m_attractRadius; };

	void ProcessInput();
	void TryMove(std::vector<Rectangle>& blockingBoxes);

	bool IsPositionOverlapping(Vector2 position, std::vector<Rectangle>& blockingBoxes);

	void Render();
};

