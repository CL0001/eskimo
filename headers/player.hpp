#pragma once

#include "rlImGui.h"

#include "collision_direction.hpp"

enum class AnimationAtlasMapper {
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

	void Update(const float deltaTime);

	void UpdateHitbox();
	CollisionDirection CheckCollisionWith(const Rectangle& otherHitbox) const;
	void ResolveCollision(const CollisionDirection& collisionDirection);

	void Idle();
	void MoveLeft(const float deltaTime);
	void MoveRight(const float deltaTime);
	void Jump(const float deltaTime);

	void ApplyGravity(const float deltaTime);
	void UpdateAnimationFrame(const float deltaTime);

	void Draw();

	void SetAnimationState(AnimationAtlasMapper newState);

private:
	Vector2 m_position;
	Vector2 m_velocity;
	bool m_isOnGround = false;
	bool m_facingRight = true;

	Texture2D m_spriteAnimationAtlas;

	float m_animationTimer = 0.0f;
	int m_currentFrame = 0;

	AnimationAtlasMapper m_currentAnimationState;
	float m_frameWidth;
	float m_frameHeight;

	Rectangle m_hitbox;
};