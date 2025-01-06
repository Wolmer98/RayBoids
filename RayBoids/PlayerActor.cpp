#include "PlayerActor.h"
#include "raymath.h"
#include "Resources.h"

PlayerActor::PlayerActor(Vector2 InPosition)
{
	m_position = InPosition;
}

void PlayerActor::ProcessInput()
{
	Vector2 direction = { 0.0f, 0.0f };
	if (IsKeyDown(KEY_A) || IsKeyDown (KEY_LEFT))
		direction += { -1.0f, 0.0f };
	if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
		direction += { 1.0f, 0.0f };
	if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
		direction += { 0.0f, -1.0f };
	if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
		direction += { 0.0f, 1.0f };

	m_direction = Vector2Normalize(direction);
}

void PlayerActor::TryMove(std::vector<Rectangle>& blockingBoxes)
{
	Vector2 newPosition = m_position + (m_direction * m_movementSpeed * GetFrameTime());
	if (IsPositionOverlapping(newPosition, blockingBoxes)) // TODO: This collision method is prone to get the player stuck. Should replace with something that adjust the position after we've moved instead.
		return;

	float playerHalfSize = m_playerSize * 0.5f;
	m_position = Vector2Clamp(newPosition, { playerHalfSize, playerHalfSize }, { (float)GetScreenWidth() - playerHalfSize, (float)GetScreenHeight() - playerHalfSize});
}

bool PlayerActor::IsPositionOverlapping(Vector2 position, std::vector<Rectangle>& blockingBoxes)
{
	float collisionSize = m_playerSize + 10.0f;
	Rectangle playerBox = { position.x - (collisionSize * 0.5f), position.y - (collisionSize * 0.5f), collisionSize, collisionSize };

	for (auto& box : blockingBoxes)
	{
		if (CheckCollisionRecs(playerBox, box))
			return true;
	}
	return false;
}

void PlayerActor::Render()
{
	DrawCircleGradient(m_position.x, m_position.y, m_attractRadius, {128, 128, 128, 180}, {128, 128, 128, 0});
	DrawTexture(Resources::GetResources()->playerTexture, m_position.x - (m_playerSize * 0.5f), m_position.y - (m_playerSize * 0.5f), WHITE);
	//DrawRectangle(m_position.x - (m_playerSize * 0.5f), m_position.y - (m_playerSize * 0.5f), m_playerSize, m_playerSize, BLUE);
}
