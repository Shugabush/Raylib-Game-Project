#pragma once
#include "raylib.h"

class InputManager
{
	InputManager() = default;

public:
	static Vector2 GetMovement();
	static Vector2 GetNormalizedMovement();
};