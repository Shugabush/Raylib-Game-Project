#pragma once
#include "GameObject.h"

class Player : public GameObject
{
public:
	Player();

	float MovementSpeed = 100;

	virtual void Update(float deltaTime) override;
};