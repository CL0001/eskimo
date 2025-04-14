#include "player.hpp"

Player::Player()
	: m_position({ static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 2) }),
	m_velocity(Vector2{}),
	m_spriteAnimationAtlas(LoadTexture((std::string(RESOURCES_PATH) + "sprites/Eskimo/Animations.png").c_str())),
	m_currentAnimationState(AnimationAtlasMapper::Idle),
	m_frameWidth(m_spriteAnimationAtlas.width / ANIMATION_FRAME_COUNT),
	m_frameHeight(m_spriteAnimationAtlas.height / 9),
	m_hitbox(m_position, m_frameWidth, m_frameHeight)
{
	if (m_spriteAnimationAtlas.id == 0) {
		std::cerr << "ERROR: Failed to load player texture atlas.\n";
		std::exit(1);
	}
}

Player::~Player() {
	UnloadTexture(m_spriteAnimationAtlas);
}

void Player::Update(float deltaTime)
{
	m_hitbox.Update(m_position, m_frameWidth, m_frameHeight);

	ApplyGravity(deltaTime);

	CheckGroundCollision();

	UpdateAnimationFrame(deltaTime);
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

void Player::CheckGroundCollision()
{
	if (m_position.y + m_frameHeight >= GetScreenHeight())
	{
		m_position.y = GetScreenHeight() - m_frameHeight;
		m_velocity.y = 0.0f;
		m_isOnGround = true;
	}
	else
	{
		m_isOnGround = false;
	}
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

void Player::Draw()
{
	Rectangle srcRect = {
		static_cast<float>(m_currentFrame * m_frameWidth),
		static_cast<float>(static_cast<int>(m_currentAnimationState) * m_frameHeight),
		static_cast<float>(m_frameWidth),
		static_cast<float>(m_frameHeight)
	};

	Rectangle destRect = {
		m_position.x,
		m_position.y,
		static_cast<float>(srcRect.width * SCALE),
		static_cast<float>(srcRect.height * SCALE)
	};

	Vector2 origin = { srcRect.width * (SCALE - 1), srcRect.height * (SCALE - 1) };

	if (!m_facingRight)
	{
		srcRect.width = -m_frameWidth;
		origin.x = srcRect.width / SCALE;
		destRect.x -= m_frameWidth * SCALE;
	}
	else
	{
		srcRect.width = m_frameWidth;
	}

	DrawTexturePro(
		m_spriteAnimationAtlas,
		srcRect,
		destRect,
		origin,
		0.0f,
		WHITE
	);
}

CollisionDirection Player::CheckCollisionWith(const Hitbox& otherHitbox) const
{
	bool collisionX = m_hitbox.GetRightBound() > otherHitbox.GetLeftBound() &&
		m_hitbox.GetLeftBound() < otherHitbox.GetRightBound();
	bool collisionY = m_hitbox.GetBottomBound() > otherHitbox.GetTopBound() &&
		m_hitbox.GetTopBound() < otherHitbox.GetBottomBound();

	if (!collisionX || !collisionY)
		return CollisionDirection::None;

	float overlapLeft = m_hitbox.GetRightBound() - otherHitbox.GetLeftBound();
	float overlapRight = otherHitbox.GetRightBound() - m_hitbox.GetLeftBound();
	float overlapTop = m_hitbox.GetBottomBound() - otherHitbox.GetTopBound();
	float overlapBottom = otherHitbox.GetBottomBound() - m_hitbox.GetTopBound();

	float minOverlap = std::min({ overlapLeft, overlapRight, overlapTop, overlapBottom });

	if (minOverlap == overlapTop)
		return CollisionDirection::Top;
	if (minOverlap == overlapBottom)
		return CollisionDirection::Bottom;
	if (minOverlap == overlapLeft)
		return CollisionDirection::Left;
	if (minOverlap == overlapRight)
		return CollisionDirection::Right;

	return CollisionDirection::None;
}

void Player::ResolveCollision(const CollisionDirection& collisionDirection)
{
	switch (collisionDirection)
	{
	case CollisionDirection::Top:
		m_velocity.y = 0.0f;
		m_position.y = m_hitbox.GetTopBound() - m_frameHeight;
		m_isOnGround = true;
		break;

	case CollisionDirection::Bottom:
		m_velocity.y = 0.0f;
		m_position.y = m_hitbox.GetBottomBound();
		break;

	case CollisionDirection::Left:
		m_velocity.x = 0.0f;
		m_position.x = m_hitbox.GetLeftBound() - m_frameWidth;
		break;

	case CollisionDirection::Right:
		m_velocity.x = 0.0f;
		m_position.x = m_hitbox.GetRightBound();
		break;
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