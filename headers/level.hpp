#pragma once

#include <string>
#include <memory>

#include "rlImGui.h"

#include "tileset.hpp"

class Level
{
public:
	Level(const std::string& filePath, std::shared_ptr<Tileset> tileset);
	~Level() = default;

	void Update(float deltaTime);

private:
	std::string m_filePath;
	std::shared_ptr<Tileset> m_tileset;
};