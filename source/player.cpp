#include "player.hpp"

Player::Player()
	: m_position({ static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 2) }),
	m_spriteAnimationAtlas(LoadTexture((std::string(RESOURCES_PATH) + "sprites/Eskimo/Animations.png").c_str())),
	m_currentAnimationState(AnimationAtlasMapper::Idle),
	m_frameWidth(m_spriteAnimationAtlas.width / m_totalFrames),
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

	m_animationTimer += deltaTime;
	if (m_animationTimer >= m_frameTime)
	{
		m_currentFrame++;
		if (m_currentFrame >= m_totalFrames)
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

Vector2 Player::GetPosition() const
{
	return m_position;
}

float Player::GetVerticalVelocity() const
{
	return m_verticalVelocity;
}

float Player::GetGravity() const
{
	return m_gravity;
}

float Player::GetFrameWidth() const
{
	return m_frameWidth;
}

float Player::GetFrameHeight() const
{
	return m_frameHeight;
}

bool Player::GetIsOnGround() const
{
	return m_isOnGround;
}

float Player::GetJumpStrength() const
{
	return m_jumpStrength;
}

float Player::GetMoveSpeed() const
{
	return m_moveSpeed;
}

Hitbox Player::GetHitbox() const
{
	return m_hitbox;
}

bool Player::GetIsFacingRight() const
{
	return m_facingRight;
}

void Player::SetPosition(Vector2 position)
{
	m_position = position;
}

void Player::SetVerticalVelocity(float velocity)
{
	m_verticalVelocity = velocity;
}

void Player::SetIsOnGround(bool isOnGround)
{
	m_isOnGround = isOnGround;
}

void Player::SetIsFacingRight(bool isFacingRight)
{
	m_facingRight = isFacingRight;
}

void Player::SetAnimationState(AnimationAtlasMapper newState)
{
	if (m_currentAnimationState != newState)
	{
		m_currentAnimationState = newState;
		m_currentFrame = 0;
	}
}