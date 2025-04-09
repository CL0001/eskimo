#pragma once

#include <memory>

#include "rlImGui.h"

#include "player.hpp"

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

	std::unique_ptr<Player> player;
};