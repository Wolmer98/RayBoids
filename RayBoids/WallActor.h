#pragma once
#include "raylib.h"

class WallActor
{
private:

	Vector2 m_position;
	float m_radius;
	float m_size;

public:

	WallActor(Vector2 InPosition, float InRadius, float InForce);

	const Vector2& GetPosition() { return m_position; }
	const float& GetRadius() { return m_radius; }
	const float& GetForce() { return m_size; }

	void Render();
};

