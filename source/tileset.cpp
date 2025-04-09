#include "tileset.hpp"

Tileset::Tileset(const std::string& filePath, int tileWidth, int tileHeight)
	: m_tileset(LoadTexture(filePath.c_str())), m_tileWidth(tileWidth), m_tileHeight(tileHeight)
{
	if (m_tileset.id == 0)
	{
		std::cerr << "ERROR: failed to load tile set texture atlas";
	}
}

Tileset::~Tileset()
{
	UnloadTexture(m_tileset);
}

Rectangle Tileset::GetTileRectangle(int tileX, int tileY)
{
	return {
		static_cast<float>(m_tileWidth * tileX),
		static_cast<float>(m_tileHeight * tileY),
		static_cast<float>(m_tileWidth),
		static_cast<float>(m_tileHeight)
	};
}

Texture2D Tileset::GetTileset()
{
	return m_tileset;
}