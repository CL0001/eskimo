#include "obstacle.hpp"

Obstacle::Obstacle(std::shared_ptr<Tileset> m_tileset, Vector2 tilesetPosition, Vector2 position)
	: m_tileset(m_tileset), m_tilesetPosition(tilesetPosition), m_position(position)
{
}

void Obstacle::Draw()
{
	Rectangle sourceRectangle = m_tileset->GetTileRectangle(
		static_cast<int>(m_tilesetPosition.x),
		static_cast<int>(m_tilesetPosition.y)
	);

	Rectangle destinationRectangle = {
		m_position.x,
		m_position.y,
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