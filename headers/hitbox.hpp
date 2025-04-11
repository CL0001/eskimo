#pragma once

#include "rlImGui.h"
#include "config.hpp"

enum class CollisionDirection
{
	None = 0,
	Left,
	Top,
	Right,
	Bottom
};

class Hitbox
{
public:
	Hitbox() = default;
	Hitbox(Vector2 position, float width, float height);
	Hitbox(Vector2 position, float width, float height, bool isFacingRight);
	~Hitbox() = default;

	void Update(Vector2 position, float width, float height);
	void Update(Vector2 position, float width, float height, bool isFacingRight);

	static CollisionDirection CheckCollisionBetween(const Hitbox& hitbox1, const Hitbox& hitbox2);

	float GetLeftBound() const;
	float GetTopBound() const;
	float GetRightBound() const;
	float GetBottomBound() const;

private:
	float m_leftBound;
	float m_topBound;
	float m_rightBound;
	float m_bottomBound;

	bool m_isFacingRight;
};