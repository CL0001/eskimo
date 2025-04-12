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

	void Idle();
	void MoveLeft(const float deltaTime);
	void MoveRight(const float deltaTime);
	void Jump(const float deltaTime);

	void ApplyGravity(const float deltaTime);
	void CheckGroundCollision();
	void UpdateAnimationFrame(const float deltaTime);

	void Draw();

	void SetAnimationState(AnimationAtlasMapper newState);

private:
	Vector2 m_position;
	float m_verticalVelocity = 0;
	bool m_isOnGround = false;
	bool m_facingRight = true;

	Texture2D m_spriteAnimationAtlas;

	float m_animationTimer = 0.0f;
	int m_currentFrame = 0;

	AnimationAtlasMapper m_currentAnimationState;
	int m_frameWidth;
	int m_frameHeight;

	Hitbox m_hitbox;
};