#pragma once

#include <string>
#include <memory>
#include <vector>

#include "rlImGui.h"

#include "tileset.hpp"
#include "player.hpp"
#include "obstacle.hpp"

class Level
{
public:
	Level(const std::string& filePath, std::shared_ptr<Tileset> tileset, std::shared_ptr<Player> player);
	~Level() = default;

	void Build();

	void Update(const float deltaTime);

	void HandlePlayerInput(const float deltaTime);
	void HandleCollisions();

	void Draw();

private:
	std::string m_filePath;
	std::shared_ptr<Tileset> m_tileset;
	std::shared_ptr<Player> m_player;
	std::vector<Obstacle> m_obstacleLayout;
};