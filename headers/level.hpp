#pragma once

#include <string>
#include <memory>
#include <vector>
#include <iostream>

#include "rlImGui.h"

#include "tileset.hpp"
#include "obstacle.hpp"

class Level
{
public:
	Level(const std::string& filePath, std::shared_ptr<Tileset> tileset);
	~Level() = default;

	void Update(float deltaTime);
	void Draw();

private:
	std::string m_filePath;
	std::shared_ptr<Tileset> m_tileset;
	std::vector<Obstacle> m_obstacleLayout;
};