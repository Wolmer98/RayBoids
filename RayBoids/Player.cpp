#include "Player.h"
#include "raymath.h"

void Player::ProcessInput()
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

	direction = Vector2Normalize(direction);
	Move(direction);
}

void Player::Move(Vector2& direction)
{
	m_position += direction * m_movementSpeed * GetFrameTime();
	float playerHalfSize = m_playerSize * 0.5f;
	m_position = Vector2Clamp(m_position, { playerHalfSize, playerHalfSize }, { (float)GetScreenWidth() - playerHalfSize, (float)GetScreenHeight() - playerHalfSize});
}

void Player::Render()
{
	DrawRectangle(m_position.x - (m_playerSize * 0.5f), m_position.y - (m_playerSize * 0.5f), m_playerSize, m_playerSize, RED);
	DrawCircleGradient(m_position.x, m_position.y, m_attractRadius, {128, 128, 128, 128}, {128, 128, 128, 0});
}
