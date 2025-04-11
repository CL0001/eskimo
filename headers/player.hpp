#pragma once

#include <iostream>
#include <unordered_map>
#include <string>

#include "rlImGui.h"

#include "config.hpp"
#include "hitbox.hpp"

enum AnimationAtlasMapper {
	Attack = 0,
	Death,
	Idle,
	Item,
	Jump,
	PickUp,
	Special,
	Walk,
	Leave
};

class Player
{
public:
	Player();
	~Player();

	void Update(float deltaTime);

	void Draw();

	Vector2 GetPosition() const;
	float GetVerticalVelocity() const;
	float GetGravity() const;
	float GetFrameWidth() const;
	float GetFrameHeight() const;
	bool GetIsOnGround() const;
	float GetJumpStrength() const;
	float GetMoveSpeed() const;
	Hitbox GetHitbox() const;
	bool GetIsFacingRight() const;

	void SetPosition(Vector2 position);
	void SetVerticalVelocity(float velocity);
	void SetIsOnGround(bool isOnGround);
	void SetIsFacingRight(bool isFacingRight);
	void SetAnimationState(AnimationAtlasMapper newState);

private:
	Vector2 m_position;
	float m_verticalVelocity = 0;
	bool m_isOnGround = false;

	const float m_gravity = 1500.0f;
	const float m_jumpStrength = -600.0f;
	const float m_moveSpeed = 300.0f;

	Texture2D m_spriteAnimationAtlas;

	float m_animationTimer = 0.0f;
	float m_frameTime = 0.1f;
	int m_currentFrame = 0;
	int m_totalFrames = 4;

	AnimationAtlasMapper m_currentAnimationState;
	int m_frameWidth;
	int m_frameHeight;

	Hitbox m_hitbox;
	bool m_facingRight = true;
};