#include "player.hpp"

Player::Player()
	: m_position({ static_cast<float>(GetScreenWidth() / 2), static_cast<float>(GetScreenHeight() / 2) }),
	m_spriteAnimationAtlas(LoadTexture((std::string(RESOURCES_PATH) + "sprites/Eskimo/Animations.png").c_str())),
	m_currentAnimationState(AnimationAtlasMapper::Idle)
{
	if (m_spriteAnimationAtlas.id == 0) {
		std::cerr << "ERROR: Failed to load player texture atlas.\n";
		std::exit(1);
	}

	m_frameWidth = m_spriteAnimationAtlas.width / m_totalFrames;
	m_frameHeight = m_spriteAnimationAtlas.height / 9;
}

Player::~Player() {
	UnloadTexture(m_spriteAnimationAtlas);
}

void Player::Update(float deltaTime)
{
	m_velocityY += m_gravity * deltaTime;
	m_position.y += m_velocityY * deltaTime;

	if (m_position.y + m_frameHeight >= GetScreenHeight())
	{
		m_position.y = GetScreenHeight() - m_frameHeight;
		m_velocityY = 0.0f;
		m_isOnGround = true;
	}
	else
	{
		m_isOnGround = false;
	}

	if ((IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_W)) && m_isOnGround)
	{
		m_velocityY = m_jumpStrength;
		m_isOnGround = false;

		SetAnimationState(AnimationAtlasMapper::Jump);
	}

	if (IsKeyDown(KEY_A) && m_position.x > 0)
	{
		m_position.x -= m_moveSpeed * deltaTime;
		m_facingRight = false;

		if (m_isOnGround)
		{
			SetAnimationState(AnimationAtlasMapper::Walk);
		}
	}

	if (IsKeyDown(KEY_D) && m_position.x + m_frameWidth < GetScreenWidth())
	{
		m_position.x += m_moveSpeed * deltaTime;
		m_facingRight = true;

		if (m_isOnGround)
		{
			SetAnimationState(AnimationAtlasMapper::Walk);
		}
	}

	if (!IsKeyDown(KEY_A) && !IsKeyDown(KEY_D) && m_isOnGround)
	{
		SetAnimationState(AnimationAtlasMapper::Idle);
	}

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

void Player::SetAnimationState(AnimationAtlasMapper newState)
{
	if (m_currentAnimationState != newState)
	{
		m_currentAnimationState = newState;
		m_currentFrame = 0;
	}
}