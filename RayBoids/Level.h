#pragma once

#include <string>
#include "World.h"

namespace Level
{
	std::unique_ptr<World> LoadLevelByIndex(int levelIndex);
	std::unique_ptr<World> LoadLevel(std::string& path);
};

