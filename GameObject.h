#pragma once
#include "Ray.hpp"

class GameObject
{
public:
	Vector2 Position;
	float Rotation;

	virtual void EarlyStart();
	virtual void Start();
	virtual void LateStart();

	virtual void EarlyUpdate(float DeltaTime);
	virtual void Update(float DeltaTime);
	virtual void LateUpdate(float DeltaTime);

	virtual void EarlyDraw();
	virtual void Draw();
	virtual void LateDraw();
};
