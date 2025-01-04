#include "TurnInActor.h"

TurnInActor::TurnInActor(Vector2 InPosition, float InRadius)
{
	m_position = InPosition;
	m_radius = InRadius;
}

void TurnInActor::Render()
{
	//DrawCircle(m_position.x, m_position.y, 500.0f, RED);
	DrawCircleGradient(m_position.x, m_position.y, m_radius + 30.0f, WHITE, { 10, 10, 10, 0 });
}
