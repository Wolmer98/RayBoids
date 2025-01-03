#pragma once

#include "raylib.h"

class Player
{

private:

	Vector2 m_position;
	float m_movementSpeed = 400.0f;
	float m_playerSize = 30.0f;
	float m_attractRadius = 200.0f;

public:

	const Vector2& GetPosition() { return m_position; };
	const float& GetAttractRadius() { return m_attractRadius; };

	void ProcessInput();
	void Move(Vector2& direction);

	void Render();
};

