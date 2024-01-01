#include "Player.h"
#include "InputManager.h"

Player::Player() : GameObject()
{
	Tag = "Player";
}

void Player::Update(float deltaTime)
{
	Velocity = InputManager::GetNormalizedMovement() * MovementSpeed * deltaTime;
}
