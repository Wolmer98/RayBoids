#include "Resources.h"

Resources* Resources::GetResources()
{
	if (instance == nullptr)
		instance = new Resources();

	return instance;
}

void Resources::LoadResources()
{
	backgroundTexture = LoadTexture("Textures/GrassBackground.png");
	wallTexture = LoadTexture("Textures/Wall.png");
	playerTexture = LoadTexture("Textures/Player.png");
	music = LoadMusicStream("Audio/Fireflies.mp3");
}

void Resources::UnloadResources()
{
	UnloadTexture(backgroundTexture);
	UnloadTexture(wallTexture);
	UnloadTexture(playerTexture);
	UnloadMusicStream(music);
}
