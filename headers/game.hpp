#pragma once

#include <memory>
#include <string>

#include "rlImGui.h"

#include "tileset.hpp"
#include "level.hpp"
#include "player.hpp"
#include "hitbox.hpp"

class Game
{
public:
	Game();

	void Run();

	void Update();

	void Render();

private:
	const int m_windowWidth = 1200;
	const int m_windowHeight = 720;

	std::shared_ptr<Tileset> m_tileset;
	std::shared_ptr<Player> m_player;
	std::unique_ptr<Level> m_level;
};