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

	m_velocity.x = BASE_JUMP_STRENGTH;
	m_isOnGround = false;

	SetAnimationState(AnimationAtlasMapper::Jump);
}

void Player::ApplyGravity(const float deltaTime)
{
	m_velocity.x += BASE_GRAVITY * deltaTime;
	m_position.y += m_velocity.x * deltaTime;
}

void Player::CheckGroundCollision()
{
	if (m_position.y + m_frameHeight >= GetScreenHeight())
	{
		m_position.y = GetScreenHeight() - m_frameHeight;
		m_velocity.x = 0.0f;
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

	// Log the player's hitbox bounds
	std::clog << "PLAYER: Left = " << m_hitbox.GetLeftBound()
		<< ", Top = " << m_hitbox.GetTopBound()
		<< ", Right = " << m_hitbox.GetRightBound()
		<< ", Bottom = " << m_hitbox.GetBottomBound()
		<< std::endl;

	// Log the obstacle's hitbox bounds
	std::clog << "OBSTACLE: Left = " << otherHitbox.GetLeftBound()
		<< ", Top = " << otherHitbox.GetTopBound()
		<< ", Right = " << otherHitbox.GetRightBound()
		<< ", Bottom = " << otherHitbox.GetBottomBound()
		<< std::endl;

	bool collisionX = m_hitbox.GetRightBound() > otherHitbox.GetLeftBound() && m_hitbox.GetLeftBound() < otherHitbox.GetRightBound();
	bool collisionY = m_hitbox.GetBottomBound() > otherHitbox.GetTopBound() && m_hitbox.GetTopBound() < otherHitbox.GetBottomBound();

	if (!collisionX || !collisionY)
		return CollisionDirection::None;

	if (m_hitbox.GetRightBound() > otherHitbox.GetLeftBound() && m_hitbox.GetLeftBound() < otherHitbox.GetLeftBound())
		return CollisionDirection::Left;

	if (m_hitbox.GetLeftBound() < otherHitbox.GetRightBound() && m_hitbox.GetRightBound() > otherHitbox.GetRightBound())
		return CollisionDirection::Right;

	if (m_hitbox.GetBottomBound() > otherHitbox.GetTopBound() && m_hitbox.GetTopBound() < otherHitbox.GetTopBound())
		return CollisionDirection::Top;

	if (m_hitbox.GetBottomBound() <= otherHitbox.GetTopBound() && m_hitbox.GetTopBound() < otherHitbox.GetTopBound())
		return CollisionDirection::Bottom;

	return CollisionDirection::None;
}

void Player::ResolveCollision(const CollisionDirection& collisionDirection)
{
	std::clog << "RESOLVING COLLISION... ";

	switch (collisionDirection)
	{
	case CollisionDirection::Left:
		std::clog << "COLLISION DETECTED - LEFT\n";
		m_position.x = m_hitbox.GetLeftBound();  // Snap player to the right of the obstacle
		m_velocity.x = 0.0f;  // Stop horizontal movement
		break;

	case CollisionDirection::Top:
		std::clog << "COLLISION DETECTED - TOP\n";
		m_velocity.x = 0.0f;  // Stop vertical movement
		m_position.y = m_hitbox.GetTopBound() + m_frameHeight;  // Snap below the obstacle
		break;

	case CollisionDirection::Right:
		std::clog << "COLLISION DETECTED - RIGHT\n";
		m_position.x = m_hitbox.GetRightBound() - m_frameWidth;  // Snap player to the left of the obstacle
		m_velocity.x = 0.0f;  // Stop horizontal movement
		break;

	case CollisionDirection::Bottom:
		std::clog << "COLLISION DETECTED - BOTTOM\n";
		m_velocity.x = 0.0f;  // Stop vertical movement
		m_position.y = m_hitbox.GetBottomBound() - m_frameHeight;  // Snap on top of the obstacle
		break;

	default:
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