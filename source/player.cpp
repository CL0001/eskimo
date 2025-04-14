#include "player.hpp"

#include <iostream>
#include <string>

#include "config.hpp"

Player::Player()
	: m_position({ static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 2) }),
	m_velocity(Vector2{}),
	m_spriteAnimationAtlas(LoadTexture((std::string(RESOURCES_PATH) + "sprites/Eskimo/Animations.png").c_str())),
	m_currentAnimationState(AnimationAtlasMapper::Idle),
	m_frameWidth(m_spriteAnimationAtlas.width / ANIMATION_FRAME_COUNT),
	m_frameHeight(m_spriteAnimationAtlas.height / ANIMATION_COUNT),
	m_hitbox(m_position.x, m_position.y, m_frameWidth, m_frameHeight)
{
	if (m_spriteAnimationAtlas.id == 0) {
		std::cerr << "ERROR: Failed to load player texture atlas.\n";
		std::exit(1);
	}
}

Player::~Player()
{
	UnloadTexture(m_spriteAnimationAtlas);
}

void Player::Draw()
{
	Rectangle sourceRectangle = {
		static_cast<float>(m_currentFrame * m_frameWidth),
		static_cast<float>(static_cast<int>(m_currentAnimationState) * m_frameHeight),
		static_cast<float>(m_frameWidth),
		static_cast<float>(m_frameHeight)
	};

	Rectangle destinationRectangle = {
		m_position.x,
		m_position.y,
		static_cast<float>(sourceRectangle.width * SCALE),
		static_cast<float>(sourceRectangle.height * SCALE)
	};

	Vector2 origin = { sourceRectangle.width * (SCALE - 1), sourceRectangle.height * (SCALE - 1) };

	if (!m_facingRight)
	{
		sourceRectangle.width = -m_frameWidth;
		origin.x = sourceRectangle.width / SCALE;
		destinationRectangle.x -= m_frameWidth * SCALE;
	}
	else
	{
		sourceRectangle.width = m_frameWidth;
	}

	DrawTexturePro(m_spriteAnimationAtlas, sourceRectangle, destinationRectangle, origin, 0.0f, WHITE);
}

void Player::Update(const float deltaTime)
{
	UpdateHitbox();

	ApplyGravity(deltaTime);

	UpdateAnimationFrame(deltaTime);
}

void Player::UpdateHitbox()
{
	m_hitbox = { m_position.x, m_position.y, m_frameWidth, m_frameHeight };
}

CollisionDirection Player::CheckCollisionWith(const Rectangle& otherHitbox) const
{
	if (!CheckCollisionRecs(m_hitbox, otherHitbox))
		return CollisionDirection::None;

	float dx = (m_hitbox.x + m_hitbox.width / 2) - (otherHitbox.x + otherHitbox.width / 2);
	float dy = (m_hitbox.y + m_hitbox.height / 2) - (otherHitbox.y + otherHitbox.height / 2);

	float combinedHalfWidths = (m_hitbox.width + otherHitbox.width) / 2;
	float combinedHalfHeights = (m_hitbox.height + otherHitbox.height) / 2;

	float overlapX = combinedHalfWidths - std::abs(dx);
	float overlapY = combinedHalfHeights - std::abs(dy);

	if (overlapX < overlapY)
	{
		return dx > 0 ? CollisionDirection::Left : CollisionDirection::Right;
	}
	else
	{
		return dy > 0 ? CollisionDirection::Top : CollisionDirection::Bottom;
	}
}

void Player::ResolveCollision(const CollisionDirection& collisionDirection)
{
	switch (collisionDirection)
	{
	case CollisionDirection::Top:
		m_velocity.y = 0.0f;
		break;

	case CollisionDirection::Bottom:
		m_velocity.y = 0.0f;
		m_isOnGround = true;
		break;

	case CollisionDirection::Left:
		m_position.x += 1.0f;
		break;

	case CollisionDirection::Right:
		m_position.x -= 1.0f;
		break;

	default:
		break;
	}
}

void Player::Idle()
{
	if (m_isOnGround)
		SetAnimationState(AnimationAtlasMapper::Idle);
}

void Player::MoveLeft(const float deltaTime)
{
	if (m_isOnGround)
		SetAnimationState(AnimationAtlasMapper::Walk);

	m_position.x -= BASE_MOVE_SPEED * deltaTime;
	m_facingRight = false;
}

void Player::MoveRight(const float deltaTime)
{
	if (m_isOnGround)
		SetAnimationState(AnimationAtlasMapper::Walk);

	m_position.x += BASE_MOVE_SPEED * deltaTime;
	m_facingRight = true;
}

void Player::Jump(const float deltaTime)
{
	if (!m_isOnGround)
		return;

	m_velocity.y = BASE_JUMP_STRENGTH;
	m_isOnGround = false;

	SetAnimationState(AnimationAtlasMapper::Jump);
}

void Player::ApplyGravity(const float deltaTime)
{
    m_velocity.y += BASE_GRAVITY * deltaTime;
    m_position.y += m_velocity.y * deltaTime;
}

void Player::UpdateAnimationFrame(const float deltaTime)
{
	m_animationTimer += deltaTime;
	if (m_animationTimer >= ANIMATION_FRAME_TIME)
	{
		m_currentFrame++;
		if (m_currentFrame >= ANIMATION_FRAME_COUNT)
		{
			m_currentFrame = 0;
		}
		m_animationTimer = 0.0f;
	}
}

void Player::SetAnimationState(AnimationAtlasMapper newState)
{
	if (m_currentAnimationState != newState)
	{
		m_currentAnimationState = newState;
		m_currentFrame = 0;
	}
}