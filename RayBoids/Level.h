#pragma once

#include <string>
#include "World.h"

namespace Level
{
	std::unique_ptr<World> LoadLevelByIndex(std::size_t levelIndex);
	std::unique_ptr<World> LoadLevel(std::string& path);
};

