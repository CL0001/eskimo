#pragma once

#include <memory>

#include "rlImGui.h"

#include "tileset.hpp"

class Obstacle
{
public:
	Obstacle(std::shared_ptr<Tileset> tileset, Vector2 tilesetPosition, Vector2 position);
	~Obstacle() = default;

	void Draw();

	Rectangle GetHitbox() const;

private:
	std::shared_ptr<Tileset> m_tileset;
	Vector2 m_tilesetPosition;
	Vector2 m_position;
	Rectangle m_hitbox;
};