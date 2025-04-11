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

	//
	if (IsKeyDown(KEY_A) && m_position.x - m_frameWidth * GetLeftFacingHitboxOffset() > 0)
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

// This function calculates the offset for the hitbox of the player's sprite when it is facing left (mirrored).
// It ensures that the left edge of the sprite aligns correctly with the screen boundary without stopping prematurely.
// This is important for ensuring the player character doesn't move outside the left edge of the screen when facing left.
//
// The calculation accounts for:
// - The scaling factor (SCALE), which affects the size of the sprite.
// - The width of the sprite frame (m_frameWidth), which determines how much space the sprite occupies on the screen.
// - The offset required when the sprite is facing left (mirrored), ensuring the sprite's left edge is correctly aligned.
//
// The factor of 0.2 is an adjustment value that was experimentally determined to compensate for the mirrored effect,
// ensuring a visually correct offset for the character's movement when facing left.
//
// Example Calculation:
// Given SCALE = 4.0, m_frameWidth = 16:
// 1. SCALE - 1.0f = 4.0 - 1.0 = 3.0
// 2. m_frameWidth / 2 = 16 / 2 = 8
// 3. (m_frameWidth / 2) / m_frameWidth = 8 / 16 = 0.5
// 4. Multiply by 0.2: 3.0 * 0.5 * 0.2 = 0.3
// The final result is 0.3, which is the offset that compensates for the mirrored sprite.
//
// This offset is used to adjust the sprite's position when facing left to ensure it aligns properly with the left screen edge.
float Player::GetLeftFacingHitboxOffset()
{
	return SCALE - (SCALE - 1.0f) * (m_frameWidth / 2) / m_frameWidth * 0.2;
}

void Player::SetAnimationState(AnimationAtlasMapper newState)
{
	if (m_currentAnimationState != newState)
	{
		m_currentAnimationState = newState;
		m_currentFrame = 0;
	}
}