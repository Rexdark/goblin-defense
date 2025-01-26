#pragma once

#include <cstdint>
#include <string>

#include <Core/Game.h>

Game::GameCreateInfo loadFromConfig(std::string configPath);

void createDefaultConfigFile(std::string configPath);

bool fileExists(const std::string& filename);