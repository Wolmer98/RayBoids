#include "WallActor.h"
#include "Resources.h"

WallActor::WallActor(Vector2 InPosition, float InRadius, float InSize)
{
	m_position = InPosition;
	m_radius = InRadius;
	m_size = InSize;
}

void WallActor::Render()
{
	float halfSize = m_size * 0.5f;
	//DrawRectangle(m_position.x - halfSize, m_position.y - halfSize, m_size, m_size, RAYWHITE);
	DrawTexture(Resources::GetResources()->wallTexture, m_position.x - halfSize, m_position.y - halfSize, WHITE);
}
