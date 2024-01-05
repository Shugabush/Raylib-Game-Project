#pragma once

#include "Collider.h"

#include <vector>
#include <string>

class GameObject
{
	GameObject* Parent = nullptr;

	Vector2 Forces = {};
	std::vector<GameObject*> Children;

public:
	GameObject();

	bool DrawForwardDirection = true;
	bool DrawCollision = true;

	Color ForwardDirectionColor = RED;
	Color CollisionColor = GREEN;

	std::string Name = "New GameObject";
	std::string Tag = "GameObject";

	Texture2D Texture = {};

	Color Tint = WHITE;

	// Position
	Vector2 Position = {};

	// Velocity
	Vector2 Velocity = {};

	// Rotation
	float Rotation = 0;

	Collider Col;

	bool UseGravity = false;

	// Mass
	float Mass = 1;

	// Drag
	float Drag = 0.5f;

	virtual void EarlyStart();
	virtual void Start();
	virtual void LateStart();

	virtual void EarlyUpdate(float deltaTime);
	virtual void Update(float deltaTime);
	virtual void LateUpdate(float deltaTime);

	virtual void EarlyDraw();
	virtual void Draw();
	virtual void LateDraw();

	void ComputeCollisionEnter(GameObject& OtherObject);
	void ComputeCollisionStay(GameObject& OtherObject);
	void ComputeCollisionExit(GameObject& OtherObject);

	virtual void FixedUpdate(float fixedDelta);

	void AddAccel(Vector2 accel);

	void AddVelocity(Vector2 velocity);

	void AddForce(Vector2 force);

	void AddImpulse(Vector2 impulse);

	Vector2 GetMomentum() const;

	void SetParent(GameObject* Parent);

	virtual void OnCollisionEnter(GameObject& OtherObject);
	virtual void OnCollisionStay(GameObject& OtherObject);
	virtual void OnCollisionExit(GameObject& OtherObject);

	virtual void OnTriggerEnter(GameObject& OtherObject);
	virtual void OnTriggerStay(GameObject& OtherObject);
	virtual void OnTriggerExit(GameObject& OtherObject);
};

void ResolvePhysObjects(GameObject& left, GameObject& right, float elasticity, Vector2 normal, float pen);

float ResolveCollision(const Vector2& posA, const Vector2& velA, float massA,
	const Vector2& posB, const Vector2& velB, float massB,
	float elasticity, const Vector2& normal);