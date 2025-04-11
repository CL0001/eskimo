#include "level.hpp"

Level::Level(const std::string& filePath, std::shared_ptr<Tileset> tileset)
	: m_filePath(filePath), m_tileset(tileset)
{
}

void Level::Update(float deltaTime)
{

}