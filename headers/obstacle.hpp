#pragma once

#include <memory>

#include "rlImGui.h"

#include "tileset.hpp"
#include "config.hpp"

class Obstacle
{
public:
	Obstacle(std::shared_ptr<Tileset> m_tileset, Vector2 tilesetPosition, Vector2 position);
	~Obstacle() = default;

	void Draw();

private:
	std::shared_ptr<Tileset> m_tileset;
	Vector2 m_tilesetPosition;
	Vector2 m_position;
};