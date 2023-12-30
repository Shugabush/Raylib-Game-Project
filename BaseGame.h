#pragma once
#include "GameObject.h"
#include <vector>

class BaseGame
{
	std::vector<GameObject*> GameObjects;

public:
	void Start();
	void Update(float DeltaTime);
	void Draw();
};