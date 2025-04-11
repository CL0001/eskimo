#include "hitbox.hpp"

Hitbox::Hitbox(Vector2 position, float width, float height)
	: m_leftBound(position.x), m_topBound(position.y), m_rightBound(position.x + width), m_bottomBound(position.y + height), m_isFacingRight(true)
{
}

Hitbox::Hitbox(Vector2 position, float width, float height, bool isFacingRight)
	: m_leftBound(position.x), m_topBound(position.y), m_rightBound(position.x + width), m_bottomBound(position.y + height), m_isFacingRight(isFacingRight)
{
	if (!isFacingRight)
	{
		m_leftBound = position.x - width * (SCALE - (SCALE - 1.0f) * (width / 2) / width * 0.2);
		m_rightBound = position.x;
	}
}

void Hitbox::Update(Vector2 position, float width, float height)
{
	m_leftBound = position.x;
	m_topBound = position.y;
	m_rightBound = position.x + width;
	m_bottomBound = position.y + height;
}

void Hitbox::Update(Vector2 position, float width, float height, bool isFacingRight)
{
	m_leftBound = position.x;
	m_topBound = position.y;
	m_rightBound = position.x + width;
	m_bottomBound = position.y + height;

	if (!isFacingRight)
	{
		m_leftBound = position.x - width * (SCALE - (SCALE - 1.0f) * (width / 2) / width * 0.2);
		m_rightBound = position.x;
	}
}

CollisionDirection Hitbox::CheckCollisionBetween(const Hitbox& hitbox1, const Hitbox& hitbox2)
{
	bool collisionX = hitbox1.GetRightBound() > hitbox2.GetLeftBound() && hitbox1.GetLeftBound() < hitbox2.GetRightBound();

	bool collisionY = hitbox1.GetBottomBound() > hitbox2.GetTopBound() && hitbox1.GetTopBound() < hitbox2.GetBottomBound();

	if (!collisionX || !collisionY)
	{
		return CollisionDirection::None;
	}

	if (hitbox1.GetRightBound() > hitbox2.GetLeftBound() && hitbox1.GetLeftBound() < hitbox2.GetLeftBound())
	{
		return CollisionDirection::Left;
	}
	if (hitbox1.GetLeftBound() < hitbox2.GetRightBound() && hitbox1.GetRightBound() > hitbox2.GetRightBound())
	{
		return CollisionDirection::Right;
	}
	if (hitbox1.GetBottomBound() > hitbox2.GetTopBound() && hitbox1.GetTopBound() < hitbox2.GetTopBound())
	{
		return CollisionDirection::Top;
	}
	if (hitbox1.GetTopBound() < hitbox2.GetBottomBound() && hitbox1.GetBottomBound() > hitbox2.GetBottomBound())
	{
		return CollisionDirection::Bottom;
	}

	return CollisionDirection::None;
}

float Hitbox::GetLeftBound() const
{
	return m_leftBound;
}

float Hitbox::GetTopBound() const
{
	return m_topBound;
}

float Hitbox::GetRightBound() const
{
	return m_rightBound;
}

float Hitbox::GetBottomBound() const
{
	return m_bottomBound;
}