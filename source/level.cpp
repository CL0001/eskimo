#include "level.hpp"

#include <fstream>

#include "json.hpp"

using json = nlohmann::json;

Level::Level(const std::string& filePath, std::shared_ptr<Tileset> tileset, std::shared_ptr<Player> player)
	: m_filePath(filePath), m_tileset(tileset), m_player(player)
{
	Build();
}

void Level::Build()
{
	std::ifstream levelFile((std::string(RESOURCES_PATH) + "levels/test.json"));
	json levelData = json::parse(levelFile);

	for (const auto& obstacle : levelData["ObstacleLayout"])
		m_obstacleLayout.emplace_back(m_tileset, Vector2{ obstacle["TilesetPosition"]["x"], obstacle["TilesetPosition"]["y"] }, Vector2{ obstacle["GamePosition"]["x"], obstacle["GamePosition"]["y"] });
}

void Level::Update(const float deltaTime)
{
	HandlePlayerInput(deltaTime);
	HandleCollisions();
}

void Level::HandlePlayerInput(const float deltaTime)
{
	if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D))
		m_player->Idle();

	if (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W))
		m_player->Jump(deltaTime);

	if (IsKeyDown(KEY_A))
		m_player->MoveLeft(deltaTime);

	if (IsKeyDown(KEY_D))
		m_player->MoveRight(deltaTime);
}

void Level::HandleCollisions()
{

}

void Level::Draw()
{
	for (auto& obstacle : m_obstacleLayout)
	{
		obstacle.Draw();
	}
}
