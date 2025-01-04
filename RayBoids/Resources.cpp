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
}
