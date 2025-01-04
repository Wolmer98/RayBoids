#include "AttractorActor.h"

AttractorActor::AttractorActor(Vector2 InPosition, float InRadius, float InForce)
{
	m_position = InPosition;
	m_radius = InRadius;
	m_force = InForce;
}

void AttractorActor::Render()
{
	DrawCircleGradient(m_position.x, m_position.y, m_radius, { 255, 128, 128, 128 }, { 128, 128, 128, 0 });
}
