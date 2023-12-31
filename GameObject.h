#pragma once

#include "raylib.h"

#include "GameObject.h"
#include "Shape.h"

class GameObject
{
	Vector2 Forces = {};

public:
	Color ObjectColor = RED;

	// Position
	Vector2 Position = {};

	// Velocity
	Vector2 Velocity = {};

	// Rotation
	float Rotation = 0;

	Shape Collider;

	bool UseGravity = true;

	// Mass
	float Mass = 1;

	// Drag
	float Drag = 0.5f;

	void TickPhys(float fixedDelta);

	void AddAccel(Vector2 accel);

	void AddVelocity(Vector2 velocity);

	void AddForce(Vector2 force);

	void AddImpulse(Vector2 impulse);

	virtual void Draw() const;

	Vector2 GetMomentum() const;
};

void ResolvePhysObjects(GameObject& left, GameObject& right, float elasticity, Vector2 normal, float pen);

float ResolveCollision(const Vector2& posA, const Vector2& velA, float massA,
	const Vector2& posB, const Vector2& velB, float massB,
	float elasticity, const Vector2& normal);