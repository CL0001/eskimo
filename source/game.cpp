#include "game.hpp"

#include <string>

Game::Game()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(m_windowWidth, m_windowHeight, "Jumper");

	m_tileset = std::make_shared<Tileset>((std::string(RESOURCES_PATH) + "sprites/World/world_tileset.png"), 16, 16);
	m_player = std::make_shared<Player>();
	m_level = std::make_unique<Level>((std::string(RESOURCES_PATH) + "levels/tutorial.json"), m_tileset, m_player);
}

void Game::Run()
{
	while (!WindowShouldClose())
	{
		Update();

		Render();
	}
}

void Game::Update()
{
	float deltaTime = GetFrameTime();

	m_level->Update(deltaTime);
	m_player->Update(deltaTime);
}

void Game::Render()
{
	BeginDrawing();
	ClearBackground(WHITE);

	m_level->Draw();
	m_player->Draw();

	EndDrawing();
}