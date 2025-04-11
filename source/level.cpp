#include "level.hpp"

Level::Level(const std::string& filePath, std::shared_ptr<Tileset> tileset)
	: m_filePath(filePath), m_tileset(tileset)
{
	m_obstacleLayout.emplace_back(m_tileset, Vector2{ 6, 1 }, Vector2{ GetScreenWidth() / 2.0f, GetScreenHeight() / 2.0f + 200});
}

void Level::Update(float deltaTime)
{

}

void Level::Draw()
{
	for (auto& obstacle : m_obstacleLayout)
	{
		obstacle.Draw();
	}
}
