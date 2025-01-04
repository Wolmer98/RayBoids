#pragma once
#include "raylib.h"

class TurnInActor
{
private:

	Vector2 m_position;
	float m_radius;

public:

	TurnInActor(Vector2 InPosition, float InRadius);

	const Vector2& GetPosition() { return m_position; }
	const float& GetRadius() { return m_radius; }

	void Render();
};

