#pragma once

#include <iostream>
#include <string>

#include "rlImGui.h"

class Tileset
{
public:
	Tileset(const std::string& filepath, int tileWidth, int tileHeight);
	~Tileset();

	Rectangle GetTileRectangle(int tileX, int tileY);

	Texture2D GetTileset();

private:
	Texture2D m_tileset;
	int m_tileWidth;
	int m_tileHeight;
};