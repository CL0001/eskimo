#include "game.hpp"

Game::Game()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(m_windowWidth, m_windowHeight, "Jumper");

	player = std::make_unique<Player>();
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

	player->Update(deltaTime);
}

void Game::Render()
{
	BeginDrawing();
	ClearBackground(WHITE);

	player->Draw();

	EndDrawing();
}