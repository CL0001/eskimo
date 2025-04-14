#include "obstacle.hpp"

#include "config.hpp"

Obstacle::Obstacle(std::shared_ptr<Tileset> tileset, Vector2 tilesetPosition, Vector2 position)
	: m_tileset(tileset), m_tilesetPosition(tilesetPosition), m_position(position), m_hitbox(m_position.x, m_position.y, TILE_SIZE, TILE_SIZE)
{
}

void Obstacle::Draw()
{
	Rectangle sourceRectangle = m_tileset->GetTileRectangle(
		static_cast<int>(m_tilesetPosition.x),
		static_cast<int>(m_tilesetPosition.y)
	);

	Rectangle destinationRectangle = {
		m_position.x * TILE_SIZE * SCALE,
		m_position.y * TILE_SIZE * SCALE,
		sourceRectangle.width * SCALE,
		sourceRectangle.height * SCALE
	};

	Vector2 origin = { 0.0f, 0.0f };

	DrawTexturePro(
		m_tileset->GetTileset(),
		sourceRectangle,
		destinationRectangle,
		origin,
		0.0f,
		WHITE
	);
}

Rectangle Obstacle::GetHitbox() const
{
	return m_hitbox;
}