#include "game.hpp"

Game::Game()
{
	SetConfigFlags(FLAG_WINDOW_RESIZABLE);
	InitWindow(m_windowWidth, m_windowHeight, "Jumper");

	m_tileset = std::make_shared<Tileset>((std::string(RESOURCES_PATH) + "sprites/World/world_tileset.png"), 16, 16);
	m_level = std::make_unique<Level>((std::string(RESOURCES_PATH) + "levels/tutorial.json"), m_tileset);
	m_player = std::make_unique<Player>();
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

	HandleInput(deltaTime);

	m_level->Update(deltaTime);
	m_player->Update(deltaTime);
}

void Game::HandleInput(float deltaTime)
{
	Vector2 playerPosition = m_player->GetPosition();
	float playerY = playerPosition.y;
	float playerFrameHeight = m_player->GetFrameHeight();
	float screenHeight = GetScreenHeight();
	float verticalVelocity = m_player->GetVerticalVelocity();
	bool isOnGround = m_player->GetIsOnGround();
	float gravity = m_player->GetGravity();
	float moveSpeed = m_player->GetMoveSpeed();
	bool isFacingRight = m_player->GetIsFacingRight();
	float jumpStrength = m_player->GetJumpStrength();

	m_player->SetVerticalVelocity(verticalVelocity + gravity * deltaTime);
	playerPosition.y = playerY + verticalVelocity * deltaTime;
	m_player->SetPosition(playerPosition);

	if (playerPosition.y + playerFrameHeight > screenHeight)
	{
		playerPosition.y = screenHeight - playerFrameHeight;
		m_player->SetPosition(playerPosition);
		m_player->SetVerticalVelocity(0.0f);
		m_player->SetIsOnGround(true);
	}
	else
	{
		m_player->SetIsOnGround(false);
	}

	if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) && isOnGround)
	{
		m_player->SetVerticalVelocity(jumpStrength);
		m_player->SetIsOnGround(false);
		m_player->SetAnimationState(AnimationAtlasMapper::Jump);
	}

	if (IsKeyDown(KEY_A))
	{
		playerPosition.x -= moveSpeed * deltaTime;
		m_player->SetPosition(playerPosition);
		m_player->SetIsFacingRight(false);

		if (isOnGround)
		{
			m_player->SetAnimationState(AnimationAtlasMapper::Walk);
		}
	}

	if (IsKeyDown(KEY_D))
	{
		playerPosition.x += moveSpeed * deltaTime;
		m_player->SetPosition(playerPosition);
		m_player->SetIsFacingRight(true);

		if (isOnGround)
		{
			m_player->SetAnimationState(AnimationAtlasMapper::Walk);
		}
	}

	if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && isOnGround)
	{
		m_player->SetAnimationState(AnimationAtlasMapper::Idle);
	}
}

void Game::CheckForCollisions()
{

}

void Game::Render()
{
	BeginDrawing();
	ClearBackground(WHITE);

	m_level->Draw();
	m_player->Draw();

	EndDrawing();
}